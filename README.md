# Ray Synth
Exploring how digital synthesis and audio works through real-time audio in Raylib

## Building
The libraries used are Raylib, which is loaded in through pkg-config. Ensure you have both installed and configured correctly to build the program. 

### Code Gen
The build system works in two steps. First, it builds `gen.c` then it runs that to generate `src/gen.h`. This allows for "compile time generation" without the need of anything more than a C compiler. In theory, all code should run on other platforms, since I am using generic C file I/O, however if the script does not function correctly. Feel free to make an issue/pull request to fix it.

## Running

As of right now, when running the synth, clicking on the window at a given y sets the frequency of the oscillator. Right clicking switches the table 
