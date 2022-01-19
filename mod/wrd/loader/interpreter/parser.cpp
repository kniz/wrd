#include "parser.hpp"
#include "bison/lowscanner.hpp"

YY_DECL;

namespace wrd {

    WRD_DEF_ME(parser)

    tstr<narr> me::parse(const wchar* script) {
        yyscan_t scanner;
        yylex_init_extra(&_eventer, &scanner);

        YY_BUFFER_STATE bufState = yy_scan_string((wchar*) script, scanner); // +2 is for space of END_OF_BUFFER, nullptr.
        if(!bufState)
            return WRD_E("bufState == nullptr"), str();

#if YYDEBUG
        yyset_debug(1, scanner); // For Flex (no longer a global, but rather a member of yyguts_t)
        yydebug = 1;             // For Bison (still global, even in a reentrant parser)
#endif

        int res = yyparse(scanner);
        // TODO: handle ret.

        yylex_destroy(scanner);
        rel();
        return _eventer.getTray();
    }

    void me::rel() {
        _eventer.rel();
    }
}
