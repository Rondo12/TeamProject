# TeamProject
CompSci Team Term Project: theoretical vs. actual performance of sorting algorithms.

## Files
- `sorts.h`, `sorts.cpp`: the 8 sorting algorithms. Comparison-based sorts count key comparisons in `comparisons`.
- `main.cpp`: generates inputs, times each sort, writes `results.csv`.

## Build and run
```
g++ -O2 -static -o sortbench main.cpp sorts.cpp
./sortbench
```
`-static` avoids loading a mismatched libstdc++ DLL on Windows (crashes at startup).

## Setup
- Inputs: values in [0, n-1].
  - `sorted`: 0..n-1
  - `reversed`: n-1..0
  - `random`: uniform random, with repeats
  - `equal`: all n/2
  - `mergeworst`: permutation that forces max merge sort comparisons
- Same input given to every algorithm (fixed seed per input type, n, rep).
- Sizes: O(n^2) sorts (insertion, selection, bubble) 1000..32000; others 1000..1024000. Doubling.
- 5 reps per test. Time with `chrono::steady_clock`, in ms.
- Each output is checked for sortedness.
- Quicksort: median of three pivot, Hoare partition.
- Bubble sort stops early after a pass with no swaps.
- Radix sort: LSD, base 10.

## Output
`results.csv`: `algorithm,input,n,rep,time_ms,comparisons` (comparisons = 0 for counting/radix).
