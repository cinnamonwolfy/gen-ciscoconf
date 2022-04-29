/************************************\
* Cisco Config Generator, v0.56      *
* (c)2021 pocketlinux32, Under GPLv3 *
* Ciscolib Source File               *
\************************************/
#include <ciscolib-const.h>
#include <ciscolib.h>

// Cisco Interface Structure
struct ciscoint {
	ciscoconst_t type;
	ciscoconst_t mode;
	uint8_t ports[2];
	char description[4096];
	plarray_t* allowedVlans;
	char ipAddr[46];
	uint8_t subMask;
	char gateway[46];
	size_t preExitBufmark;
};

// Cisco Table Structure
struct ciscotable {
	ciscoconst_t type;
	ciscoconst_t mode;
	char name[128];
	uint16_t number;
	plarray_t* interfaces;
};

// Converts a IPv4 CIDR subnet mask into an octet-based mask address
plarray_t* ciscoCidrToOctet(uint8_t cidrMask, plgc_t* gc){
	if(cidrMask > 32)
		return NULL;

	uint8_t ipBytes[4] = { 0, 0, 0, 0 };
	uint8_t ipAdding[4] = { 128, 128, 128, 128 };

	for(int i = 0; i < cidrMask; i++){
		if(i < 8){
			ipBytes[0] += ipAdding[0];
			ipAdding[0] = ipAdding[0] >> 1;
		}else if(i < 16){
			ipBytes[1] += ipAdding[1];
			ipAdding[1] = ipAdding[1] >> 1;
		}else if(i < 24){
			ipBytes[2] += ipAdding[2];
			ipAdding[2] = ipAdding[2] >> 1;
		}else{
			ipBytes[3] += ipAdding[3];
			ipAdding[3] = ipAdding[3] >> 1;
		}
	}

	plarray_t* retArray = plGCAlloc(gc, sizeof(plarray_t));
	retArray->array = plGCAlloc(gc, 4 * sizeof(uint8_t));
	retArray->size = 4;
	memcpy(retArray->array, ipAdding, 4 * sizeof(uint8_t));

	return retArray;
}

// Converts an interface constant into a string
char* ciscoGenerateIntString(ciscoconst_t type, plgc_t* gc){
	char* returnString = plGCCalloc(gc, 5, sizeof(char));

	switch(type){
		case CISCO_INT_F0: ;
			strcpy(returnString, "f0");
			break;
		case CISCO_INT_G0: ;
			strcpy(returnString, "g0");
			break;
		case CISCO_INT_G00: ;
			strcpy(returnString, "g0/0");
			break;
		case CISCO_INT_G01: ;
			strcpy(returnString, "g0/1");
			break;
		case CISCO_INT_S00: ;
			strcpy(returnString, "s0/0");
			break;
		case CISCO_INT_S01: ;
			strcpy(returnString, "s0/1");
			break;
		default:
			plGCFree(gc, returnString);
			returnString = NULL;
	}

	return returnString;
}

// Converts a string into an interface constant
ciscoconst_t ciscoStringToIntType(char* string){
	if(strstr(string, "f0/"))
		return CISCO_INT_F0;

	if(strstr(string, "g0/0/"))
		return CISCO_INT_G00;

	if(strstr(string, "g0/1/"))
		return CISCO_INT_G01;

	if(strstr(string, "s0/0/"))
		return CISCO_INT_S00;

	if(strstr(string, "s0/1/"))
		return CISCO_INT_S01;

	return CISCO_ERROR_INVALID_VALUE;
}

// Converts a mode constant into a string
char* ciscoGenerateModeString(ciscoconst_t mode, plgc_t* gc){
	char* returnString = plGCCalloc(gc, 7, sizeof(char));

	switch(mode){
		case CISCO_MODE_ACCESS: ;
			strcpy(returnString, "access");
			break;
		case CISCO_MODE_TRUNK: ;
			strcpy(returnString, "trunk");
			break;
		case CISCO_MODE_AUTO: ;
			strcpy(returnString, "auto");
			break;
		case CISCO_MODE_ACTIVE: ;
			strcpy(returnString, "active");
			break;
		case CISCO_MODE_PASSIVE: ;
			strcpy(returnString, "passive");
			break;
		case CISCO_MODE_DESIRABLE: ;
			strcpy(returnString, "desirable");
			break;
		default:
			plGCFree(gc, returnString);
			returnString = NULL;
	}

	return returnString;
}

