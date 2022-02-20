# gen-ciscoconf

**NOTICE: This is the old version of the software. Due to this project
a rewrite, the old source has been moved from `src` to here, alongside its
original helpdoc (I know it says for 0.54, but 0.55.3 is basically 0.54 with
some minor bug fixes). This version is deprecated and will be removed once the
rewrite is complete**

`gen-ciscoconf` is a configuration command generator for Cisco equipment. The
output of this program is meant to be copy-pasted onto the command prompt of the
switch/router.

Currently, it only has full support for switches and basic support for routers.

## Supported Features

On both switches and routers:

- Hostname and Date Configuration
- Console/Terminal Security

On switches only:

- VLAN Table
- Etherchannel Table

## Features under development

- Interface Configuration
- Remote Console Configuration
- IPv4/IPv6 Routing
- Port Security
- Load-balancing Configuration
