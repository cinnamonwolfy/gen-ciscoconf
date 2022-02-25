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
	uint16_t ports[2];
	char description[4096];
	size_t allvlan_amnt;
	uint16_t* allowed_vlans;
	char ip_addr[16];
	uint16_t sub_mask;
	char gateway[16];
}

// Cisco Table Structure
struct ciscotable {
	ciscoconst_t type;
	ciscoconst_t mode;
	char name[128];
	size_t size;
	ciscoint_t** interfaces;
}

// Allocates memory for an interface structure and returns it
ciscoint_t* ciscoCreateInterface(ciscoconst_t type, uint16_t port1, uint16_t port2){
	ciscoint_t* returnInt = plGCMalloc(sizeof(ciscoint_t));

	returnInt->type = type;
	returnInt->mode = CISCO_MODE_ACCESS;
	returnInt->ports[0] = port1;
	returnInt->ports[1] = port2;
	returnInt->name = NULL;
	returnInt->allowed_vlans = NULL;
	returnInt->ip_addr = NULL;
	returnInt->sub_mask = 0;
	returnInt->gateway = NULL;

	return returnInt;
}

// Allocates memory for a table structure and returns it
ciscotable_t* ciscoCreateTable(ciscoconst_t type, ciscoconst_t mode){
	ciscotable_t* returnTable = plGCMalloc(sizeof(ciscotable_t));

	returnTable->type = type;
	returnTable->mode = mode;
	returnTable->size = 0;
	returnTable->interfaces = plGCMalloc(2 * sizeof(ciscoint_t*));

	return returnTable;
}

// Modifies attributes in an interface
int ciscoModifyInterface(ciscoint_t* interface, ciscoconst_t modType, ...){
	va_list values;
	va_start(values, modType);
	ciscoconst_t constant;
	char* string;
	uint16_t numbers[2];

	// Type parser
	switch(modType){
		case CISCO_MODTYPE_TYPE: ;
		case CISCO_MODTYPE_MODE: ;
			constant = va_arg(values, ciscoconst_t);
			break;
		case CISCO_MODTYPE_DESC: ;
		case CISCO_MODTYPE_IP_ADDR: ;
		case CISCO_MODTYPE_GATEWAY: ;
			string = va_arg(values, char*);
			break;
		case CISCO_MODTYPE_PORTS: ;
			numbers[0] = va_arg(values, uint16_t);
		case CISCO_MODTYPE_ALLOW_VLAN: ;
		case CISCP_MODTYPE_SUBMASK: ;
			numbers[1] = va_arg(values, uint16_t);
			break;
		default:
			return 1;
	}

	//
	switch(modType){
		case CISCO_MODTYPE_TYPE: ;
			if(constant < 0 || constant > 7)
				return 2;

			
	}

	return 0;
}

// Adds an interface to a table
int ciscoAddInterface(ciscotable_t* table, ciscoint_t* interface){
	if(table->size > 1){
		void* tempPtr = plGCRealloc(table->interfaces, (table->size + 1) * sizeof(ciscoint_t*));

		if(!tempPtr){
			return 1;
		}

		table->interfaces = tempPtr;
	}

	table->interfaces[table->size] = interface;
	interface->mode = CISCO_MODE_IN_VLAN;
	table->size++;

	return 0;
}

// Gets a pointer to an interface from a table
ciscoint_t* ciscoGetInterface(ciscotable_t* table, int index){
	if(index < 0 || index > table->size-1){
		return NULL;
	}

	return table->interfaces[index];
}

char* ciscoParseInterface(ciscoint_t* interface){
	//TODO: add code to parse interfaces
}

char* ciscoParseTable(ciscotable_t* table){
	//TODO: add code to parse tables
}

void ciscoPrintInterface(ciscoint_t* interface){
	//TODO: add code to print interfaces
}

void ciscoPrintTable(ciscotable_t* table){
	//TODO: add code to print tables
}
