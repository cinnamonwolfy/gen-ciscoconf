/************************************\
* Cisco Config Generator, v0.56      *
* (c)2021 pocketlinux32, Under GPLv3 *
* Types Header File                  *
\************************************/
#include <pl32-memory.h>

typedef struct ciscoint ciscoint_t;
typedef struct ciscotable ciscotable_t;
typedef int ciscoconst_t;

ciscoint_t* ciscoCreateInterface(ciscoconst_t inttype);
ciscotable_t* ciscoCreateTable(ciscoconst_t inttype);

char* ciscoParseInterface(ciscoint_t);
char* ciscoParseTable(ciscotable_t* table);

void ciscoPrintInterface(ciscoint_t* interface);
void ciscoPrintTable(ciscotable_t* table);
