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

typedef struct ciscoint ciscoint_t;
typedef struct ciscotable ciscotable_t;
typedef uint8_t ciscoconst_t;

ciscoint_t* ciscoCreateInterface(ciscoconst_t type, uint8_t port1, uint8_t port2, plgc_t* gc);
ciscotable_t* ciscoCreateTable(ciscoconst_t type, ciscoconst_t mode, uint16_t number, plgc_t* gc);

uint8_t ciscoModifyInterface(ciscoint_t* interface, plgc_t* gc, ciscoconst_t modType, ...);
uint8_t ciscoModifyTable(ciscotable_t* table, plgc_t* gc, ciscoconst_t modType, ...);

int ciscoAddInterface(ciscotable_t* table, ciscoint_t* interface, plgc_t* gc);
ciscoint_t* ciscoGetInterface(ciscotable_t* table, int index);

plfile_t* ciscoParseInterface(ciscoint_t* interface, plgc_t* gc);
plfile_t* ciscoParseTable(ciscotable_t* table, plgc_t* gc);

void ciscoPrintInterface(ciscoint_t* interface, plgc_t* gc);
void ciscoPrintTable(ciscotable_t* table, plgc_t* gc);
