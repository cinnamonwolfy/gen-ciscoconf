/************************************\
* Cisco Config Generator, v0.56      *
* (c)2021 pocketlinux32, Under GPLv3 *
* Ciscolib Header File               *
\************************************/
#include <pl32-memory.h>
#include <pl32-file.h>
#include <stdint.h>
#include <stdarg.h>
#include <time.h>

typedef struct ciscoint ciscoint_t;
typedef struct ciscotable ciscotable_t;
typedef uint8_t ciscoconst_t;

ciscoint_t* ciscoCreateInterface(ciscoconst_t type, int port1, int port2, plgc_t* gc);
ciscotable_t* ciscoCreateTable(ciscoconst_t type, uint16_t number, ciscoconst_t tmode, ciscoconst_t imode, plgc_t* gc);

int ciscoModifyInterface(ciscoint_t* interface, plgc_t* gc, ciscoconst_t modType, ...);
int ciscoModifyTable(ciscotable_t* table, ciscoconst_t modType, ...);

int ciscoAddInterface(ciscotable_t* table, ciscoint_t* interface);
ciscoint_t* ciscoGetInterface(ciscotable_t* table, int index);

plfile_t* ciscoParseInterface(ciscoint_t* interface);
plfile_t* ciscoParseTable(ciscotable_t* table);

void ciscoPrintInterface(ciscoint_t* interface);
void ciscoPrintTable(ciscotable_t* table);
