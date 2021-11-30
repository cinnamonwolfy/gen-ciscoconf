# `gen-ciscoconf` Syntax

DISCLAIMER: This software is no longer updated in this repository. The new repository
is pocketlinux32/gen-ciscoconf


As requested by `kirby`, here is the docs for `gen-ciscoconf`. A lot of the commands
are quite self-explanatory, but some of them are not trivial (such as the `vlan`
and `ether` commands. This was made to help clarify the usage of this program.
I hope this document accomplishes its goal.

## `hostname` Command
Usage: `hostname STRING`

```
STRING: Host name
```

Sets the hostname for the switch/router to `STRING`. Default value is "default".

## `enable_passwd` Command
Usage: `enable_passwd STRING`

STRING: Password

Sets the password for USER-EXEC mode (`#` prompt) to `STRING`.

## `enable_secret` Command
Usage: `enable_secret STRING`

```
STRING: Password
```

Sets the secret (encrypted password) for USER-EXEC mode (`#` prompt) to `STRING`.

## `line_passwd` Command
Usage: `line_passwd STRING`

```
STRING: Password
```

Sets the password for the login console to `STRING`.

## `vlan` Command

Usage: `vlan ID NAME PORT`

```
ID: VLAN ID number
NAME: VLAN name
PORTS: Port range assigned to VLAN ID
```

Creates a VLAN with VLAN number `ID`, name `NAME`, and assigns it to ports `PORTS`.

## `ether` Command

Usage: `ether ID PORTS MODE`

```
ID: EtherChannel number
PORTS: Port range assigned to EtherChannel
MODE: Communication mode for EtherChannel
```

Creates an EtherChannel interface with number `ID`, with `PORTS` assigned to it, and set to communication mode `MODE`
