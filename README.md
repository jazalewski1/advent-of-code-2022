My solutions to Advent of Code 2022 puzzles. Nothing fancy, just as an exercice for my brain :)

Project is CMake based. To generate files (with ninja for example) run:
```
cmake -G Ninja -B build
```

Each day is a separate target named `d<day_number>p<part_number>`, for example `day 3 - part 2 = d3p2`.

So to compile run:
```
ninja all
# OR
ninja d3p2
```

To execute any solution you need to provide your own input files and pass them to binaries as first argument, for example:
```
build/bin/d3p2 day3/input.txt
```

There is also a bash script to run each solution, which needs the `input.txt` in day directory.
```
./run day3 part2
```