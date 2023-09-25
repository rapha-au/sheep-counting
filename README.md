# sheep-counting

Timer software where instead of counting seconds it counts sheep.

## Compiling

ncurses being installed in your system is a pre-requisite for the compilation.

> g++ -o sheep-count main.cpp -lncurses
> ./sheep-count

## Usage

> sheep-count [sheep_to_count]

Each sheep is equal to one second, by default 60 sheep are counted (60 seconds).
