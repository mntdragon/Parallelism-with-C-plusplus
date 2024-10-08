from collections import OrderedDict
from typing import Any, List, Dict, Optional, Union
import json
import sys
import platform

from ppcgrader import BOX_PATH
from ppcgrader.config import Config
from ppcgrader.compiler import Compiler, CompilerOutput, analyze_compile_errors
from ppcgrader.runner import RunnerOutput, AsanRunnerOutput, MemcheckRunnerOutput, NvprofRunnerOutput

MAX_RUN_JSON_OUTPUT = 30000  # Characters

WVLA_ERROR = """
  It seems you tried to use Variable Length Arrays. These can lead to crashes
  if the allocated data exceeds the stack size. We have therefore disabled
  them for this course. Consider using a std::vector instead."""[1:]

OMP_TRUE_ERROR = """
  This omp pragma was ignored. Check it for typos if it was meant to be used."""[
    1:]

OMP_FALSE_ERROR = """
  Openmp is disabled for this exercise. The exercise templates for the
  different tasks (e.g CP2a, CP2b CP2c) are different. If you are working on a
  task that requires Openmp, be sure to download the correct exercise
  template: for example CP2b or MF2."""[1:]

ASAN_ASLR_BUG = """
Note: You might have encountered a bug in address sanitizer. You can
try disabling address space layout randomization by running with

setarch -R ./grading ...

More information: https://github.com/google/sanitizers/issues/1614"""[1:]

# TODO use the actual cache line size of the cpu
CACHE_LINE = 64


def bin_fmt(num: int):
    if num < 2 * 1024:
        return f'{num:.2f} '
    elif num < 2 * 1024 * 1024:
        return f'{num/1024/1024:.2f} M'
    else:
        return f'{num/1024/1024/1024:.2f} G'


