#include <ciscolib-const.h>
#include <ciscolib.h>

bool verbose = false;
bool parseOnly = false;
bool snippet = false;
bool router = false;
bool isCmdlineOpt = false;
plarray_t* interfaces;
plarray_t* tables;
plfile_t* generatedConfig;
char* outputPath;

int showConfig(plarray_t* args, plgc_t* gc){
	char** argv = args->array;
	if(args->size > 1 && strcmp(argv[1], "gen-conf") == 0){
		plFSeek(generatedConfig, 0, SEEK_SET);
		char text[4096] = "";
		plFGets(text, 4095, generatedConfig);
		printf("%s\n", text);
	}else{
		ciscoint_t** interfaceArr = interfaces->array;
		ciscotable_t** tableArr = tables->array;

		for(int i = 0; i < interfaces->size; i++){
			ciscoPrintInterface(interfaceArr[i], gc);
		}

		for(int i = 0; i < tables->size; i++){
			ciscoPrintTable(tableArr[i], gc);
		}
	}
}

int generateConfig(plarray_t* args, plgc_t* gc){
	ciscoint_t** interfaceArr = interfaces->array;
	ciscotable_t** tableArr = tables->array;

	if(verbose){
		showConfig(NULL, gc);
	}

	if(!parseOnly){
		if(!snippet)
			plFPuts("enable\nconfig t\n", generatedConfig);

		for(int i = 0; i < tables->size; i++){
			ciscoParseTable(tableArr[i], gc);
		}

		for(int i = 0; i < tables->size; i++){
			ciscoParseTable(tableArr[i], gc);
		}
	}else{
		return 0;
	}

	if(outputPath)
		plFPToFile(outputPath, generatedConfig);

}

int ciscoconfSettings(plarray_t* args, plgc_t* gc){
	
}

int configCmdParser(plarray_t* args, plgc_t* gc){
	char** argv = args->array;
	char* junk;
	if(strcmp(argv[0], "int") == 0 || strcmp(argv[0], "interface") == 0){
		if(args->size < 2){
			printf("%s: Not enough arguments\n", argv[0]);
			return CISCO_ERROR_INVALID_ACTION;
		}

		if(interfaces->size > 2){
			void* tempPtr = plGCAlloc(gc, (interfaces->size + 1) * sizeof(ciscoint_t*));
			if(!tempPtr){
				printf("%s: Internal pl32lib error\n", argv[0]);
				return CISCO_ERROR_PL32LIB_GC;
			}
			interfaces->array = tempPtr;
		}

		ciscoint_t** array = interfaces->array;
		size_t index = interfaces->size;
		ciscoconst_t type = ciscoStringToIntType(argv[1]);
		char* intNums = strchr(argv[1], '/');
		uint8_t nums[2];

		if(!intNums){
			printf("%s: Invalid interface\n", argv[0]);
			return CISCO_ERROR_INVALID_VALUE;
		}

		while(strchr(intNums, '/') != NULL)
			intNums++;

		if(strchr(intNums, '-')){
			char* tempChr = strchr(intNums, '-');
			char* tempPtr = plGCCalloc(gc, (tempChr - intNums) + 1, sizeof(char));
			memcpy(tempPtr, intNums, tempChr - intNums);
			nums[1] = strtol(tempChr + 1, &junk, 10);
			intNums = tempPtr;
		}else{
			nums[1] = 0;
		}

		nums[0] = strtol(intNums, &junk, 10);
		plGCFree(gc, intNums);

		array[index] = ciscoCreateInterface(type, nums[0], nums[1], gc);

		if(args->size >= 3){
			int retVar = ciscoModifyInterface(array[index], gc, CISCO_MODTYPE_MODE, ciscoStringToMode(argv[2]));
			if(retVar == CISCO_ERROR_INVALID_VALUE){
				printf("%s: Invalid value\n", argv[0]);
				return retVar;
			}
		}

		if(args->size >= 4){
			int retVar = ciscoModifyInterface(array[index], gc, CISCO_MODTYPE_IP_ADDR, argv[3]);
			if(retVar == CISCO_ERROR_MISMATCHED_IPVER){
				printf("%s: Mismatched IP version\n", argv[0]);
				return retVar;
			}
		}

		if(args->size >= 5){
			uint8_t subMask = strtol(argv[4], &junk, 10);

			int retVar = ciscoModifyInterface(array[index], gc, CISCO_MODTYPE_SUBMASK, subMask);
			if(retVar == CISCO_ERROR_OUT_OF_RANGE){
				printf("%s: Out of range\n", argv[0]);
				return retVar;
			}
		}

		if(args->size >= 6){
			int retVar = ciscoModifyInterface(array[index], gc, CISCO_MODTYPE_DESC, argv[5]);
			if(retVar == CISCO_ERROR_BUFFER_OVERFLOW){
				printf("%s: Buffer overflow", argv[0]);
				return retVar;
			}
		}

		interfaces->size++;
	}else if(strcmp(argv[0], "vlan") == 0){
		if(args->size < 2){
			printf("%s: Not enough args\n", argv[0]);
			return CISCO_ERROR_INVALID_ACTION;
		}

		if(tables->size > 2){
			void* tempPtr = plGCAlloc(gc, (tables->size + 1) * sizeof(ciscoint_t*));
			if(!tempPtr){
				printf("%s: Internal pl32lib error\n", argv[0]);
				return CISCO_ERROR_PL32LIB_GC;
			}
			tables->array = tempPtr;
		}

		uint16_t number = strtol(argv[1], &junk, 10);
		ciscotable_t** array = tables->array;
		size_t index = tables->size;

		array[index] = ciscoCreateTable(CISCO_INT_VLAN, CISCO_MODE_AUTO, number, gc);

		tables->size++;
	}
}

