# Cisco Configurator (gen-ciscoconf)

**NOTICE: This project is currently undergoing a rewrite. Expect it to have
bugs, missing features and crappy documentation. I will do my best to fix all
of these issues soon. If you'd like to use the old version of the program, go
to the `old` folder**

Cisco Configurator is a library and a toolset to aid with the configuration of
Cisco equipment, mainly switches and routers.

# Build Instructions

**NOTICE: Due to this project not being complete yet, the build system won't
compile everything yet. Currently, `ciscolib` is the only one that compiles.**

The build process goes as follows:

1. pl32lib
2. ciscolib
3. gen-ciscoconf
4. cc-fltk

## [pl32lib](https://github.com/pocketlinux32/pl32lib)

Download the pl32lib source from the link above, and follow the instructions
outlined in the README.md

## ciscolib

Run `./compile build` from the project folder to compile and `./compile install`
to install to your system

## gen-ciscoconf

Run `./compile build` from the project folder to compile

## cc-fltk (optional)

Run the following from the `gen-ciscoconf` project folder:

```
cd src
gcc -I../include -I/path/to/pl32/headers -L. -L/path/to/libpl32 -lfltk cc-fltk.c \
-o cc-fltk
```