// Converts a string into a mode constant
ciscoconst_t ciscoStringToMode(char* string){
	if(strcmp(string, "access") == 0)
		return CISCO_MODE_ACCESS;

	if(strcmp(string, "trunk") == 0)
		return CISCO_MODE_TRUNK;

	if(strcmp(string, "auto") == 0)
		return CISCO_MODE_AUTO;

	if(strcmp(string, "active") == 0)
		return CISCO_MODE_ACTIVE;

	if(strcmp(string, "passive") == 0)
		return CISCO_MODE_PASSIVE;

	if(strcmp(string, "desirable") == 0)
		return CISCO_MODE_DESIRABLE;

	return CISCO_ERROR_INVALID_VALUE;
}

// Allocates memory for an interface structure and returns it
ciscoint_t* ciscoCreateInterface(ciscoconst_t type, uint8_t port1, uint8_t port2, plgc_t* gc){
	ciscoint_t* returnInt = plGCAlloc(gc, sizeof(ciscoint_t));

	returnInt->type = type;
	returnInt->mode = CISCO_MODE_ACCESS;
	returnInt->ports[0] = port1;
	returnInt->ports[1] = port2;
	returnInt->allowedVlans = plGCAlloc(gc, sizeof(plarray_t));
	returnInt->allowedVlans->size = 0;
	returnInt->allowedVlans->array = plGCAlloc(gc, 2 * sizeof(uint16_t));
	returnInt->subMask = 24;
	for(int i = 0; i < 46; i++){
		returnInt->ipAddr[i] = 0;
		returnInt->gateway[i] = 0;
	}

	for(int i = 0; i < 4096; i++)
		returnInt->description[i] = 0;

	return returnInt;
}

// Allocates memory for a table structure and returns it
ciscotable_t* ciscoCreateTable(ciscoconst_t type, ciscoconst_t mode, uint16_t number, plgc_t* gc){
	ciscotable_t* returnTable = plGCAlloc(gc, sizeof(ciscotable_t));

	returnTable->type = type;
	returnTable->mode = mode;
	for(int i = 0; i < 128; i++)
		returnTable->name[i] = 0;
	returnTable->number = number;
	returnTable->interfaces = plGCAlloc(gc, sizeof(plarray_t));
	returnTable->interfaces->size = 0;
	returnTable->interfaces->array = plGCAlloc(gc, 2 * sizeof(ciscoint_t*));

	return returnTable;
}

// Modifies attributes in an interface
ciscoconst_t ciscoModifyInterface(ciscoint_t* interface, plgc_t* gc, ciscoconst_t modType, ...){
	va_list values;
	va_start(values, modType);
	ciscoconst_t constant;
	char* string;
	uint16_t numbers[2];

	// Type parser
	switch(modType){
		case CISCO_MODTYPE_TYPE: ;
		case CISCO_MODTYPE_MODE: ;
			constant = va_arg(values, int);
			break;
		case CISCO_MODTYPE_DESC: ;
		case CISCO_MODTYPE_IP_ADDR: ;
		case CISCO_MODTYPE_GATEWAY: ;
			string = va_arg(values, char*);
			break;
		case CISCO_MODTYPE_PORTS: ;
			numbers[0] = va_arg(values, int);
		case CISCO_MODTYPE_ALLOW_VLAN: ;
		case CISCO_MODTYPE_SUBMASK: ;
			numbers[1] = va_arg(values, int);
			break;
		default:
			return CISCO_ERROR_INVALID_ACTION;
	}

	// Action Parser
	switch(modType){
		case CISCO_MODTYPE_TYPE: ;
			if(constant > CISCO_INT_VLAN)
				return CISCO_ERROR_INVALID_VALUE;

			interface->type = constant;
			break;
		case CISCO_MODTYPE_MODE: ;
			if(interface->mode == CISCO_MODE_IN_PORTCH){
				return CISCO_ERROR_INVALID_ACTION;
			}else if(constant < CISCO_MODE_ACCESS || constant > CISCO_MODE_IN_PORTCH){
				return CISCO_ERROR_INVALID_VALUE;
			}

			interface->mode = constant;
			break;
		case CISCO_MODTYPE_PORTS: ;
			interface->ports[0] = numbers[0];
			interface->ports[1] = numbers[1];
			break;
		case CISCO_MODTYPE_DESC: ;
			if(strlen(string)+1 > 4096)
				return CISCO_ERROR_BUFFER_OVERFLOW;

			strcpy(interface->description, string);
			break;
		case CISCO_MODTYPE_ALLOW_VLAN: ;
			if(numbers[1] > 4096)
				return CISCO_ERROR_OUT_OF_RANGE;

			if(interface->allowedVlans->size > 2){
				void* tempVar = plGCRealloc(gc, interface->allowedVlans->array, (interface->allowedVlans->size + 1) * sizeof(uint16_t));

				if(!tempVar)
					return CISCO_ERROR_PL32LIB_GC;

				interface->allowedVlans->array = tempVar;
			}

			uint16_t* array = interface->allowedVlans->array;

			array[interface->allowedVlans->size] = numbers[1];
			interface->allowedVlans->size++;
			break;
		case CISCO_MODTYPE_IP_ADDR: ;
		case CISCO_MODTYPE_GATEWAY: ;
			char* testString;
			char* writeString;

			if(modType == CISCO_MODTYPE_GATEWAY){
				testString = interface->ipAddr;
				writeString = interface->gateway;
			}else{
				testString = interface->gateway;
				writeString = interface->ipAddr;
			}

			char* isIPv6[2] = { strchr(string, ':'), strchr(testString, ':') };
			size_t strSize = strlen(string);

			if(strcmp(testString, "") != 0 && ((isIPv6[0] && !isIPv6[1]) || (isIPv6[1] && !isIPv6[0])))
				return CISCO_ERROR_MISMATCHED_IPVER;

			if((!isIPv6[0] && strSize > 15) || (isIPv6[0] && strSize > 45))
				return CISCO_ERROR_BUFFER_OVERFLOW;

			memcpy(writeString, string, strSize);
			break;
		case CISCO_MODTYPE_SUBMASK: ;
			if(numbers[1] > 32)
				return CISCO_ERROR_OUT_OF_RANGE;

			interface->subMask = numbers[1];
			break;
	}

	return 0;
}

