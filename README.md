# 11GNU

> **Coursework.** Written in 2024 for the course 11GNU *GNU Programming* at the
> Faculty of Nuclear Sciences and Physical Engineering, CTU in Prague.

A small Linux toolchain that turns a raw measurement recording into a graph:

1. `gnucode` (C) reads the binary samples, splits them into windows of N
   samples and writes the mean or the variance of each window.
2. `script.sh` (bash + sed) reads the amplifier gain and the sampling frequency
   from the measurement header and runs `gnucode`.
3. `plot.gpl` (gnuplot) converts the numbers to volts and draws `graph.png`.

The project is packaged with GNU Autotools.

## Layout

| Path | What it is |
|---|---|
| `src/code.c` | `gnucode`: windowed mean / variance, options parsed with `getopt_long` |
| `src/script.sh` | wrapper: parses the header, calls `gnucode`, calls gnuplot |
| `src/plot.gpl` | gnuplot script for the graph |
| `configure.ac`, `Makefile.am`, `src/Makefile.am` | Autotools build |

## Build

Needs gcc, make, autoconf, automake and gnuplot.

```bash
autoreconf -i
./configure
make
```

## Interface

```
gnucode --number-n=N --file=FILE.dat [--return-sigma]
script.sh -d FILE.txt -n N [-s true]
```

- `FILE.dat`: samples as 4-byte integers from a 16-bit converter.
- `FILE.txt`: the text header of the measurement. The script reads the lines
  `Pre-Amplifier Gain`, `Amplifier Gain` and `Sampling Freq.` from it.
- `gnucode` writes `plotData.dat`, one number per window; the script then
  draws `graph.png`.

## Licence

MIT, see `LICENSE`.
