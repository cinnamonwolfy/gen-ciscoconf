# `gen-ciscoconf` syntax

This is the syntax of `gen-ciscoconf` version 0.54.

## Types

The few types recognized by the parser are:

- `number`: A whole number from 0 to 65535. The C/C++ equivalent would be `unsigned short` (16-bit or higher) or `uint16_t`.
- `string`: An array of characters. Maximum length is 4096. The C/C++ equivalent would be `char[4096]`.
- `mode`: A special token that designates the mode of an interface. Internally an `int`.

## Commands

### `hostname` command

```
hostname [host]

host: A string containing the hostname.
```

Sets the hostname

### `enable_passwd` command

```
enable_passwd [passwd]

passwd: A string containing the password, in plaintext.
```

Sets the USER-EXEC password.

### `enable_secret` command

```
enable_secret [passwd]

passwd: A string containing the password, in plaintext.
```

Sets the USER-EXEC secret. Overrides `enable_passwd`

### `line_passwd`

```
line_passwd [passwd]

passwd: A string containing the password, in plaintext
```

Sets the console password

### `vlan` command

```
vlan [id] [ports] [name] [wmode]

id: A number defining the VLAN ID.
	Valid values: 1-4096
ports: A string containing the interfaces that will be assigned to this VLAN.
name: A string containing the name of the vlan
wmode: A mode defining the mode of communication of the VLAN.
	Valid values: access, trunk
```

Adds a VLAN to the VLAN table

### `ether` command

```
ether [channel-group] [ports] [wmode]

channel-group: A number defining the channel group the ports will be added to.
	Valid values: 1-6
ports: A string containing the interfaces that will be added to the channel-group.
wmode: A mode defining the mode of communication of the EtherChannel
	Valid values: active, passive, desirable, auto, on
```

Creates an EtherChannel
