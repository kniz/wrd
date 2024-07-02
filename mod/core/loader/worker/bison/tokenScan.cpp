#include "tokenScan.hpp"
#include "lowscanner.hpp"
#include <iostream>

YY_DECL;

std::string getTokenName(int tok);

namespace nm {

    nint tokenScan::onScan(parser& ps, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner, nbool& isBypass) {
        tokenDispatcher& disp = ps.getDispatcher();
        nint tok;

        if(!(isBypass = disp.pop(tok)))
            tok = yylexOrigin(val, loc, scanner);
        if(tok == ENDOFFILE)
            tok = ps.onTokenEndOfFile();

        std::string tokName = getTokenName(tok);
        NAMU_DI("%s: dispatcher[%d]%s(token: \"%s\" [%d]) at %d,%d", getType().getName().c_str(), disp.len(), isBypass ? ".dispatch" : " lowscanner",
                tokName.c_str(), tok, loc->start.row, loc->start.col);
        return tok;
    }

    nint normalScan::onScan(parser& ps, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner, nbool& isBypass) {
        nint tok = super::onScan(ps, val, loc, scanner, isBypass);
        switch(tok) {
            case TAB:
                return SCAN_AGAIN;
        }

        return tok;
    }

    normalScan* normalScan::_instance = new normalScan();

    nint indentScan::onScan(parser& ps, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner, nbool& isBypass) {
        nint tok = super::onScan(ps, val, loc, scanner, isBypass);
        switch(tok) {
            case NEWLINE:
                if(!isBypass) {
                    NAMU_DI("indentScan: ignore NEWLINE");
                    return SCAN_AGAIN;
                }
                break;
        }

        ps.setScan<normalScan>();
        if(isBypass) return tok;

        ncnt cur = loc->start.col;
        std::vector<ncnt>& ind = ps.getIndents();
        if(ind.size() == 0) {
            NAMU_DI("indentScan: initial indent lv: %d", cur);
            ind.push_back(cur);
        }

        ncnt prev = ind.back();
        NAMU_DI("indentScan: column check: cur[%d] prev[%d]", cur, prev);

        if(cur > prev)
            return ps.onIndent(cur, tok);
        else if(cur < prev)
            return ps.onDedent(cur, tok);

        return tok;
    }

    indentScan* indentScan::_instance = new indentScan();

}
