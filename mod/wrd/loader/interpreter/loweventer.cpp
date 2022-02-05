#include "loweventer.hpp"
#include "bison/lowparser.hpp"
#include "../../ast/subpack.hpp"

namespace wrd {

    using std::string;
    WRD_DEF_ME(loweventer)

    namespace {
        string merge(const narr& dotname) {
            string ret;
            for(auto& e : dotname)
                ret += e.cast<std::string>();
            return ret;
        }
    }

    wint me::_onScan(YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner) {
        int tok = _mode->onScan(*this, val, loc, scanner);
        if (_isIgnoreWhitespace && tok == NEWLINE) return SCAN_AGAIN;
        _isIgnoreWhitespace = false;

        switch(tok) {
            case SCAN_MODE_NORMAL: setScan<normalScan>(); return SCAN_AGAIN;
            case SCAN_MODE_INDENT: setScan<indentScan>(); return SCAN_AGAIN;
            case SCAN_MODE_INDENT_IGNORE: _isIgnoreWhitespace = true; return SCAN_AGAIN;
            case SCAN_MODE_END: tok = 0; // == yyterminate();
        }

        return tok;
    }

    wint me::onScan(loweventer& ev, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner, wbool& isBypass) {
        int tok;
        do
            // why do you put redundant _onScan() func?:
            //  because of definately, clang++ bug. when I use continue at switch statement inside of
            //  do-while loop here, it doesn't work like usual 'continue' keyword does, but it does like
            //  'break'.
            tok = _onScan(val, loc, scanner);
        while(tok == SCAN_AGAIN);

        return tok;
    }

    wint me::onEndOfFile(const area& loc) {
        WRD_DI("tokenEvent: onEndOfFile() indents.size()=%d", _indents.size());
        if(_indents.size() <= 1)
            _dispatcher.add(SCAN_MODE_END);
        else
            _dispatcher.addFront(onDedent(_indents.front(), SCAN_MODE_END, loc));

        WRD_DI("tokenEvent: onEndOfFile: finalize by adding 'NEWLINE', then dispatch end-of-file.");
        return NEWLINE;
    }

    wint me::onIndent(wcnt col, wint tok) {
        WRD_DI("tokenEvent: onIndent(col: %d, tok: %d) indents.size()=%d", col, tok, _indents.size());
        _indents.push_back(col);
        _dispatcher.add(tok);
        return INDENT;
    }

    wint me::onDedent(wcnt col, wint tok, const area& loc) {
        WRD_DI("tokenEvent: onDedent(col: %d, tok: %d) indents.size()=%d", col, tok, _indents.size());

        _indents.pop_back();
        wint now = _indents.back();
        if(now < col)
            onErr(new srcErr(err::WARN, 10, loc, col, now, now));

        while(_indents.back() > col) {
            WRD_DI("tokenEvent: onDedent: indentlv become %d -> %d", _indents.back(), _indents[_indents.size()-2]);
            _indents.pop_back();
            _dispatcher.add(DEDENT);
        }

        _dispatcher.add(tok);
        return DEDENT;
    }

    void me::onNewLine() {
        WRD_DI("tokenEvent: onNewLine: _isIgnoreWhitespace=%s, _indents.size()=%d",
            _isIgnoreWhitespace ? "true" : "false", _indents.size());
        if(!_isIgnoreWhitespace && _indents.size() >= 1)
            _dispatcher.add(SCAN_MODE_INDENT);
    }

    wchar me::onScanUnexpected(const area& src, const wchar* token) {
        onErr(new srcErr(err::ERR, 9, src, token));
        return token[0];
    }

    wint me::onIgnoreIndent(wint tok) {
        WRD_DI("tokenEvent: onIgnoreIndent(%d)", tok);
        _dispatcher.add(SCAN_MODE_INDENT_IGNORE);
        return tok;
    }

    void me::onErr(const err* new1) {
        new1->log();
        _report->add(new1);
    }

    void me::onEndParse(const point& pt) {
        WRD_DI("tokenEvent: onEndParse(%d,%d)", pt.row, pt.col);
    }

    str me::onPack(const area& src, const narr& dotname) {
        std::string firstName = dotname[0].cast<std::string>();
        WRD_DI("tokenEvent: onPack(%s)", merge(dotname).c_str());

        pack& pak = *getPack();
        if(nul(pak)) {
            onErr(new srcErr(err::ERR, 13, src, dotname[0].cast<std::string>().c_str()));
            return _onFindSubPack(*(new pack(manifest(), packLoadings())));
        }
        if(dotname.len() == 1)
            return _onFindSubPack(pak);

        // pack syntax rule #1:
        //     middle name automatically created if not exist.
        //     on interpreting 'mypack' pack, user may uses 'pack' keyword with dotted-name.
        //     for instance,
        //         'pack mypack.component.ui'
        //     in this scenario, mypack instance should be created before. and component sub
        //     pack object can be created in this parsing keyword.
        node* e = &pak;
        for(int n=1; n < dotname.len()-1; n++) {
            const std::string& name = dotname[1].cast<std::string>();
            node* sub = &e->sub(name);
            if(nul(sub))
                e->subs().add(sub = new subpack(name));
            e = sub;
        }

        // pack syntax rule #2:
        //     however, if subpack has already existed as a subpack in 'component' instance,
        //     it's definately an error. because I don't allow for user to redefine or extend
        //     pack interpreted.
        const string& lastName = dotname.last()->cast<std::string>();
        node* ret = &e->sub(lastName);
        if(ret) {
            onErr(new srcErr(err::ERR, 14, src, lastName.c_str(), merge(dotname).c_str()));
            return ret;
        }
        e->subs().add(ret = new subpack(lastName));
        return _onFindSubPack(*ret);
    }

    str me::onPackWithout() {
        WRD_DI("tokenEvent: onPackWithout()");

        onErr(new err(err::WARN, 14));
        return str(new pack(manifest(), packLoadings())); // this is a default pack containing name as '{default}'.
    }
}
