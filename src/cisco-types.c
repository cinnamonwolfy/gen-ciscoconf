/***********************************\
* Cisco Config Generator, v0.56     *
* (c)2021 pocketlinux32, Under GPL3 *
* Types Source File                 *
\***********************************/
#include <cisco-constants.h>
#include <cisco-types.h>

// Cisco Interface Structure
struct ciscoint {
	ciscoconst_t type;
	ciscoconst_t mode;
	int ports[2];
	char* name;
	char* allowed_vlans;
	char* ip_addr;
	int sub_mask;
	char* gateway;
}

// Cisco Table Structure
struct ciscotable {
	ciscoconst_t type;
	ciscoint_t* interfaces;
}

