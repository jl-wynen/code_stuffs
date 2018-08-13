Lib google/benchmark (github)

Tool perf
 - compile with -g -fno-omit-frame-pointer
 - perf record
 - perf report -g 'graph,0.5,caller'
      options mean: use total percentages, filter out stuff below 0.5%,
                    show graph with respect to callers (shows with respect to callees normally)
      or: just use -G
                                 
Swtich off optimization with
```cpp
static void escape(void *p) {
    asm volatile("" : : "g"(p) : "memory");
}
```
means:
    - asm: inline assembly (not portable!)
    - volatile: has side effects
    - assembly: pass in p and get nothing out but "might touch all of memory"

Similar:
```cpp
static void clobber() {
    asm volatile("" : : : "memory");
}
```

Use like
```cpp
std::vector<int> v;
escape(&v);
```
to keep v on the stack.

*Better*: use benchmark::DoNotOptimize(...)


Example (using googles benchmarks):
```cpp
static void bench_create(benchmark::State &state) {
    while (state.KeepRunning()) {
        std::vector<int> v;
        escape(&v);
        (void)v;
    }
}
BENCHMARK(bench_create);

static void bench_reserve(benchmark::State &state) {
    while (state.KeepRunning()) {
        std::vector<int> v;
        v.reserve(1);
        escape(v.data);  // only really need data not the whole vector
    }
}

static void bench_reserve(benchmark::State &state) {
    while (state.KeepRunning()) {
        std::vector<int> v;
        v.reserve(1);
        escape(v.data);
        v.push_back(42);
        clobber();  // make sure that pushed back int is "observed"
    }
}
BENCHMARK(bench_create);
```
