# Sunshine GPU

An emulator for a simple programmable GPU.  It is meant for anyone who is interested in the inner workings of a GPU; it includes a programmable vertex engine with its own instruction set.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine. This project was built on Ubuntu 16.04.

### Prerequisites

You'll need the SDL2 library:

```
apt-get install libsdl2-dev
```

### Building

Make a copy on your local machine and change directory into the project folder:

```
git clone https://github.com/mlugo2/SunshineGPU.git

cd SunshineGPU
```

Build the project:

```
make
```

### Running

To run the emulator you'll need a vertices object file, vertex shader hex file, and a constants file: 

```
./gpu teapot.off trans.hex ident.const
```