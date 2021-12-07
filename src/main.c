/************************************\
* gen-ciscoconf, v0.55               *
* (c)2021 pocketlinux32, Under GPLv3 *
* Source file                        *
\************************************/
#ifndef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#else
#include <cstdio>
#include <cstdlib>
#include <ctime>
#endif

// Cisco interface struct
typedef struct ciscoint {
	char* ports;			// String containing the ports/interfaces
	char* mode;			// Mode of the interface
	char* allowed_vlans;		// Specifies allowed vlans over a trunk. Only used if mode == "trunk"
	char* ip_addr;			/* Specifies IP address of the interface (VLANs for switches, everything else for routers).
					IP version automatically determined */
	char* sub_mask;			// Subnet mask for IP address. CIDR preferred, but not required
} ciscoint_t;

// Cisco VLAN table struct
typedef struct ciscovlantable {
	char** ids;			// ID array
	char** names;			// Name array
	ciscoint_t* interfaces;		// Interface array
	size_t size;			// Size of Vlan Database
} ciscovlantable_t;

// Cisco device config struct
typedef struct ciscoconf {
	char* filename;			// Filename of output file. Default is NULL
	char* hostname;			// Hostname of device. Default is 'default'
	char* enable_password;		// Enable password
	char* enable_secret;		// Enable secret
	char* line_password;		// Console line password
	ciscovlantable_t vlan;		// Vlan database
	ciscovlantable_t etherchannel;	// EtherChannel database
} ciscoconf_t;

// Create a ciscovlantabe_t with default values
ciscovlantable_t createVlanTableStruct(){
	ciscovlantable_t returnStruct;
	returnStruct.ids = NULL;
	returnStruct.names = NULL;
	returnStruct.interfaces = NULL;
	returnStruct.size = 0;

	return returnStruct;
}

// Create a ciscoconf_t with default values
ciscoconf_t createConfigStruct(){
	ciscoconf_t returnStruct;

	returnStruct.filename = NULL;
	returnStruct.hostname = "default";
	returnStruct.enable_password = NULL;
	returnStruct.enable_secret = NULL;
	returnStruct.line_password = NULL;

	return returnStruct;
}

// Print the contents of a ciscoconf_t struct
void printConfigStruct(ciscoconf_t* structptr){
	printf("Output File: %s\n", structptr->filename);
	printf("Hostname: %s\n", structptr->hostname);
	printf("Enable Passsword: %s\n", structptr->enable_password);
	printf("Enable Secret: %s\n", structptr->enable_secret);
	printf("Line Password: %s\n", structptr->line_password);
}

// Print the contents of a ciscovlantable_t struct
void printVlanTableStruct(ciscovlantable_t* structptr){
	printf("Amount of Vlans: %ld\n", structptr->size);

	for(int i = 0; i < structptr->size; i++){
		printf("Vlan %s:\n", structptr->ids[i]);
		printf("	Name: %s\n", structptr->names[i]);
		printf("	Port(s): %s\n", structptr->interfaces[i].ports);
		printf("	Mode: %s\n", structptr->interfaces[i].mode);
	}
}

ciscoconf_t config;