// Currently not implemented
ciscoconst_t ciscoModifyTable(ciscotable_t* table, plgc_t* gc, ciscoconst_t modType, ...){
	return CISCO_ERROR_INVALID_ACTION;
}

// Adds an interface to a table
int ciscoAddInterface(ciscotable_t* table, ciscoint_t* interface, plgc_t* gc){
	if(table->interfaces->size > 1){
		void* tempPtr = plGCRealloc(gc, table->interfaces->array, (table->interfaces->size + 1) * sizeof(ciscoint_t*));

		if(!tempPtr){
			return CISCO_ERROR_PL32LIB_GC;
		}

		table->interfaces->array = tempPtr;
	}

	ciscoint_t** array = table->interfaces->array;
	array[table->interfaces->size] = interface;

	if(table->type == CISCO_INT_PORTCH)
		interface->mode = CISCO_MODE_IN_PORTCH;

	table->interfaces->size++;

	return 0;
}

// Gets a pointer to an interface from a table
ciscoint_t* ciscoGetInterface(ciscotable_t* table, int index){
	if(index < 0 || index > table->interfaces->size-1){
		return NULL;
	}

	return ((ciscoint_t**)table->interfaces->array)[index];
}

plfile_t* ciscoParseInterface(ciscoint_t* interface, plgc_t* gc){
	plfile_t* returnBuffer = plFOpen(NULL, "w+", gc);
	char* pointerString = ciscoGenerateIntString(interface->type, gc);
	char cmdline[8192] = "";

	if(interface->ports[0] == interface->ports[1]){
		sprintf(cmdline, "int %s/%d\0", pointerString, interface->ports[0]);
	}else{
		sprintf(cmdline, "int range %s/%d-%d\0", pointerString, interface->ports[0], interface->ports[1]);
	}

	plGCFree(gc, pointerString);
	plFPuts(cmdline, returnBuffer);

	pointerString = ciscoGenerateModeString(interface->mode, gc);
	sprintf(cmdline, "switchport mode %s\n\0", pointerString);
	plGCFree(gc, pointerString);
	plFPuts(cmdline, returnBuffer);

	if(strcmp(interface->ipAddr, "") != 0){
		char* isIpAddrV6 = strchr(interface->ipAddr, ':');

		if(!isIpAddrV6){
			plarray_t* octetSubmask = ciscoCidrToOctet(interface->subMask, gc);
			uint8_t* array = octetSubmask->array;
			sprintf(cmdline, "ip address %s %d.%d.%d.%d\n\0", interface->ipAddr, array[0], array[1], array[2], array[3]);
			plShellFreeArray(octetSubmask, false, gc);
		}else{
			sprintf(cmdline, "ipv6 address %s/%d\n\0", interface->ipAddr, interface->subMask);
		}

		plFPuts(cmdline, returnBuffer);
	}

	if(strcmp(interface->description, "") != 0){
		sprintf(cmdline, "description %s\n\0", interface->description);
		plFPuts(cmdline, returnBuffer);
	}

	interface->preExitBufmark = plFTell(returnBuffer);

	if(strcmp(interface->gateway, "") != 0){
		char* isIpAddrV6 = strchr(interface->ipAddr, ':');

		if(!isIpAddrV6)
			sprintf(cmdline, "ip default-gateway %s\n\0", interface->gateway);

		plFPuts(cmdline, returnBuffer);
	}else{
		plFPuts("exit\n", returnBuffer);
	}

	return returnBuffer;
}

