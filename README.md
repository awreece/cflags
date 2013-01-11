# A command line flag library for C #

## Why not use `getopt` ##

After using [`google-gflags`](https://code.google.com/p/gflags/), `getopt` 
became unbearable to use for command line flag parsing. I didn't want to have
to define a usage statement or write the boilerplate for every program. I didn't
want to have to define every flag in the main file (I want to define flags in
the library where they are used). I figured there had to be a better way, but 
I couldn't find it. So I made this library, inspired by `google-gflags`, to
avoid the aforementioned shortcomings of `getopt`.

Also, I use `getopt` internally (althought that may change at a later date -
I belive `getopt` stops parsing after seeing the first non-flag argument which
I don't quite agree with).

## Usage ##

To use this library, you define your flags anywhere you want to in your program
(at global scope) and then call `parse_flags(argc, argv)` from `main`. This
will define all the `getopt` boilerplate for you (and define a usage function) 
and call `getopt`, processing all the flags and permuting `argv` so the flag
options are removed and all the non-options are at the end.

This library defines several types for you to use. They all follow the same
interface: 

    FLAG_TYPE(<name>, <initial value>, <Usage string>);

The supported types and example usage are:

    // Compile with `gcc main.c flag.c`.

    #include "flag.h"

    FLAG_INT(num_threads, 4, "Number of threads to use");
    FLAG_BOOL(use_locks, false, "Use locks or atomic instructions");
    FLAG_DOUBLE(sleep_time, 0.3, "Time to sleep between iterations");
    FLAG_STRING(input, "hello.csv", "File to parse");

    int main(int argc, char** argv) {
      parse_flags(argc, argv);

      // Something interesting here!
    }

## How does it work? ##

You really would rather not know, but I use C preprocessor hackery to create
`__attribute__((constructor))` functions for each flag. These register the flag
in a global array that is passed to `getopt`.

## You don't support &lt;feature&gt; ##

Ask me for something and provide a compelling case for it (and hopefully a real
example that needs it) and I might do it. :) I'm <awreece at gmail dot com>.
