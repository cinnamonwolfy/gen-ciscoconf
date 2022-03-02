# Ciscolib Documentation: Constants

Here is a list of all the constants defined in ciscolib

## Interface Type Constants

All interface type constants are prefixed with `CISCO_INT_` and are used to
determine what interface type a port is. Here is a list of valid interface
types:

- `CISCO_INT_F0` -> FastEthernet0/x

- `CISCO_INT_G0` -> GigabitEthernet0/x

- `CISCO_INT_G00` -> GigabitEthernet0/0/x

- `CISCO_INT_G01` -> GigabitEthernet0/1/x

- `CISCO_INT_S00` -> Serial0/0/x

- `CISCO_INT_S01` -> Serial0/1/x

- `CISCO_INT_PORTCH` -> Port-Channel

- `CISCO_INT_VLAN` -> VLAN

## Mode Constants

All mode constants are prefixed with `CISCO_MODE_` and are used to set the
mode of an interface/port-channel.

### Interface and Port-Channel

Here is a list of valid modes for both interface and port-channel:

- `CISCO_MODE_AUTO` -> The switchport mode switches between access or trunk
depending on the connection.

- `CISCO_MODE_ACCESS` -> Sets the switchport mode to access mode

- `CISCO_MODE_TRUNK` -> Sets the switchport mode to trunk mode

### Port-Channel Only

Here is a list of valid modes for only port-channel:

- `CISCO_MODE_ACTIVE` -> Enables EtherChannel in LACP mode

- `CISCO_MODE_PASSIVE` -> Enables EtherChannel in LACP mode if connected to
another device in LACP mode

- `CISCO_MODE_DESIRABLE -> Enables EtherChannel in PAgP mode

- `CISCO_MODE_AUTO` -> Enables EtherChannel in PAgP mode if connected to
another device in PAgP mode

### Interface Only

The only valid mode constant that is for interfaces only is the
`CISCO_MODE_IN_PORTCH` constant. This constant is used to signal to the
interface modifier that this interface is in a Port-Channel and its mode cannot
be changed directly.
