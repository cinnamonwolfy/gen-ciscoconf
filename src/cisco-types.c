/***********************************\
* Cisco Config Generator, v0.56     *
* (c)2021 pocketlinux32, Under GPL3 *
* Types Source File                 *
\***********************************/
#include <cisco/cisco-constants.h>
#include <cisco/cisco-types.h>

// Cisco Interface Structure
struct ciscoint {
	ciscoconst_t type;
	ciscoconst_t mode;
	int ports[2];
	char* name;
	int* allowed_vlans;
	char* ip_addr;
	int sub_mask;
	char* gateway;
}

// Cisco Table Structure
struct ciscotable {
	ciscoconst_t type;
	ciscoint_t* interfaces;
}

ciscoint_t* ciscoCreateInterface(ciscoconst_t inttype, int port1, int port2){
	ciscoconst_t* returnInt = plGCMalloc(sizeof(ciscoint_t*));

	returnInt->type = inttype;
	returnInt->mode = CISCO_MODE_ACCESS;
	returnInt->ports[0] = port1;
	returnInt->ports[1] = port2;
	returnInt->name = NULL;
	returnInt->allowed_vlans = NULL;
	returnInt->ip_addr = NULL;
	returnInt->sub_mask = 0;
	returnInt->gateway = NULL;
}
