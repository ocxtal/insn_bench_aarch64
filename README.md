
# Instruction latency & throughput profiler for AArch64

## Run

```bash
$ git clone --recursive https://github.com/ocxtal/insn_bench_aarch64.git && cd insn_bench_aarch64
$ make -j                                         # needs gcc or clang
$ ./insn_bench_aarch64 > result.txt               # dump in tsv, -m for dump in markdown
```

## Results

* [Apple M1 Firestorm (mac mini 2020)](https://github.com/ocxtal/insn_bench_aarch64/blob/master/results/apple_m1_firestorm.md)

## Miscellaneous

My brief notes on M1 code tuning is [here](xxx).

## Copyright and License

Hajime Suzuki (2021), all the codes in this repository are licensed under MIT (not applied to submodules).