def statistics_terminal(output):
    stat = output.statistics
    if stat is None:
        return None
    wallclock = stat.get('perf_wall_clock_ns', None)
    enabled = stat.get('perf_time_enabled_ns', None)
    running = stat.get('perf_time_running_ns', None)
    usr_time = stat.get('perf_time_usr_ns', None)
    sys_time = stat.get('perf_time_sys_ns', None)
    instrs = stat.get('perf_instructions', None)
    cycles = stat.get('perf_cycles', None)
    branches = stat.get('perf_branches', None)
    branch_misses = stat.get('perf_branch_misses', None)
    cache_refs = stat.get('perf_cache_refs', None)
    cache_misses = stat.get('perf_cache_misses', None)
    if not wallclock:
        return None
    if wallclock < 1e7:
        return None

    wallclock_secs = wallclock / 1e9
    exclude_kernel = stat.get('perf_exclude_kernel', True)

    if usr_time is not None and sys_time is not None:
        total_time = usr_time + sys_time
        if exclude_kernel:
            running = usr_time
    elif running is not None:
        total_time = running
    else:
        total_time = None

    r = ""
    if total_time:
        r += f'  Your code used {wallclock_secs:.3f} sec of wallclock time, and {total_time/1e9:.3f} sec of CPU time\n'
        r += f'  ≈ you used {total_time/wallclock:.1f} simultaneous hardware threads on average\n'
        if sys_time and sys_time > 0.01 * total_time and sys_time >= 1e7:
            r += f'  {sys_time/1e9:.3f} sec ({100 * sys_time  / total_time:.1f}%) were spent outside user space.\n'
            if exclude_kernel:
                r += '\n'
                r += f'  WARNING: No measurements were performed inside kernel calls.\n'
                r += f'  The numbers below may be inaccurate.\n'
        r += '\n'
    if cycles:
        r += f'  The total number of clock cycles was {cycles/1e9:.1f} billion\n'
        r += f'  ≈ CPU was running at {cycles/running:.1f} GHz\n\n'
    if instrs:
        r += f'  The CPU executed {instrs/1e9:.2f} billion machine-language instructions\n'
        r += f'  ≈ {instrs/wallclock:.2f} instructions per nanosecond\n'
        if cycles:
            r += f'  ≈ {instrs/cycles:.2f} instructions per clock cycle\n'
        r += '\n'
    if branches:
        r += f'  {branches/instrs*100:.1f}% of the instructions were branches\n'
        if branch_misses:
            r += f'  and {branch_misses/branches*100:.1f}% of them were mispredicted\n'
        r += '\n'

    generic_cache = False
    l3_refs = stat.get('perf_l3_read_refs', None)
    l3_misses = stat.get('perf_l3_read_misses', None)
    if l3_refs is None:
        # the generic cache events. their meaning is a bit diffuse, so they are only here as a fallback
        l3_refs = stat.get('perf_cache_refs', None)
        l3_misses = stat.get('perf_cache_misses', None)
        generic_cache = True
    if l3_refs and l3_misses:
        read_gb = l3_refs * CACHE_LINE / 1024 / 1024 / 1024
        miss_gb = l3_misses * CACHE_LINE / 1024 / 1024 / 1024
        r += f'  Your code read {l3_refs / 1e9:.2f} billion times from L3 cache.\n'
        r += f'  The miss rate was {l3_misses/l3_refs*100:.2f}%.\n'
        r += f'  ≈ {read_gb:.2f} GB ≈ {read_gb / wallclock_secs:.2f} GB/s of transfer between L3 and L2 cache\n'
        r += f'  ≈ {miss_gb:.2f} GB ≈ {miss_gb / wallclock_secs:.2f} GB/s of transfer between RAM and L3 cache\n'
        if generic_cache:
            r += f'  (Cache usage was measured using system-specific CACHE_REFS event,\n'
            r += f'   so these numbers might not correspond exactly to L3 reads)\n'
        r += '\n'

    l1_refs = stat.get('perf_l1_read_refs', None)
    l1_misses = stat.get('perf_l1_read_misses', None)
    if l1_refs and l1_misses:
        read_gb = l1_refs * CACHE_LINE / 1024 / 1024 / 1024
        miss_gb = l1_misses * CACHE_LINE / 1024 / 1024 / 1024
        r += f'  Your code read {l1_refs / 1e9:.2f} billion times from L1 cache.\n'
        r += f'  The miss rate was {l1_misses/l1_refs*100:.2f}%.\n'
        r += f'  ≈ {miss_gb:.2f} GB ≈ {miss_gb / wallclock_secs:.2f} GB/s of transfer between L2 and L1 cache\n'
        if instrs:
            r += f'  Your code performed {instrs/l1_refs:.2f} instructions per L1 cache access.\n'
        r += '\n'

    r = r.rstrip()
    if len(r):
        return r
    else:
        return None


def table(data: List[Dict[str, Any]], keys: List[str]):
    def to_str(v):
        if v is None:
            return ''
        if isinstance(v, float):
            return f'{v:f}'
        return str(v)

    # Compute field widths
    widths = [len(k) for k in keys]
    for row in data:
        widths = [
            max(w, len(to_str(row.get(k, '')))) for w, k in zip(widths, keys)
        ]

    r = ''
    r += ' | '.join(f"{k:{w}}" for w, k in zip(widths, keys))
    r += '\n'
    r += '-+-'.join("-" * w for w in widths)
    r += '\n'
    for row in data:
        r += ' | '.join(f"{to_str(row.get(k,'')):{w}}"
                        for w, k in zip(widths, keys))
        r += '\n'

    return r


def _safe_json_dump(data):
    return json.dumps(
        json.loads(json.dumps(data), parse_constant=lambda s: str(s)))


