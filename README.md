# Cisco Configurator (gen-ciscoconf)

**NOTICE: This project is currently undergoing a rewrite. Expect it to have
bugs, missing features and crappy documentation. I will do my best to fix all
of these issues soon. If you'd like to use the old version of the program, go
to the `old` folder**

Cisco Configurator is a library and a toolset to aid with the configuration of
Cisco equipment, mainly switches and routers.

# Build Instructions

**NOTICE: There is no build system set in place yet, so you'll have to compile
it manually. The build system is being worked on currently. This will only cover
building it on Unix-like/Cygwin/WSL systems with the GCC compiler**

The build process goes as follows:

1. pl32lib
2. ciscolib
3. gen-ciscoconf
4. cc-tui

## [pl32lib](https://github.com/pocketlinux32/pl32lib)

Download the pl32lib source from the link above, and follow the instructions
outlined in the README.md

## ciscolib

Run the following from the gen-ciscoconf project folder:

```
cd src
# For shared
gcc -fPIC -c -I../include cisco-types.c -o libcisco.o
gcc -shared libcisco.o -o libcisco.so
# For static
gcc -c -I../include cisco-types.c -o libcisco.o
ar rc libcisco.a libcisco.o
```

## gen-ciscoconf

Run the following from the gen-ciscoconf project folder:

```
cd src
gcc -I../include -I/path/to/pl32/headers -L. -L/path/to/libpl32 gen-ciscoconf.c -o gen-ciscoconf
```

## cc-tui (optional)

Run the following from the gen-ciscoconf project folder:

```
cd src
gcc -I../include -I/path/to/pl32/headers -L. -L/path/to/libpl32 -lncurses cc-tui.c -o cc-tui
```