// Parses a tokenized source line
int configParser(char* args[]){
	if(args[0] == NULL || strchr(args[0], '#') != NULL){
		return 0;
	}

	if(strcmp(args[0], "hostname") == 0){
		config.hostname = args[1];
	}else if(strcmp(args[0], "enable_passwd") == 0){
		config.enable_password = args[1];
	}else if(strcmp(args[0], "enable_secret") == 0){
		config.enable_secret = args[1];
	}else if(strcmp(args[0], "line_passwd") == 0){
		config.line_password = args[1];
	}else if(strcmp(args[0], "vlan") == 0 || strcmp(args[0], "ether") == 0){
		if(args[1] == NULL){
			return 1;
		}

		ciscovlantable_t* reference;

		if(strcmp(args[0], "vlan") == 0){
			reference = &config.vlan;
		}else{
			reference = &config.etherchannel;
		}

		reference->size++;

		if(reference->size < 2){
			reference->ids = malloc(2 * sizeof(char*));
			reference->names = malloc(2 * sizeof(char*));
			reference->interfaces = malloc(2 * sizeof(ciscoint_t));
		}else{
			size_t reallocSize[2] = { reference->size * sizeof(char*), reference->size * sizeof(ciscoint_t) };
			void* tempPtr[3] = { realloc(reference->ids, reallocSize[0]), realloc(reference->names, reallocSize[0]), realloc(reference->interfaces, reallocSize[1]) };

			for(int i = 0; i < 4; i++){
				if(!tempPtr[i]){
					printf("Call to realloc() failed. Aborting...\n");
					exit(1);
				}
			}

			reference->ids = tempPtr[0];
			reference->names = tempPtr[1];
			reference->interfaces = tempPtr[2];
		}

		reference->ids[reference->size - 1] = args[1];
		if(strcmp(args[0], "vlan") == 0){
			reference->names[reference->size - 1] = args[2];
			reference->interfaces[reference->size - 1].ports = args[3];
			reference->interfaces[reference->size - 1].mode = args[4];
		}else{
			reference->names[reference->size - 1] = args[4];
			reference->interfaces[reference->size - 1].ports = args[2];
			reference->interfaces[reference->size - 1].mode = args[3];
		}
	}else{
		printf("Unknown command\n");
		return 1;
	}
}

// Interactive shell that gets ran every single time a source file isn't given
void interactiveShell(){
	bool run = true;
	char input[256] = " ";
	size_t size = 256;

	while(run){
		printf("ptsh>");
		fgets(input, size, stdin);
		printf("input: %s", input);

		char* args[64];
		int i = 1;

		args[0] = strtok(input, " ");

		while((args[i] = strtok(NULL, " ")) != NULL && i < 64){
			i++;
		}

	}
}

