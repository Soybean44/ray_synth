# Ray Synth
Exploring how digital synthesis and audio works through real-time audio in Raylib

## How does it work?
Currently we use Wavetable Synthesis to generate the Audio Signal. The benefit of this is the signal itself is pre-computed. The wavetable itself is a 1Hz wave, We then essentially squash that 1Hz wave by sampling that wave at a different sample rate, defined by our target frequency. The most calculation this requires is linear interpolation between our samples, which is not very expensive.

The benefit of wavetable synthesis, is that you can easily add your own waveforms trivially. Create an array of samples that represents your waveform over one period, Then set the oscillator table to your wavetable. The sky is the limit for all the different waveforms you want to use.

## Building
The libraries used are Raylib, which is loaded in through pkg-config. Ensure you have both installed and configured correctly to build the program. 

### Code Gen
The build system works in two steps. First, it builds `gen.c` then it runs that to generate `src/gen.h`. This allows for "compile time generation" without the need of anything more than a C compiler. In theory, all code should run on other platforms, since I am using generic C file I/O, however if the script does not function correctly. Feel free to make an issue/pull request to fix it.

## Running

As of right now, when running the synth, it creates a virtual midi output through portmidi. You can connect this to any midi input through software such as qjackctl (assuming you're using pipewire, pipewire-alsa, and pipewire-jack). Then midi input should correspond to midi output from the synth.