class Reporter:
    class TestGroup:
        def compilation(self,
                        compiler: Compiler) -> 'Reporter.CompilationProxy':
            raise NotImplementedError()

        def test(self, test: str, output: RunnerOutput):
            raise NotImplementedError()

    class BenchmarkGroup:
        def compilation(self,
                        compiler: Compiler) -> 'Reporter.CompilationProxy':
            raise NotImplementedError()

        def benchmark(self, test: str, output: RunnerOutput):
            raise NotImplementedError()

    class AnalysisGroup:
        def compilation(self,
                        compiler: Compiler) -> 'Reporter.CompilationProxy':
            raise NotImplementedError()

        def analyze(self, output, success=True):
            raise NotImplementedError()

    class CompilationProxy:
        def compile(self, *args, **kwargs) -> 'CompilerOutput':
            raise NotImplementedError()

    def __init__(self, config: Config):
        self.config = config

    def test_group(self, name: str, tests: List[str]) -> 'TestGroup':
        raise NotImplementedError()

    def benchmark_group(self, name: str, tests: List[str]) -> 'BenchmarkGroup':
        raise NotImplementedError()

    def analysis_group(self, name: str) -> 'AnalysisGroup':
        raise NotImplementedError()

    def log(self, msg: str, kind=None):
        raise NotImplementedError()

    def finalize(self):
        raise NotImplementedError()


