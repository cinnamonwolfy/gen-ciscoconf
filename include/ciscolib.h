/************************************\
* Cisco Config Generator, v0.56      *
* (c)2021 pocketlinux32, Under GPLv3 *
* Ciscolib Header File               *
\************************************/
#include <pl32-memory.h>
#include <pl32-shell.h>
#include <pl32-file.h>
#include <stdint.h>
#include <stdarg.h>
#include <time.h>

typedef struct ciscomisc {
	char* line_passwd;
	char* enable_passwd;
	char* enable_secret;
	char* hostname
} ciscomisc_t;
typedef struct ciscoint ciscoint_t;
typedef struct ciscotable ciscotable_t;
typedef uint8_t ciscoconst_t;

plarray_t* ciscoCidrToOctet(uint8_t cidrMask, plgc_t* gc);
char* ciscoGenerateIntString(ciscoconst_t type, plgc_t* gc);
ciscoconst_t ciscoStringToIntType(char* string);
char* ciscoGenerateModeString(ciscoconst_t mode, plgc_t* gc);
ciscoconst_t ciscoStringToMode(char* string);

ciscoint_t* ciscoCreateInterface(ciscoconst_t type, uint8_t port1, uint8_t port2, plgc_t* gc);
ciscotable_t* ciscoCreateTable(ciscoconst_t type, ciscoconst_t mode, uint16_t number, plgc_t* gc);

ciscoconst_t ciscoModifyInterface(ciscoint_t* interface, plgc_t* gc, ciscoconst_t modType, ...);
ciscoconst_t ciscoModifyTable(ciscotable_t* table, plgc_t* gc, ciscoconst_t modType, ...);

int ciscoAddInterface(ciscotable_t* table, ciscoint_t* interface, plgc_t* gc);
ciscoint_t* ciscoGetInterface(ciscotable_t* table, int index);

plfile_t* ciscoParseInterface(ciscoint_t* interface, plgc_t* gc);
plfile_t* ciscoParseTable(ciscotable_t* table, plgc_t* gc);

void ciscoPrintInterface(ciscoint_t* interface, plgc_t* gc);
void ciscoPrintTable(ciscotable_t* table, plgc_t* gc);
