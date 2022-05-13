/************************************\
* gen-ciscoconf, v0.56               *
* (c)2022 pocketlinux32, Under GPLv3 *
* Source file                        *
\************************************/
#include <ciscolib-const.h>
#include <ciscolib.h>

bool verbose = false;
bool parseOnly = false;
bool snippet = false;
bool router = false;
bool isTerminal = false;
plarray_t* interfaces;
plarray_t* tables;
plfile_t* generatedConfig;
char* outputPath = NULL;

int showConfig(plarray_t* args, plgc_t* gc){
	char** argv = args->array;
	if(args->size > 1 && strcmp(argv[1], "gen-conf") == 0){
		plFSeek(generatedConfig, 0, SEEK_SET);
		char text[4096] = "";
		while(plFGets(text, 4095, generatedConfig) != NULL)
			printf("%s", text);
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

	plarray_t generatedSnippets;
	generatedSnippets.array = plGCAlloc(gc, 2 * sizeof(plfile_t*));
	generatedSnippets.size = 0;

	if(verbose){
		showConfig(NULL, gc);
	}

	if(!parseOnly){
		if(!snippet)
			plFPuts("enable\nconfig t\n", generatedConfig);

		for(int i = 0; i < tables->size; i++){
			((plfile_t**)generatedSnippets.array)[i] = ciscoParseTable(tableArr[i], gc);
			if(generatedSnippets.size + 1 >= 2){
				void* tempPtr = plGCRealloc(gc, generatedSnippets.array, (generatedSnippets.size + 2) * sizeof(plfile_t*));
				if(!tempPtr){
					printf("generateConfig: Internal pl32lib error\n");
					return CISCO_ERROR_PL32LIB_GC;
				}

				generatedSnippets.array = tempPtr;
			}
			generatedSnippets.size++;
		}

		for(int i = 0; i < interfaces->size; i++){
			((plfile_t**)generatedSnippets.array)[tables->size + i] = ciscoParseInterface(interfaceArr[i], gc);
			if(generatedSnippets.size + 1 >= 2){
				void* tempPtr = plGCRealloc(gc, generatedSnippets.array, generatedSnippets.size + 2);
				if(!tempPtr){
					printf("generateConfig: Internal pl32lib error\n");
					return CISCO_ERROR_PL32LIB_GC;
				}

				generatedSnippets.array = tempPtr;
			}
			generatedSnippets.size++;
		}
	}else{
		return 0;
	}

	for(int i = 0; i < generatedSnippets.size; i++){
		plFCat(generatedConfig, ((plfile_t**)generatedSnippets.array)[i], SEEK_END, SEEK_SET, true);
	}

	plGCFree(gc, generatedSnippets.array);

	if(outputPath)
		plFPToFile(outputPath, generatedConfig);

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

		nums[0] = strtol(intNums, &junk, 10);

		if(strchr(intNums, '-') != NULL){
			char* tempChr = strchr(intNums, '-');
			char* tempPtr = plGCCalloc(gc, (tempChr - intNums) + 1, sizeof(char));
			memcpy(tempPtr, intNums, tempChr - intNums);
			nums[1] = strtol(tempChr + 1, &junk, 10);
			intNums = tempPtr;
		}else{
			nums[1] = nums[0];
		}

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
	}else if(strcmp(argv[0], "vlan") == 0 || strcmp(argv[0], "ether") == 0){
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

		if(strcmp(argv[0], "vlan")){
			array[index] = ciscoCreateTable(CISCO_INT_VLAN, CISCO_MODE_AUTO, number, gc);
		}else{
			array[index] = ciscoCreateTable(CISCO_INT_PORTCH, CISCO_MODE_AUTO, number, gc);
		}

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
	char* sourcePath = NULL;
	plfile_t* sourceFile = NULL;

	if(argc > 1){
		for(int i = 1; i < argc; i++){
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
				printf("			ignores any output files and only writes to either stdout or a terminal device.\n\n");
				printf("-t|--terminal		Writes generated configuration to a terminal device. This is intended for 'flashing' the configuration\n");
				printf("			to a Cisco device connected over serial, but it can be done to any terminal device.\n\n");
				return 0;
			}else if(strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0){
				verbose = true;
			}else if(strcmp(argv[i], "--parse") == 0 || strcmp(argv[i], "-p") == 0){
				parseOnly = true;
			}else if(strcmp(argv[i], "--snippet") == 0 || strcmp(argv[i], "-s") == 0){
				snippet = true;
			}else if(strcmp(argv[i], "--out") == 0 || strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--terminal") == 0 || strcmp(argv[i], "-t") == 0){
				if(i + 1 >= argc){
					printf("%s requires an operand\n", argv[i]);
					printf("Try '%s --help' for more information\n", argv[0]);
					return 1;
				}

				outputPath = argv[i + 1];

				if(strcmp(argv[i], "--terminal") == 0 || strcmp(argv[i], "-t") == 0){
					isTerminal = true;
				}else{
					isTerminal = false;
				}

				i++;
			}else if(strchr(argv[i], '-') == argv[i]){
				printf("Invalid option: %s\n", argv[i]);
				printf("Try '%s --help' for more information\n", argv[0]);
				return 1;
			}else{
				sourcePath = argv[i];
			}
		}
	}

	plarray_t commandBuf;
	plarray_t variableBuf;
	commandBuf.array = plGCAlloc(mainGC, 6 * sizeof(plfunctionptr_t));
	variableBuf.array = plGCAlloc(mainGC, 6 * sizeof(plvariable_t));

	((plfunctionptr_t*)commandBuf.array)[0].function = configCmdParser;
	((plfunctionptr_t*)commandBuf.array)[0].name = "int";
	((plfunctionptr_t*)commandBuf.array)[1].function = configCmdParser;
	((plfunctionptr_t*)commandBuf.array)[1].name = "vlan";
	((plfunctionptr_t*)commandBuf.array)[2].function = configCmdParser;
	((plfunctionptr_t*)commandBuf.array)[2].name = "ether";
	((plfunctionptr_t*)commandBuf.array)[3].function = configCmdParser;
	((plfunctionptr_t*)commandBuf.array)[3].name = "system";
	((plfunctionptr_t*)commandBuf.array)[4].function = generateConfig;
	((plfunctionptr_t*)commandBuf.array)[4].name = "generate";
	((plfunctionptr_t*)commandBuf.array)[5].function = showConfig;
	((plfunctionptr_t*)commandBuf.array)[5].name = "show";
	commandBuf.size = 6;

	((plvariable_t*)variableBuf.array)[0].varptr = &verbose;
	((plvariable_t*)variableBuf.array)[0].type = PLSHVAR_BOOL;
	((plvariable_t*)variableBuf.array)[0].name = "verbose";
	((plvariable_t*)variableBuf.array)[0].isMemAlloc = false;
	((plvariable_t*)variableBuf.array)[1].varptr = &parseOnly;
	((plvariable_t*)variableBuf.array)[1].type = PLSHVAR_BOOL;
	((plvariable_t*)variableBuf.array)[1].name = "parseOnly";
	((plvariable_t*)variableBuf.array)[1].isMemAlloc = false;
	((plvariable_t*)variableBuf.array)[2].varptr = &snippet;
	((plvariable_t*)variableBuf.array)[2].type = PLSHVAR_BOOL;
	((plvariable_t*)variableBuf.array)[2].name = "snippet";
	((plvariable_t*)variableBuf.array)[2].isMemAlloc = false;
	((plvariable_t*)variableBuf.array)[3].varptr = &router;
	((plvariable_t*)variableBuf.array)[3].type = PLSHVAR_BOOL;
	((plvariable_t*)variableBuf.array)[3].name = "router";
	((plvariable_t*)variableBuf.array)[3].isMemAlloc = false;
	((plvariable_t*)variableBuf.array)[4].varptr = &isTerminal;
	((plvariable_t*)variableBuf.array)[4].type = PLSHVAR_BOOL;
	((plvariable_t*)variableBuf.array)[4].name = "isTerminal";
	((plvariable_t*)variableBuf.array)[4].isMemAlloc = false;
	((plvariable_t*)variableBuf.array)[5].varptr = &outputPath;
	((plvariable_t*)variableBuf.array)[5].type = PLSHVAR_STRING;
	((plvariable_t*)variableBuf.array)[5].name = "outputPath";
	((plvariable_t*)variableBuf.array)[5].isMemAlloc = false;
	variableBuf.size = 6;

	if(sourcePath)
		sourceFile = plFOpen(sourcePath, "r", mainGC);

	if(!sourceFile){
		plShellInteractive(NULL, true, &variableBuf, &commandBuf, mainGC);
	}else{
		printf("Source path has been specified. Using source file.\n");
		char cmdline[4096];
		while(plFGets(cmdline, 4096, sourceFile) != NULL){
			plShell(cmdline, &variableBuf, &commandBuf, &mainGC);
		}
	}

	return 0;
}