class TerminalReporter(Reporter):
    def check_timeout(self, output: RunnerOutput):
        if not output.is_success():
            self.log_sep()
            if output.is_timed_out():
                self.log('It seems that your program timed out.')
                self.log(
                    f'The test should have ran in less than {output.timeout} seconds.'
                )
                self.log(
                    'You can override allowed running time with --timeout [timeout in seconds] or disable running time checks with --no-timeout.'
                )
            else:
                self.log('It seems that your program crashed unexpectedly.')
            self.log_sep()

    def _simplify_name(self, test: str):
        if self.config.on_remote and test.startswith(BOX_PATH):
            return test[len(BOX_PATH):]
        return test

    class TestGroup(Reporter.TestGroup):
        def __init__(self, reporter: 'TerminalReporter', tests: List[str]):
            self.reporter = reporter
            self.header_printed = False
            self.test_name_width = max(
                4,
                max(len(self.reporter._simplify_name(test)) for test in tests))

        def compilation(self,
                        compiler: Compiler) -> 'Reporter.CompilationProxy':
            return TerminalReporter.CompilationProxy(self.reporter, compiler)

        def test(self, test: str, output: RunnerOutput):
            if not self.header_printed:
                self.header_printed = True
                self.reporter.log(
                    f'{"test":<{self.test_name_width}}  {"time":>9}  {"result":6}',
                    'heading')
            if output.is_success():
                msg = "errors" if output.errors else "pass"
                self.reporter.log(
                    f'{self.reporter._simplify_name(test):<{self.test_name_width}}  {output.time:>8.3f}s  {msg:6}',
                    'error' if output.errors else 'pass')
            else:
                self.reporter.log(
                    f'{self.reporter._simplify_name(test):<{self.test_name_width}}  [failed]',
                    'error')

            if output.stdout:
                self.reporter.log_sep()
                self.reporter.log('Standard output:')
                self.reporter.log(output.stdout, 'output')
                self.reporter.log_sep()

            if output.stderr:
                self.reporter.log_sep()
                self.reporter.log('Standard error:')
                self.reporter.log(output.stderr, 'output')
                self.reporter.log_sep()

            self.reporter.check_timeout(output)

            if isinstance(output, AsanRunnerOutput) and output.asanoutput:
                self.reporter.log_sep()
                self.reporter.log(
                    'AddressSanitizer reported the following errors:')
                self.reporter.log(output.asanoutput, 'output')
                self.reporter.log_sep()

            if platform.system() == 'Linux' and isinstance(
                    output, AsanRunnerOutput) and not output.run_successful:
                # Check for ASAN bug with ASLR vm.mmap_rnd_bits=32
                # See https://github.com/google/sanitizers/issues/1614
                # A common symptom seems to be that we get multiple lines of AddressSanitizer:DEADLYSIGNAL
                if "AddressSanitizer:DEADLYSIGNAL\nAddressSanitizer:DEADLYSIGNAL" in output.stderr:
                    self.reporter.log_sep()
                    self.reporter.log(ASAN_ASLR_BUG)
                    self.reporter.log_sep()

            if isinstance(output,
                          MemcheckRunnerOutput) and output.memcheckoutput:
                self.reporter.log_sep()
                self.reporter.log('Memcheck reported the following errors:')
                self.reporter.log(output.memcheckoutput, 'output')
                self.reporter.log_sep()

            if output.errors and not self.reporter.config.ignore_errors:
                human_readable = self.reporter.config.explain_terminal(
                    output, self.reporter.color)
                if human_readable is not None:
                    self.reporter.log_sep()
                    self.reporter.log(human_readable, 'preformatted')
                    self.reporter.log_sep()

    class BenchmarkGroup(Reporter.BenchmarkGroup):
        def __init__(self, reporter: 'TerminalReporter', tests: List[str]):
            self.reporter = reporter
            self.header_printed = False
            self.test_name_width = max(
                4,
                max(len(self.reporter._simplify_name(test)) for test in tests))

        def compilation(self,
                        compiler: Compiler) -> 'Reporter.CompilationProxy':
            return TerminalReporter.CompilationProxy(self.reporter, compiler)

        def benchmark(self, test: str, output: RunnerOutput):
            if not self.header_printed:
                self.header_printed = True
                self.reporter.log(
                    f'{"test":<{self.test_name_width}}  {"time":>9}  {"result":6}',
                    'heading')
            if output.is_success():
                msg = "errors" if output.errors else "pass"
                self.reporter.log(
                    f'{self.reporter._simplify_name(test):<{self.test_name_width}}  {output.time:>8.3f}s  {msg:6}',
                    'error' if output.errors else 'pass')
            else:
                self.reporter.log(
                    f'{self.reporter._simplify_name(test):<{self.test_name_width}}  [failed]',
                    'error')

            if output.stdout:
                self.reporter.log_sep()
                self.reporter.log('Standard output:')
                self.reporter.log(output.stdout, 'output')
                self.reporter.log_sep()

            if output.stderr:
                self.reporter.log_sep()
                self.reporter.log('Standard error:')
                self.reporter.log(output.stderr, 'output')
                self.reporter.log_sep()

            self.reporter.check_timeout(output)

            if output.is_success() and not output.errors:
                human_readable = statistics_terminal(output)
                if human_readable is not None:
                    self.reporter.log_sep()
                    self.reporter.log(human_readable, 'preformatted')
                    self.reporter.log_sep()

                if isinstance(output, NvprofRunnerOutput):
                    if output.nvprof:
                        if output.nvprof.get('gpu_trace') is not None:

                            def safe_scale(value, scale):
                                if value is not None:
                                    return value * scale
                                return value

                            # Pick and format relevant columns for printing
                            t = []
                            for row in output.nvprof['gpu_trace']:
                                d = OrderedDict()

                                d['Start (s)'] = row['Start s']
                                d['Duration (s)'] = row['Duration s']
                                if row['Grid X'] and row['Grid Y'] and row[
                                        'Grid Z']:
                                    d['Grid Size'] = f"{row['Grid X']}, {row['Grid Y']}, {row['Grid Z']}"
                                else:
                                    d['Grid Size'] = ''
                                if row['Block X'] and row['Block Y'] and row[
                                        'Block Z']:
                                    d['Block Size'] = f"{row['Block X']}, {row['Block Y']}, {row['Block Z']}"
                                else:
                                    d['Block Size'] = ''
                                d['Regs'] = row['Registers Per Thread']
                                d['SMem (B)'] = row['Static SMem bytes']
                                d['DMem (B)'] = row['Dynamic SMem bytes']
                                d['Size (MB)'] = safe_scale(
                                    row['Size bytes'], 1e-6)
                                d['Throughput (GB/s)'] = safe_scale(
                                    row['Throughput bytes/s'], 1e-9)
                                d['Name'] = row['Name']

                                t.append(d)

                            if t:
                                self.reporter.log_sep()
                                self.reporter.log('Nvprof GPU trace:')
                                self.reporter.log(table(t, t[0].keys()),
                                                  'output')
                                self.reporter.log_sep()

                        if output.nvprof.get('gpu_trace_message') is not None:
                            self.reporter.log_sep()
                            self.reporter.log(
                                output.nvprof['gpu_trace_message'])
                            self.reporter.log_sep()

                    elif output.nvprof_raw:
                        self.reporter.log_sep()
                        self.reporter.log(
                            'Failed to parse nvprof output. Here it is in raw form:'
                        )
                        self.reporter.log(output.nvprof_raw, 'output')
                        self.reporter.log_sep()
                    else:
                        self.reporter.log_sep()
                        self.reporter.log('No output from nvprof')
                        self.reporter.log_sep()

    class AnalysisGroup(Reporter.AnalysisGroup):
        def __init__(self, name: str, reporter: 'TerminalReporter'):
            self.name = name
            self.reporter = reporter

        def compilation(self,
                        compiler: Compiler) -> 'Reporter.CompilationProxy':
            return TerminalReporter.CompilationProxy(self.reporter, compiler)

        def analyze(self, output, success=True):
            self.reporter.log(f'Output for {self.name}:', 'heading')
            self.reporter.log(output, 'output')

    class CompilationProxy(Reporter.CompilationProxy):
        def __init__(self, reporter: 'TerminalReporter', compiler: Compiler):
            self.reporter = reporter
            self.compiler = compiler

        def compile(self, *args, **kwargs) -> 'CompilerOutput':
            self.reporter.log('Compiling...')
            result = self.compiler.compile(*args, **kwargs)
            if result.stdout:
                self.reporter.log_sep()
                self.reporter.log('Compiler stdout:')
                self.reporter.log(result.stdout, 'output')
                self.reporter.log_sep()
            if result.stderr:
                self.reporter.log_sep()
                self.reporter.log('Compiler stderr:')
                self.reporter.log(result.stderr, 'output')
                self.reporter.log_sep()
            if result.is_success():
                self.reporter.log('Compiled')
            else:
                self.reporter.log('Compilation failed!', 'error')
                self.reporter.log_sep()

            self.print_errors(result.stderr)

            return result

        def print_errors(self, stderr: str):
            errors = analyze_compile_errors(stderr)
            lines = stderr.splitlines()
            for error in errors:
                if error['type'] == "Wvla":
                    self.reporter.log(lines[error['line']], 'error')
                    self.print_context(lines, error['line'])
                    self.reporter.log_sep()
                    self.reporter.log(WVLA_ERROR, 'output')
                    self.reporter.log_sep()
                elif error['type'] == "omp":
                    self.reporter.log(lines[error['line']], 'error')
                    self.print_context(lines, error['line'])
                    self.reporter.log_sep()
                    if self.reporter.config.openmp:
                        self.reporter.log(OMP_TRUE_ERROR, 'output')
                    else:
                        self.reporter.log(OMP_FALSE_ERROR, 'output')
                    self.reporter.log_sep()

        def print_context(self, error_lines: List[str], error_index: int):
            """
            Given a list of lines that are the output of the compiler, and
            an index into this list that points to the start of a specific error
            message, this function attempts to print the entire context of the
            error, i.e., it will print the lines *following* `error_index`
            until it reaches a line that does not start with whitespace, i.e.
            is not indented.
            :param error_lines: List of lines in the compiler output
            :param error_index: Index into that list
            """
            for i in range(error_index + 1, len(error_lines)):
                if error_lines[i][0].isspace():
                    self.reporter.log(error_lines[i], 'output')
                else:
                    break

    def __init__(self, config: Config, color: Optional[bool] = None):
        super().__init__(config)
        self.color = sys.stdout.isatty() if color is None else color
        self.sep_printed = False

    def test_group(self, name: str, tests: List[str]) -> 'TestGroup':
        return TerminalReporter.TestGroup(self, tests)

    def benchmark_group(self, name: str, tests: List[str]) -> 'BenchmarkGroup':
        return TerminalReporter.BenchmarkGroup(self, tests)

    def analysis_group(self, name: str) -> 'AnalysisGroup':
        return TerminalReporter.AnalysisGroup(name, self)

    def log_sep(self):
        if not self.sep_printed:
            print()
            self.sep_printed = True

    def log(self, msg: str, kind=None):
        msg = msg.rstrip()
        before, after = '', ''
        if kind is not None and self.color:
            reset = '\033[0m'
            before, after = {
                'title': ('\033[34;1m', reset),
                'heading': ('\033[1m', reset),
                'error': ('\033[31;1m', reset),
                'pass': ('\033[34m', reset),
                'command': ('\033[34m', reset),
                'output': ('\033[34m', reset),
            }.get(kind, ('', ''))
        print(before + msg + after)
        self.sep_printed = False

    def finalize(self):
        pass