plfile_t* ciscoParseTable(ciscotable_t* table, plgc_t* gc){
	plfile_t* returnBuffer = plFOpen(NULL, "w+", gc);
	ciscoint_t** array = table->interfaces->array;
	char placeholder[9] = "";
	char cmdline[192] = "";

	switch(table->type){
		case CISCO_INT_VLAN: ;
			sprintf(cmdline, "vlan %d\n\0", table->number);
			plFPuts(cmdline, returnBuffer);

			if(strcmp(table->name, "") != 0){
				sprintf(cmdline, "name %s\n\0", table->name);
				plFPuts(cmdline, returnBuffer);
			}
			plFPuts("exit\n", returnBuffer);

			for(int i = 0; i < table->interfaces->size; i++){
				plfile_t* tempFile = ciscoParseInterface(array[i], gc);

				switch(array[i]->mode){
					case CISCO_MODE_TRUNK: ;
						strcpy(placeholder, "trunk");
						break;
					default: ;
						strcpy(placeholder, "access");
						break;
				}
				sprintf(cmdline, "switchport %s vlan %d\n\0", placeholder, table->number);

				plFSeek(tempFile, plFTell(tempFile), SEEK_SET);
				plFPuts(cmdline, tempFile);
				plFCat(returnBuffer, tempFile, SEEK_END, SEEK_SET, true);
			}
			break;
		case CISCO_INT_PORTCH: ;
			char* tempString = ciscoGenerateModeString(table->mode, gc);
			sprintf(cmdline, "channel-group %d mode %s\n\0", table->number, tempString);
			plGCFree(gc, tempString);

			for(int i = 0; i < table->interfaces->size; i++){
				plfile_t* tempFile = ciscoParseInterface(array[i], gc);

				plFSeek(tempFile, plFTell(tempFile), SEEK_SET);
				plFPuts(cmdline, tempFile);
				plFCat(returnBuffer, tempFile, SEEK_END, SEEK_SET, true);
			}
			break;
	}

	return returnBuffer;
}

void ciscoPrintInterface(ciscoint_t* interface, plgc_t* gc){
	char* pointerString = ciscoGenerateIntString(interface->type, gc);

	if(interface->ports[0] == interface->ports[1]){
		printf("Interface %s/%d\n\n", pointerString, interface->ports[0]);
	}else{
		printf("Interface range %s/%d-%d\n\n", pointerString, interface->ports[0], interface->ports[1]);
	}

	plGCFree(gc, pointerString);
	pointerString = ciscoGenerateModeString(interface->mode, gc);

	printf("	Mode: %s\n", pointerString);
	plGCFree(gc, pointerString);

	if(strcmp(interface->ipAddr, "") != 0){
		printf("	IP Address: %s\n", interface->ipAddr);
		printf("	Subnet Mask (CIDR): %d\n", interface->subMask);
		if(strcmp(interface->gateway, "") != 0)
			printf("	Default Gateway: %s\n", interface->gateway);
	}
}

void ciscoPrintTable(ciscotable_t* table, plgc_t* gc){
	switch(table->type){
		case CISCO_INT_VLAN: ;
			printf("Vlan %d\n\n", table->number);
			break;
		case CISCO_INT_PORTCH: ;
			char* tempString = ciscoGenerateModeString(table->mode, gc);

			printf("EtherChannel %d\n\n", table->number);
			printf("	Mode: %s\n", tempString);
			plGCFree(gc, tempString);
			break;
	}

	printf("	Number of Interfaces: %d\n", table->interfaces->size);
}