int main(int argc, char* argv[]){
	plgc_t* mainGC = plGCInit(8 * 1024 * 1024);
	interfaces = plGCAlloc(mainGC, sizeof(plarray_t));
	interfaces->array = plGCAlloc(mainGC, 2 * sizeof(ciscoint_t*));
	tables = plGCAlloc(mainGC, sizeof(plarray_t));
	tables->array = plGCAlloc(mainGC, 2 * sizeof(ciscoint_t*));
	generatedConfig = plFOpen(NULL, "w+", mainGC);
	outputPath = NULL;

	for(int i = 0; i < argc; i++){
		if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0){
			printf("Cisco Configurator v0.56\n");
			printf("(c)2022 pocketlinux32, Under GPLv3\n\n");
			printf("Usage: %s { --help | --out OUTPUT | --parse | --verbose | --snippet | --terminal TERM_DEV } [ SOURCE ]\n", argv[0]);
			printf("Generates a configuration script for a Cisco device. If no arguments given, it runs in interactive\n");
			printf("mode and outputs generated configuration to stdout.\n\n");
			printf("-h|--help		Shows this help.\n");
			printf("-o|--out		Writes generated configuration to a file.\n");
			printf("-v|--verbose		Enables verbosity (shows parser debug messages and generated configuration in a readable format).\n");
			printf("-p|--parse		Only parses the configuration (verbosity must be enabled).\n");
			printf("-s|--snippet		Generates the configuration without the header (\"enable\\nconfig t\\n\"). This option\n");
			printf("			ignores any output files and only writes to either stdout or a terminal device.\n");
			printf("-t|--terminal		Writes generated configuration to a terminal device. This is intended for 'flashing' the configuration\n");
			printf("			to a Cisco device connected over serial, but it can be done to any terminal device.\n\n");
			return 0;
		}else if(strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0){
			verbose = true;
		}else if(strcmp(argv[i], "--parse") == 0 || strcmp(argv[i], "-p") == 0){
			parseOnly = true;
		}else if(strcmp(argv[i], "--snippet") == 0 || strcmp(argv[i], "-s") == 0){
			snippet = true;
		}else if(strchr(argv[i], '-') == argv[i]){
			printf("Invalid option: %s\n", argv[i]);
			printf("Try '%s --help' for more information\n", argv[0]);
			return 1;
		}
	}

	plarray_t* commandBuf = plGCAlloc(mainGC, sizeof(plarray_t));
	commandBuf->array = plGCAlloc(mainGC, 6 * sizeof(plfunctionptr_t));
	((plfunctionptr_t*)commandBuf->array)[0].function = configCmdParser;
	((plfunctionptr_t*)commandBuf->array)[0].name = "int";
	((plfunctionptr_t*)commandBuf->array)[1].function = configCmdParser;
	((plfunctionptr_t*)commandBuf->array)[1].name = "vlan";
	((plfunctionptr_t*)commandBuf->array)[2].function = configCmdParser;
	((plfunctionptr_t*)commandBuf->array)[2].name = "ether";
	((plfunctionptr_t*)commandBuf->array)[3].function = configCmdParser;
	((plfunctionptr_t*)commandBuf->array)[3].name = "system";
	((plfunctionptr_t*)commandBuf->array)[4].function = generateConfig;
	((plfunctionptr_t*)commandBuf->array)[4].name = "generate";
	((plfunctionptr_t*)commandBuf->array)[5].function = showConfig;
	((plfunctionptr_t*)commandBuf->array)[5].name = "show";
	commandBuf->size = 6;

	plShellInteractive(NULL, true, commandBuf, mainGC);
	return 0;
}