def limit_output_for_json(output: str | None) -> str | None:
    """Limit the output to roughly MAX_RUN_JSON_OUTPUT characters.
    
    Note that the output may be up to 3 characters or 7 bytes longer than the
    limit because of the added ellipsis."""
    if output is None:
        return None
    if len(output) < MAX_RUN_JSON_OUTPUT:
        return output
    # Output too long. Try to cut it at a line break
    output = output[:MAX_RUN_JSON_OUTPUT]
    last_line_break = output.rfind("\n")
    if last_line_break == -1:
        # Output doesn't contain any line breaks. So just add ellipsis
        # at the end, and another on the next line
        return output + "…\n…"
    if last_line_break < MAX_RUN_JSON_OUTPUT - MAX_RUN_JSON_OUTPUT / 10:
        # Let's not remove over 10% of the students output. Again,
        # just add the ellipsis at the end of the line
        return output + "…\n…"
    # Okay, found a sensible place to add a break.
    return output[:last_line_break] + "\n…"


def output_to_json(test: str, output: RunnerOutput, benchmark: bool,
                   export_streams: bool) -> Dict[str, str]:
    result = {
        'name': test,
        'test': open(test, 'r').read(),
        'success': output.is_success(),
    }
    if output.is_success():
        result['time'] = output.time
        result['errors'] = output.errors
        if output.errors:
            result['input'] = output.input_data
            result['output'] = output.output_data
            result['output_errors'] = output.output_errors
        if benchmark:
            result['statistics'] = output.statistics
    else:
        result['timed_out'] = output.is_timed_out()
    if isinstance(output, AsanRunnerOutput):
        result['asanoutput'] = limit_output_for_json(output.asanoutput)
    if isinstance(output, MemcheckRunnerOutput):
        result['memcheckoutput'] = limit_output_for_json(output.memcheckoutput)
    if isinstance(output, NvprofRunnerOutput):
        result['nvprof'] = output.nvprof
    if export_streams:
        result['stdout'] = limit_output_for_json(output.stdout)
        result['stderr'] = limit_output_for_json(output.stderr)
    return result


