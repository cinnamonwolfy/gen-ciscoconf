/***********************************\
* Cisco Config Generator, v0.55     *
* (c)2021 pocketlinux32, Under GPL3 *
* Types Source File                 *
\***********************************/
#include <pl32-memory.h>
#include <cisco-constants.h>
#include <cisco-types.h>

// Cisco Interface Structure
struct ciscoint {
	char* ports;			// Interface(s)
	int mode;			// Mode of the interface(s)
	char* allowed_vlans;		// VLANS allowed to cross the trunk. Only used if mode = "trunk"
	char* ip_addr;			/* IP address of the interface (VLANs for switches, everything else for routers). IP version
					automatically determined.*/
	int sub_mask;			// Subnet mask for IP address. CIDR preferred but not required
	char* gateway;			// Gateway for interface. Only used if ports = "vlan x"
}

// Cisco Table Structure

struct ciscotable {
	
}
