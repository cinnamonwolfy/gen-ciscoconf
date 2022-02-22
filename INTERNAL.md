# Internals

This software consists of two parts: the backend (pl32lib+ciscolib) and the
frontend (gen-ciscoconf+cc-tui)

## Backend

The backend is mostly made up by ciscolib, which depends on pl32lib. Both of
these libraries have been built with portability in mind, and thus are strictly
C99 and POSIX compliant. You can compile it on any system that supports POSIX
(Windows users will need to compile it wiht Cygwin or WSL unless the terminal
communication module is disabled. If it is disabled, it will compile on any
system with a C99-compliant C compiler and C library).

### ciscolib

This library contains definitions for data structures and data parsers. It's the
core library of this toolset. This library depends on pl32lib.

### pl32lib

This library contains program-independent code that I use in any application I
make. This library includes routines for memory tracking, string tokenization,
command-line interpreters, and serial terminal communication. This library
does its best in sticking within C99, but the serial terminal module uses POSIX,
and thus isn't fully portable. If you want this library to have the serial
terminal support on Windows, you must compile it under Cygwin or WSL.

## Frontend

The frontend is made up of `gen-ciscoconf`, which requires ciscolib. `cc-tui` is
a TUI (terminal-based user interface) that's put on top of `gen-ciscoconf`.

### gen-ciscoconf

This is the main program of the toolset. It creates and modifies ciscolib data
structures gotten from files defining them, parses them out to a series of
Cisco IOS commands which are then sent down a serial connection to configure the
device (if pl32lib has the serial module enabled). It will also generate an
output file containing the parsed output commands if told to do so via a
command-line option.

### cc-tui

This is a ncurses-based user interface build on top of `gen-ciscoconf`. This
program is meant to make it easier to write configurations and communicate with
the device currently connected. It depends on GNU Ncurses, making it not fully
portable. Windows users will need to compile it with either Cygwin or WSL for
the moment being