class JsonReporter(Reporter):
    class TestGroup(Reporter.TestGroup):
        def __init__(self, reporter: 'JsonReporter', name: str):
            self.reporter = reporter
            self.name = name
            self.compiler_output = None
            self.tests = []

        def compilation(self,
                        compiler: Compiler) -> 'Reporter.CompilationProxy':
            if self.compiler_output is not None:
                raise RuntimeError(
                    'Must not compiler code twice in test group')
            self.compiler_output = {}
            return JsonReporter.CompilationProxy(self.compiler_output,
                                                 compiler)

        def test(self, test: str, output: RunnerOutput):
            self.tests.append(
                output_to_json(test, output, False,
                               self.reporter.config.export_streams))

        def is_success(self):
            if not self.compiler_output:
                return False

            return self.compiler_output['status'] == 0 and all(
                test['success'] and test['errors'] == 0 for test in self.tests)

        def to_json(self):
            return {
                'name': self.name,
                'compiler_output': self.compiler_output,
                'tests': self.tests,
            }

    class BenchmarkGroup(Reporter.BenchmarkGroup):
        def __init__(self, reporter: 'JsonReporter', name: str):
            self.reporter = reporter
            self.name = name
            self.compiler_output = None
            self.benchmarks = []

        def compilation(self,
                        compiler: Compiler) -> 'Reporter.CompilationProxy':
            if self.compiler_output is not None:
                raise RuntimeError(
                    'Must not compiler code twice in benchmark group')
            self.compiler_output = {}
            return JsonReporter.CompilationProxy(self.compiler_output,
                                                 compiler)

        def benchmark(self, test: str, output: RunnerOutput):
            self.benchmarks.append(
                output_to_json(test, output, True,
                               self.reporter.config.export_streams))

        def is_success(self):
            if not self.compiler_output:
                return False

            return self.compiler_output['status'] == 0 and all(
                benchmark['success'] and benchmark['errors'] == 0
                for benchmark in self.benchmarks)

        def to_json(self):
            return {
                'name': self.name,
                'compiler_output': self.compiler_output,
                'benchmarks': self.benchmarks,
            }

    class AnalysisGroup(Reporter.AnalysisGroup):
        def __init__(self, name: str):
            self.name = name
            self.compiler_output = None
            self.output = None
            self.success = False

        def compilation(self,
                        compiler: Compiler) -> 'Reporter.CompilationProxy':
            if self.compiler_output is not None:
                raise RuntimeError(
                    'Must not compiler code twice in analysis group')
            self.compiler_output = {}
            return JsonReporter.CompilationProxy(self.compiler_output,
                                                 compiler)

        def analyze(self, output, success=True):
            if self.output is not None:
                raise RuntimeError('Analysis must be recorded only once')
            self.output = output

        def is_success(self):
            return self.success

        def to_json(self):
            return {
                'name': self.name,
                'compiler_output': self.compiler_output,
                'output': self.output,
                'success': self.success,
            }

    class CompilationProxy(Reporter.CompilationProxy):
        def __init__(self, output: Dict[str, Union[str, int, list]],
                     compiler: Compiler):
            self.output = output
            self.compiler = compiler

        def compile(self, *args, **kwargs) -> 'CompilerOutput':
            result = self.compiler.compile(*args, **kwargs)
            self.output['status'] = result.returncode
            self.output['stdout'] = result.stdout
            self.output['stderr'] = result.stderr
            errors = analyze_compile_errors(result.stderr)
            self.output['errors'] = errors
            return result

    def __init__(self, config: Config):
        super().__init__(config)
        self.test_groups = []
        self.benchmark_groups = []
        self.analysis_groups = []

    def test_group(self, name: str, tests: List[str]) -> 'TestGroup':
        group = JsonReporter.TestGroup(self, name)
        self.test_groups.append(group)
        return group

    def benchmark_group(self, name: str, tests: List[str]) -> 'BenchmarkGroup':
        group = JsonReporter.BenchmarkGroup(self, name)
        self.benchmark_groups.append(group)
        return group

    def analysis_group(self, name: str) -> 'AnalysisGroup':
        group = JsonReporter.AnalysisGroup(name)
        self.analysis_groups.append(group)
        return group

    def log(self, msg: str, kind=None):
        pass  # No logging with json output

    def log_sep(self):
        pass  # No logging with json output

    def finalize(self):
        print(_safe_json_dump(self.to_json()))

    def to_json(self):
        return {
            'success':
            all(group.is_success() for group in self.test_groups)
            and all(group.is_success() for group in self.benchmark_groups)
            and all(group.is_success() for group in self.analysis_groups),
            'tests': [group.to_json() for group in self.test_groups],
            'benchmarks': [group.to_json() for group in self.benchmark_groups],
            'analyses': [group.to_json() for group in self.analysis_groups],
        }
