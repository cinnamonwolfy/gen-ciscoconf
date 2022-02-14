/************************************\
* Cisco Config Generator, v0.56      *
* (c)2021 pocketlinux32, Under GPLv3 *
* Types Header File                  *
\************************************/
#include <pl32/pl32-memory.h>
#include <stdarg.h>

typedef struct ciscoint ciscoint_t;
typedef struct ciscotable ciscotable_t;
typedef int ciscoconst_t;

ciscoint_t* ciscoCreateInterface(ciscoconst_t inttype, int port1, int port2);
ciscotable_t* ciscoCreateTable(ciscoconst_t inttype);

void ciscoModifyInterface(ciscoint_t* interface, int modType, ...);
int ciscoAddInterface(ciscotable_t* table, ciscoint_t* interface);

char* ciscoParseInterface(ciscoint_t* interface);
char* ciscoParseTable(ciscotable_t* table);

void ciscoPrintInterface(ciscoint_t* interface);
void ciscoPrintTable(ciscotable_t* table);