int main(int argc, const char* argv[]){
	FILE* containerFileStream;
	FILE* sourceFileStream;
	char* containerFilename = NULL;
	bool parseOnly = false, verbose = false, snippet = false;
	char* month[] = { "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec" };

	config = createConfigStruct();
	config.vlan = createVlanTableStruct();
	config.etherchannel = createVlanTableStruct();

	if(argc > 1){
		for(int i = 1; i < argc; i++){
			if(strcmp(argv[i], "-o") == 0){
				config.filename = argv[i + 1];
				i++;
			}else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
				printf("Cisco Config Generator, Version 0.55\n");
				printf("(c)2021 pocketlinux32, Under GPLv3\n\n");
				printf("Usage: %s [ --help | -o OUTPUT_FILE | -p | -v | -s ] SOURCE_FILE \n\n", argv[0]);
				printf("-h		Shows this help\n");
				printf("-o		Outputs generated config to OUTPUT_FILE\n");
				printf("-v		Shows parsed config\n");
				printf("-p		Only parses the source file/commands, doesn't output config file. Requires -v\n");
				printf("-s		Doesn't generate 'enable, config t' at the beginning of file\n\n");
				return 0;
			}else if(strcmp(argv[i], "-v") == 0){
				verbose = true;
			}else if(strcmp(argv[i], "-p") == 0){
				parseOnly = true;
			}else if(strcmp(argv[i], "-s") == 0){
				snippet = true;
			}else if(containerFilename == NULL){
				containerFilename = argv[i];
			}
		}

		sourceFileStream = fopen(containerFilename, "r");
	}else{
		sourceFileStream = NULL;
	}

	if(!sourceFileStream){
		printf("Initializing interactive shell\n");
		interactiveShell();
	}else{
		printf("Source file: %s\n", containerFilename);
		char readLine[256];
		size_t size = 256;

		while(fgets(readLine, size, sourceFileStream) != NULL){
			char* argArr[5];
			char* workPtr = strtok(readLine, " \n");

			for(int i = 0; i < 5; i++){
				if(workPtr != NULL){
					argArr[i] =  malloc((strlen(workPtr) + 1) * sizeof(char));
					strcpy(argArr[i], workPtr);
				}else{
					argArr[i] = malloc(2 * sizeof(char));
					strcpy(argArr[i], "#");
				}

				workPtr = strtok(NULL, " \n");
			}


			configParser(argArr);
		}

		fclose(sourceFileStream);
	}

	if(verbose){
		printConfigStruct(&config);
		printVlanTableStruct(&config.vlan);
		printVlanTableStruct(&config.etherchannel);

		if(parseOnly){
			exit(0);
		}
	}

	printf("Generating config file...");

	if(!config.filename){
		printf("Aborted.\n\n");
		printf("This feature is not supported yet. Please try again in the next version\n");
		exit(0);
	}else{
		containerFileStream = fopen(config.filename, "w+");
	}

	time_t timePtr = time(NULL);
	struct tm timeStruct = *localtime(&timePtr);

	if(!snippet)
		fprintf(containerFileStream, "enable\nclock set %d:%d:%d %d %s %d\nconfig t", timeStruct.tm_hour, timeStruct.tm_min, timeStruct.tm_sec, timeStruct.tm_mday, month[timeStruct.tm_mon], timeStruct.tm_year + 1900);

	if(config.hostname)
		fprintf(containerFileStream, "hostname %s\n", config.hostname);

	if(config.enable_password || config.enable_secret || config.line_password){
		fprintf(containerFileStream, "service password-encryption\n");

		if(config.enable_password)
			fprintf(containerFileStream, "enable password %s\n", config.enable_password);

		if(config.enable_secret)
			fprintf(containerFileStream, "enable secret %s\n", config.enable_secret);

		if(config.line_password)
			fprintf(containerFileStream, "line con 0\npassword %s\nlogin\nline vty 0 15\npassword %s\nlogin\nexit\n", config.line_password, config.line_password);
	}

	if(config.vlan.size > 0){
		for(int i = 0; i < config.vlan.size; i++){
			fprintf(containerFileStream, "vlan %s\n", config.vlan.ids[i]);

			if(strcmp(config.vlan.names[i],"#") != 0 && strcmp(config.vlan.names[i], "noname") != 0)
				fprintf(containerFileStream, "name %s\n", config.vlan.names[i]);

			fprintf(containerFileStream, "exit\n");

			if(strcmp(config.vlan.interfaces[i].ports,"#") != 0 && strcmp(config.vlan.interfaces[i].ports, "noport") != 0){
				char* tempMode = "access";

				if(strcmp(config.vlan.interfaces[i].mode,"#") != 0 && strcmp(config.vlan.interfaces[i].mode, "nomode") != 0)
					tempMode = config.vlan.interfaces[i].mode;

				fprintf(containerFileStream, "int range %s\nswitchport mode %s\nswitchport %s vlan %s\nexit\n", config.vlan.interfaces[i].ports, tempMode, tempMode, config.vlan.ids[i]);
			}

		}
	}

	if(config.etherchannel.size > 0){
		for(int i = 0; i < config.etherchannel.size; i++){
			if(strcmp(config.etherchannel.interfaces[i].ports, "#") != 0 && strcmp(config.etherchannel.interfaces[i].ports, "noport") != 0){
				char* tempMode = "on";

				if(strcmp(config.etherchannel.interfaces[i].mode, "#") != 0 || strcmp(config.etherchannel.interfaces[i].mode, "nomode") != 0){
					tempMode = config.etherchannel.interfaces[i].mode;
				}

				fprintf(containerFileStream, "int range %s\nchannel-group %s mode %s\n", config.etherchannel.interfaces[i].ports, config.etherchannel.ids[i], tempMode);
			}
		}
	}

	fprintf(containerFileStream, "exit\n");

	fclose(containerFileStream);

	printf("Done.\n\n");
	if(config.filename != NULL){
		printf("Output File: %s\n", config.filename);
	}

	return 0;
}
