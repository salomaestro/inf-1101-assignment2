# INF-1101 Assignment 2

*Author: Christian Salomonsen <christian.salomonsen@uit.no>*

My implementation of the second assignment for the course Data structures and Algorithms. The goal is to implement a hash-map ADT

## Objective

For this assignment there are two objectives:

1. Implement a map in C. Must have functionalities defined by the header file
   `map.h`.
2. Create a binary that gives a count to each unique word of input files. Built
   from `wordfreqs.c`

## Usage

The code is built for UNIX based systems and can be tested all together using the command:

```bash
make test
```

This will compile a binary `wordfreqs` and download data in the form of tweets
from Donald Trump, and parse a random selection into separate text files. This
functionality alone can be tested using the command `make data`.

### Gotchas

Be aware that when using the command `make clean`, all data in `data/` will be forcefully removed in addition to the regular object-files and binaries.
