#pragma once

extern FILE *yyin;
namespace wrd { namespace swrd {
    class obj;
}}
extern wrd::swrd::obj* root;
int yyparse();
typedef struct yy_buffer_state* YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char* str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
