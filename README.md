
# Instruction latency & throughput profiler for AArch64

## Run

```bash
$ git clone --recursive https://github.com/ocxtal/insn_bench_aarch64.git && cd insn_bench_aarch64
$ make -j                                         # needs gcc or clang
$ ./insn_bench_aarch64 > result.txt               # dump in tsv, -m for dump in markdown
```

## Results

* [Apple M1 Firestorm](https://github.com/ocxtal/insn_bench_aarch64/blob/master/results/apple_m1_firestorm.md)
* [aws Graviton 3](https://github.com/ocxtal/insn_bench_aarch64/blob/master/results/aws_graviton3.md)
* [aws Graviton 2](https://github.com/ocxtal/insn_bench_aarch64/blob/master/results/aws_graviton2.md)
* [GCP Ampere Altra](https://github.com/ocxtal/insn_bench_aarch64/blob/master/results/gcp_ampere_altra.md)
* [NXP LX2160A (Cortex-A72)](https://github.com/ocxtal/insn_bench_aarch64/blob/master/results/nxp_lx2160a_a72.md)

My brief notes on M1 code tuning is [here](https://github.com/ocxtal/insn_bench_aarch64/blob/master/optimization_notes_apple_m1.md).

## Copyright and License

Hajime Suzuki (2021-2022), all the codes in this repository are licensed under MIT (not applied to submodules).
