# Deprecation

Since the [May 13th update](https://store.steampowered.com/news/app/730/view/529846144626329750) this fix has been implemented in the game.

Original README follows:

# wayland_fix

Fix for mouse grabbing and text input events in Counter Strike 2.

[Video](https://www.youtube.com/watch?v=5LAEN4oAKMc)

Please mind that the code is very much not clean. I only spend about 3 hours writing this, most of the time I spent reversing.

## Requirements

You need the following packages:

- SDL3 (headers should be enough)
- CMake
- A compiler capable of building C++ programs (gcc 15 is tested, clang should also work)

## Build instructions

```
$ git clone https://github.com/Sumandora/wayland_fix
$ cd wayland_fix
$ mkdir build
$ cd build
$ cmake ..
$ make -j
```

A file with the name `libwayland_fix.so` should have been created in the directory your shell is now in.

## Injection

Use gdb to inject the shared library into the game. Perhaps you find [CoreInject](https://github.com/Sumandora/CoreInject) interesting.

## VAC safe?

I don't know, I have friends doing much more aggressive changes to the game and VAC does not seem to take action.
On the other side, I also couldn't care less anymore since Valve seems more interested in [breaking](https://github.com/ValveSoftware/Source-1-Games/issues/6868) every remaining game they have, rather than actually shipping something working for once.
