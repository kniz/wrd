#include "loweventer.hpp"
#include "bison/lowparser.hpp"
#include "../../ast.hpp"
#include "../../ast/mgd/mgdFunc.hpp"
#include "../../builtin/primitive.hpp"
#include "../../frame/thread.hpp"
#include "../../ast/genericObj.hpp"
#include "../../type/mgdType.hpp"

namespace namu {

    using std::string;
    NAMU_DEF_ME(loweventer)

    namespace {
        string merge(const narr& dotname) {
            string ret;
            for(auto& e : dotname)
                ret += e.cast<std::string>();
            return ret;
        }
    }

    nint me::_onScan(YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner) {
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

    nint me::onScan(loweventer& ev, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner, nbool& isBypass) {
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

    nint me::onEndOfFile() {
        NAMU_DI("tokenEvent: onEndOfFile() indents.size()=%d", _indents.size());
        if(_indents.size() <= 1)
            _dispatcher.add(SCAN_MODE_END);
        else
            _dispatcher.addFront(onDedent(_indents.front(), SCAN_MODE_END));

        NAMU_DI("tokenEvent: onEndOfFile: finalize by adding 'NEWLINE', then dispatch end-of-file.");
        return NEWLINE;
    }

    nint me::onIndent(ncnt col, nint tok) {
        NAMU_DI("tokenEvent: onIndent(col: %d, tok: %d) indents.size()=%d", col, tok, _indents.size());
        _indents.push_back(col);
        _dispatcher.add(tok);
        return INDENT;
    }

    nint me::onDedent(ncnt col, nint tok) {
        NAMU_DI("tokenEvent: onDedent(col: %d, tok: %d) indents.size()=%d", col, tok, _indents.size());

        _indents.pop_back();
        nint now = _indents.back();
        if(now < col)
            onSrcWarn(errCode::WRONG_INDENT_LV, col, now, now);

        while(_indents.back() > col) {
            NAMU_DI("tokenEvent: onDedent: indentlv become %d -> %d", _indents.back(), _indents[_indents.size()-2]);
            _indents.pop_back();
            _dispatcher.add(DEDENT);
        }

        _dispatcher.add(tok);
        return DEDENT;
    }

    void me::onNewLine() {
        NAMU_DI("tokenEvent: onNewLine: _isIgnoreWhitespace=%s, _indents.size()=%d",
            _isIgnoreWhitespace ? "true" : "false", _indents.size());
        if(!_isIgnoreWhitespace && _indents.size() >= 1)
            _dispatcher.add(SCAN_MODE_INDENT);
    }

    nchar me::onScanUnexpected(const nchar* token) {
        onSrcErr(errCode::UNEXPECTED_TOK, token);
        return token[0];
    }

    nint me::onIgnoreIndent(nint tok) {
        NAMU_DI("tokenEvent: onIgnoreIndent(%d)", tok);
        _dispatcher.add(SCAN_MODE_INDENT_IGNORE);
        return tok;
    }

    void me::onEndParse() {
#if NAMU_IS_DBG
        const point& pt = getArea().start;
        NAMU_DI("tokenEvent: onEndParse(%d,%d)", pt.row, pt.col);
#endif
        _srcArea.rel();
    }

    obj* me::onPack(const narr& dotname) {
        NAMU_DI("tokenEvent: onPack(%s)", merge(dotname).c_str());

        // pack syntax rule #1:
        //  if there is no specified name of pack, I create an one.
        std::string firstName = dotname[0].cast<std::string>();
        if(!_slot)
            _slot.bind(new slot(manifest(firstName)));
        obj* e = &_slot->getPack();

        const std::string& realName = _slot->getManifest().name;
        if(realName != firstName)
            return onErr(errCode::PACK_NOT_MATCH, firstName.c_str(), realName.c_str()), e;

        // pack syntax rule #2:
        //  middle name automatically created if not exist.
        //  on interpreting 'mypack' pack, user may uses 'pack' keyword with dotted-name.
        //  for instance,
        //      'pack mypack.component.ui'
        //  in this scenario, mypack instance should be created before. and component sub
        //  pack object can be created in this parsing keyword.
        for(int n=1; n < dotname.len(); n++) {
            const std::string& name = dotname[n].cast<std::string>();
            obj* sub = &e->sub<obj>(name);
            if(nul(sub))
                e->subs().add(name, sub = new obj(new mgdType(name)));
            e = sub;
        }

        return onSubPack(*e);
    }

    obj* me::onSubPack(obj& subpack) {
        NAMU_DI("tokenEvent: onSubPack()");
        _subpack.bind(subpack);
        return &subpack;
    }

    obj* me::onPack() {
        NAMU_DI("tokenEvent: onPack()");

        slot* newSlot = &_slot.get();
        if(nul(newSlot))
            _slot.bind(newSlot = new slot(manifest()));

        const std::string& name = _slot->getManifest().name;
        if(name != manifest::DEFAULT_NAME)
            return onErr(errCode::PACK_NOT_MATCH, manifest::DEFAULT_NAME, name.c_str()), &newSlot->getPack();

        return onSubPack(newSlot->getPack()); // this is a default pack containing name as '{default}'.
    }

    blockExpr* me::onBlock() {
        NAMU_DI("tokenEvent: onBlock()");
        return _maker.make<blockExpr>();
    }

    blockExpr* me::onBlock(blockExpr& blk, node& stmt) {
        NAMU_DI("tokenEvent: onBlock()");
        if(nul(blk))
            return onSrcErr(errCode::IS_NULL, "blk"), onBlock();

        blk.getStmts().add(stmt);
        NAMU_DI("tokenEvent: onBlock().len=%d", blk.getStmts().len());
        return &blk;
    }

    defBlock* me::onDefBlock() {
        NAMU_DI("tokenEvent: onDefBlock()");
        return new defBlock();
    }

    defBlock* me::onDefBlock(defBlock& s, node& candidate) {
        NAMU_DI("tokenEvent: onDefBlock(candidate=%s)", candidate.getType().getName().c_str());
        if(nul(s))
            return onSrcErr(errCode::IS_NULL, "s"), onDefBlock();

        defVarExpr& defVar = candidate.cast<defVarExpr>();
        if(!nul(defVar)) {
            node* clone = (node*) defVar.getOrigin().clone();
            clone->_setPos(defVar.getPos());
            s.asScope->add(defVar.getName(), *clone);
            return &s;
        }
        defAssignExpr& defAssign = candidate.cast<defAssignExpr>();
        if(!nul(defAssign)) {
            defAssign.setTo(*_maker.make<getExpr>("me"));
            defAssign.setOnDefBlock(true);
            s.asPreCtor->add(defAssign);
            return &s;
        }

        s.asScope->add(_onPopName(candidate), &candidate);
        return &s;
    }

    node* me::onDefVar(const std::string& name, const node& origin) {
        NAMU_DI("tokenEvent: onDefVar(%s, %s)", origin.getType().getName().c_str(), name.c_str());

        return _maker.make<defVarExpr>(name, origin);
    }

    node* me::onDefArray(const narr& items) {
        NAMU_DI("tokenEvent: onDefArray(items.len[%d])", items.len());
        return _maker.make<defArrayExpr>(items);
    }

    node* me::onDefSeq(const node& start, const node& end) {
        NAMU_DI("tokenEvent: onDefSeq()");

        return _maker.make<defSeqExpr>(start, end);
    }

    void me::onSrcArea(const area& area) {
        _srcArea = area;
        ++_srcArea;
        NAMU_DI("tokenEvent: onSrcArea(%d, %d) -> (%d, %d)", area.start.row, area.start.col,
                _srcArea.start.row, _srcArea.start.col);
        _maker.setRow(_srcArea.start.row).setCol(_srcArea.start.col);
    }

    void me::_onRes(err* new1) {
        new1->log();
        _report->add(new1);
    }

    params me::_convertParams(const narr& ps) {
        params ret;
        for(auto& p : ps) {
            tstr<defVarExpr> defVar(p.cast<defVarExpr>());
            if(!defVar) return onSrcErr(errCode::PARAM_HAS_VAL), ret;

            ret.add(new param(defVar->getName(), defVar->getOrigin()));
        }

        return ret;
    }

    mgdFunc* me::onFunc(const std::string& name, const narr& exprs, const node& evalObj, const blockExpr& blk) {
        NAMU_DI("tokenEvent: onFunc: %s(...[%x]) %s: blk.len()=%d", name.c_str(), &exprs,
                evalObj.getType().getName().c_str(), blk.getStmts().len());

        tstr<narr> exprsWrapper(exprs);
        mgdFunc* ret = _maker.make<mgdFunc>(_convertParams(*exprsWrapper), evalObj, blk);
        _onPushName(name, *ret);
        return ret;
    }

    narr* me::onList() {
        narr* ret = new narr();
        NAMU_DI("tokenEvent: onList()=%x", ret);
        return ret;
    }

    narr* me::onList(node* newExpr) {
        narr* ret = new narr(*newExpr);
        NAMU_DI("tokenEvent: onList(%s[%x])=%x", newExpr->getType().getName().c_str(), newExpr, ret);
        return ret;
    }

    narr* me::onList(narr& list, node* newExpr) {
        NAMU_DI("tokenEvent: onList(list[%x], %s[%x])", &list, newExpr->getType().getName().c_str(), newExpr);
        list.add(newExpr);
        return &list;
    }

    args* me::onTypeNames(const node& param) {
        NAMU_DI("tokenEvent: onTypeNames(%s)", param.getType().getName().c_str());
        auto* ret = new args();
        ret->add(param);
        return ret;
    }

    args* me::onTypeNames(args& params, const node& param) {
        NAMU_DI("tokenEvent: onTypeNames(len[%d], %s)", params.len(), param.getType().getName().c_str());
        params.add(param);
        return &params;
    }

    obj* me::onDefObj(const std::string& name, defBlock& blk) {
        NAMU_DI("tokenEvent: onDefObj(%s, defBlock[%x])", name.c_str(), &blk);

        obj& ret = *_maker.make<obj>(new mgdType(name));
        ret._setComplete(false);
        _onInjectObjSubs(ret, blk);
        ret.getShares().link(_subpack->subs());

        _onPushName(name, ret);
        return &ret;
    }

    std::vector<std::string> me::_extractParamTypeNames(const args& types) {
        std::vector<std::string> ret;
        for(const auto& a : types) {
            // all args should be getExpr instances.
            const getExpr& cast = a.cast<getExpr>();
            if(nul(cast))
                return onSrcErr(errCode::SHOULD_TYPE_PARAM_NAME, a.getType().getName().c_str()),
                       std::vector<std::string>();

            ret.push_back(cast.getSubName());
        }

        return ret;
    }

    node* me::onDefObjGeneric(const std::string& name, const args& typeParams, defBlock& blk) {
        NAMU_DI("tokenEvent: onDefObjGeneric(%s, type.len[%d], defBlock[%x]", name.c_str(),
                typeParams.len(), &blk);

        obj& org = *_maker.make<obj>(new mgdType(name, typeParams));
        org._setComplete(false);
        _onInjectObjSubs(org, blk);
        org.getShares().link(_subpack->subs());

        node* ret = _maker.make<genericObj>(org, _extractParamTypeNames(typeParams));
        _onPushName(name, *ret);
        return ret;
    }

    void me::onCompilationUnit(obj& subpack, defBlock& blk) {
        NAMU_DI("tokenEvent: onCompilationUnit(%x, defBlock[%x])", &subpack, &blk);
        if(nul(subpack)) {
            onErr(errCode::NO_PACK_TRAY);
            return;
        }

        _onInjectObjSubs(subpack, blk);

        // link system slots:
        subpack.getShares().link(thread::get().getSlots());
        NAMU_DI("link system slots[%d]: len=%d", thread::get().getSlots().len(), subpack.subs().len());

        // at this far, subpack must have at least 1 default ctor created just before:
        NAMU_DI("tokenEvent: onCompilationUnit: run preconstructor(%d lines)",
                (blk.asPreCtor) ? blk.asPreCtor->len() : 0);
        subpack.run(baseObj::CTOR_NAME); // don't need argument. it's default ctor.
    }

    nbool me::_onInjectObjSubs(obj& it, defBlock& blk) {
        NAMU_DI("tokenEvent: _onInjectObjSubs(%s, defBlock[%x])", it.getType().getName().c_str(), &blk);
        if(nul(it)) return false;

        bicontainable& share = it.getShares().getContainer();
        bicontainable& own = it.getOwns();
        for(auto e=blk.asScope->begin(); e ;++e) {
            // TODO: not only func, but also shared variable.
            bicontainable& con = nul(e.getVal<func>()) ? own : share;
            con.add(e.getKey(), *e);
        }

        return _onInjectCtor(it, blk);
    }

    nbool me::_onInjectCtor(obj& it, defBlock& blk) {
        nbool hasCtor = !nul(it.sub(baseObj::CTOR_NAME));
        NAMU_DI("tokenEvent: _onInjectDefaultCtor(%s, has=%d)", it.getType().getName().c_str(), hasCtor);
        if(hasCtor) return false;

        // TODO: ctor need to call superclass's ctor.
        it.getShares().getContainer().add(baseObj::CTOR_NAME, *_maker.make<defaultCtor>(it.getOrigin()));
        it.getShares().getContainer().add(baseObj::CTOR_NAME, *_maker.make<defaultCopyCtor>(it.getOrigin()));

        // add preCtor:
        if(blk.asPreCtor && blk.asPreCtor->len()) {
            mgdFunc* preCtor = _maker.make<mgdFunc>(params(), new nVoid());
            preCtor->getBlock().getStmts().add(*blk.asPreCtor);
            it.subs().add(baseObj::PRECTOR_NAME, preCtor);
        }
        return true;
    }

    retExpr* me::onRet() {
        NAMU_DI("tokenEvent: onRet()");

        return _maker.make<retExpr>();
    }

    retExpr* me::onRet(node& exp) {
        NAMU_DI("tokenEvent: onRet(%s)", exp.getType().getName().c_str());

        return _maker.make<retExpr>(exp);
    }

    breakExpr* me::onBreak() {
        NAMU_DI("tokenEvent: onBreak()");

        return _maker.make<breakExpr>();
    }

    breakExpr* me::onBreak(node& exp) {
        NAMU_DI("tokenEvent: onBreak(%s)", exp.getType().getName().c_str());

        return _maker.make<breakExpr>(exp);
    }

    nextExpr* me::onNext() {
        NAMU_DI("tokenEvent: onNext()");

        return _maker.make<nextExpr>();
    }

    narr* me::onPackDotname(const std::string& name) {
        narr* ret = new narr();
        ret->add(new nStr(name));
        return ret;
    }

    narr* me::onPackDotname(narr& names, const std::string& name) {
        names.add(new nStr(name));
        return &names;
    }

    getExpr* me::onDotname(const getExpr& from, getExpr& name) {
        name.setMe(from);
        return &name;
    }

    getExpr* me::onDotname(const std::string& name) {
        return _maker.make<getExpr>(name);
    }

    node* me::onGet(const std::string& name) {
        NAMU_DI("tokenEvent: onGet(%s)", name.c_str());
        return _maker.make<getExpr>(name);
    }

    node* me::onGet(const std::string& name, const narr& args) {
        NAMU_DI("tokenEvent: onGet(%s, %d)", name.c_str(), args.len());
        return _maker.make<getExpr>(name, args);
    }

    node* me::onGet(node& from, const std::string& name) {
        NAMU_DI("tokenEvent: onGet(%s, %s)", from.getType().getName().c_str(), name.c_str());
        return _maker.make<getExpr>(from, name);
    }

    node* me::onGet(node& from, const std::string& name, const narr& args) {
        NAMU_DI("tokenEvent: onGet(%s, %s, %d)", from.getType().getName().c_str(), name.c_str(),
                args.len());
        return _maker.make<getExpr>(from, name, args);
    }

    node* me::onGetArray(node& elemType) {
        NAMU_DI("tokenEvent: onGetArray(%s)", elemType.getType().getName().c_str());
        return new arr(elemType);
    }

    node* me::onGetElem(const node& arr, const node& idx) {
        NAMU_DI("tokenEvent: onGetElem(%s, %s)", arr.getType().getName().c_str(),
                idx.getType().getName().c_str());

        return _maker.make<runExpr>(arr, *_maker.make<getExpr>(arr, "get", *new args{narr{idx}}),
                args{narr{idx}});
    }

    node* me::onGetGeneric(const std::string& genericObjName, const args& typeParams) {
        NAMU_DI("tokenEvent: onGetGeneric(%s, params.len[%d])", genericObjName.c_str(), typeParams.len());
        return _maker.make<getGenericExpr>(genericObjName, typeParams);
    }

    void me::_onPushName(const std::string& name, node& n) {
        _nameMap[&n] = name;
    }

    std::string me::_onPopName(node& n) {
        std::string ret = _nameMap[&n];
        _nameMap.erase(&n);
        return ret;
    }

    runExpr* me::onRunExpr(const node& trg, const narr& a) {
        NAMU_DI("tokenEvent: onRunExpr(%s, narr[%d])", trg.getType().getName().c_str(), a.len());

        return _maker.make<runExpr>(nulOf<node>(), trg, *new args(a));
    }
    runExpr* me::onRunExpr(const node& trg, const args& a) {
        NAMU_DI("tokenEvent: onRunExpr(%s, args[%d])", trg.getType().getName().c_str(), a.len());

        return _maker.make<runExpr>(nulOf<node>(), trg, a);
    }

    // @param from  can be expr. so I need to evaluate it through 'as()'.
    runExpr* me::onFillFromOfFuncCall(const node& me, runExpr& to) {
        NAMU_DI("tokenEvent: onFillFromOfFuncCall(%s, runExpr[%x])", me.getType().getName().c_str(), &to);

        to.setMe(me);
        return &to;
    }

    node* me::onAssign(node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());
        // _onSetElem branch:
        //  if user code is 'arr[0] = 1', then it will be interpreted to 'arr.set(0, 1)'
        runExpr& cast = lhs.cast<runExpr>();
        if(!nul(cast)) {
            getExpr& subject = cast.getSubject().cast<getExpr>();
            if(!nul(subject))
                if(subject.getSubName() == "get")
                    return _onSetElem(cast, rhs);
        }

        return _maker.make<assignExpr>(lhs, rhs);
    }

    node* me::_onSetElem(runExpr& lhs, const node& rhs) {
        //  if user code is 'arr[0] = 1', then it will be interpreted to 'arr.set(0, 1)'
        //  for instance,
        //  AST: before
        //      assignExpr
        //          |-[0]: runExpr (lhs)
        //          |       |-[0]: getExpr: "arr" from 'frame'
        //          |       |-[1]: getExpr: "get" from [0]
        //          |       |       |-[0]: nInt
        //          |       |-[2]: nInt: 0
        //          |-[1]: nInt: 1 (rhs)
        //
        //  after:
        //      runExpr
        //          |-[0]: getExpr: "arr" from 'frame'
        //          |-[1]: getExpr: "set" from [0]
        //          |-[2]: nInt: 0 // --> this was runExpr[2] of lhs.
        //          |-[3]: nInt: 1 (rhs)
        //
        //  conclusion:
        //      1. rename 'get' of 'lhs[1].getExpr.getSubName to 'set'.
        //      2. unbind subArgs of (1).
        //      3. lhs.getArgs().add(rhs).
        NAMU_DI("tokenEvent:: _onSetElem(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());

        getExpr& subject = lhs.getSubject().cast<getExpr>();
        subject._name = "set";
        subject._args.rel();
        lhs.getArgs().add(rhs);

        return &lhs;
    }

    node* me::_onAssignElem(FBOExpr::rule type, node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onFBOAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());

        // _onConvertAssignElem branch:
        //  if user code is 'arr[0] = 1', then it will be interpreted to 'arr.set(0, 1)'
        runExpr& cast = lhs.cast<runExpr>();
        if(!nul(cast)) {
            getExpr& subject = cast.getSubject().cast<getExpr>();
            if(!nul(subject))
                if(subject.getSubName() == "get")
                    return _onConvertAssignElem(cast, *_maker.make<FBOExpr>(type, lhs, rhs));
        }
        return onAssign(lhs, *_maker.make<FBOExpr>(type, *(node*) lhs.clone(), rhs));
    }

    node* me::_onConvertAssignElem(runExpr& lhs, node& rhs) {
        // if user code is 'arr[0] += 1' then it will be interpreted to 'arr.set(0, arr.get(0) + 1)
        // for instance,
        // AST: before
        //  lhs: arr.get(0)
        //      runExpr
        //          |-[0]: getExpr: "arr" from 'frame'
        //          |-[1]: getExpr: "get" from [0]
        //          |-[2]: nInt: 0
        //  rhs: 1
        //      nInt: 1
        //
        // after: arr.set(0, arr.get(0) + 1)
        //  runExpr
        //      |-[0]: getExpr: "arr" from 'frame
        //      |-[1]: getExpr: "set" from [0]
        //      |-[2]: nInt: 0
        //      |-[3]: FBOExpr(ADD) (rhs of this func)
        //              |-[0]: runExpr (lhs of this func)
        //              |         |-[0]: getExpr: "arr" from 'frame'
        //              |         |-[1]: getExpr: "get" from [0]
        //              |         |-[2]: nInt: 0
        //              |-[1]: nInt: 1
        //
        // conclusion:
        //  1. deep clone lhs and set its[1].subName to 'set'
        //  2. (1).getArgs.add(rhs)
        NAMU_DI("tokenEvent: _onConvertAssignElem(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());

        runExpr& setter = *(runExpr*) lhs.clone();
        getExpr& newSubj = *(getExpr*) setter.getSubject().clone();
        newSubj._name = "set";
        newSubj._args.rel();
        setter._subject.bind(newSubj);
        setter.getArgs().add(rhs);
        return &setter;
    }

    node* me::onAddAssign(node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onAddAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());
        return _onAssignElem(FBOExpr::ADD, lhs, rhs);
    }

    node* me::onSubAssign(node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onSubAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());
        return _onAssignElem(FBOExpr::SUB, lhs, rhs);
    }

    node* me::onMulAssign(node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onMulAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());
        return _onAssignElem(FBOExpr::MUL, lhs, rhs);
    }

    node* me::onDivAssign(node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onDivAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());
        return _onAssignElem(FBOExpr::DIV, lhs, rhs);
    }

    node* me::onModAssign(node& lhs, node& rhs) {
        NAMU_DI("tokenEvent: onModAssign(%s, %s)", lhs.getType().getName().c_str(),
                rhs.getType().getName().c_str());
        return _onAssignElem(FBOExpr::MOD, lhs, rhs);
    }

    node* me::onDefAssign(const std::string& name, node& rhs) {
        NAMU_DI("tokenEvent: onDefAssign(%s, %s)", name.c_str(), rhs.getType().getName().c_str());

        return _maker.make<defAssignExpr>(name, rhs);
    }

    asExpr* me::onAs(const node& me, const node& as) {
        NAMU_DI("tokenEvent: onAs(%s, %s)", me.getType().getName().c_str(), as.getType().getName().c_str());

        return _maker.make<asExpr>(me, as);
    }

    FBOExpr* me::onUnaryMinus(const node& it) {
        NAMU_DI("tokenEvent: onUnaryMinus(%s)", it.getType().getName().c_str());

        return _maker.make<FBOExpr>(FBOExpr::MUL, it, *new nInt(-1));
    }

    node* me::onUnaryDoublePlus(node& it) {
        NAMU_DI("tokenEvent: onUnaryDoublePlus(%s)", it.getType().getName().c_str());

        return onAssign(it, *_maker.make<FBOExpr>(FBOExpr::ADD, it, *new nInt(1)));
    }

    node* me::onUnaryDoubleMinus(node& it) {
        NAMU_DI("tokenEvent: onUnaryDoubleMinus(%s)", it.getType().getName().c_str());

        return onAssign(it, *_maker.make<FBOExpr>(FBOExpr::SUB, it, *new nInt(1)));
    }

    FUOExpr* me::onUnaryPostfixDoublePlus(const node& it) {
        NAMU_DI("tokenEvent: onUnaryPostfixDoublePlus(%s)", it.getType().getName().c_str());

        return _maker.make<FUOExpr>(FUOExpr::POSTFIX_DOUBLE_PLUS, it);
    }

    FUOExpr* me::onUnaryPostfixDoubleMinus(const node& it) {
        NAMU_DI("tokenEvent: onUnaryPostfixDoubleMinus(%s)", it.getType().getName().c_str());

        return _maker.make<FUOExpr>(FUOExpr::POSTFIX_DOUBLE_MINUS, it);
    }

    FBOExpr* me::onUnaryNot(const node& it) {
        NAMU_DI("tokenEvent: onUnaryNot(%s)", it.getType().getName().c_str());

        return _maker.make<FBOExpr>(FBOExpr::EQ, it, *new nBool(false));
    }

    FUOExpr* me::onUnaryBitwiseNot(const node& it) {
        NAMU_DI("tokenEvent: onUnaryBitwiseNot(%s)", it.getType().getName().c_str());

        return _maker.make<FUOExpr>(FUOExpr::BITWISE_NOT, it);
    }

    FBOExpr* me::onAdd(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onAdd(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::ADD, lhs, rhs);
    }

    FBOExpr* me::onSub(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onSub(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::SUB, lhs, rhs);
    }

    FBOExpr* me::onMul(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onMul(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::MUL, lhs, rhs);
    }

    FBOExpr* me::onDiv(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onDiv(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::DIV, lhs, rhs);
    }

    FBOExpr* me::onMod(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onMod(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::MOD, lhs, rhs);
    }

    FBOExpr* me::onBitwiseAnd(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onBitwiseAnd(%s, %s)", lhs.getType().getName().c_str(), rhs.getType()
                .getName().c_str());

        return _maker.make<FBOExpr>(FBOExpr::BITWISE_AND, lhs, rhs);
    }

    FBOExpr* me::onBitwiseOr(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onBitwiseOr(%s, %s)", lhs.getType().getName().c_str(), rhs.getType()
                .getName().c_str());

        return _maker.make<FBOExpr>(FBOExpr::BITWISE_OR, lhs, rhs);
    }

    FBOExpr* me::onBitwiseXor(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onBitwiseXor(%s, %s)", lhs.getType().getName().c_str(), rhs.getType()
                .getName().c_str());

        return _maker.make<FBOExpr>(FBOExpr::BITWISE_XOR, lhs, rhs);
    }

    FBOExpr* me::onLShift(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: LShift(%s, %s)", lhs.getType().getName().c_str(), rhs.getType()
                .getName().c_str());

       return _maker.make<FBOExpr>(FBOExpr::LSHIFT, lhs, rhs);
    }

    FBOExpr* me::onRShift(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: RShift(%s, %s)", lhs.getType().getName().c_str(), rhs.getType()
                .getName().c_str());

        return _maker.make<FBOExpr>(FBOExpr::RSHIFT, lhs, rhs);
    }

    FBOExpr* me::onGt(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onGt(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::GT, lhs, rhs);
    }

    FBOExpr* me::onLt(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onLt(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::LT, lhs, rhs);
    }

    FBOExpr* me::onGe(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onGe(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::GE, lhs, rhs);
    }

    FBOExpr* me::onLe(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onLe(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::LE, lhs, rhs);
    }

    FBOExpr* me::onEq(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onEq(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::EQ, lhs, rhs);
    }

    FBOExpr* me::onNe(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onNe(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::NE, lhs, rhs);
    }

    FBOExpr* me::onAnd(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onAnd(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::AND, lhs, rhs);
    }

    FBOExpr* me::onOr(const node& lhs, const node& rhs) {
        NAMU_DI("tokenEvent: onOr(%s, %s)", lhs.getType().getName().c_str(), rhs.getType().getName()
                .c_str());

        return _maker.make<FBOExpr>(FBOExpr::OR, lhs, rhs);
    }

    node* me::onFor(const std::string& iterName, const node& expr, const blockExpr& blk) {
        NAMU_DI("tokenEvent: onFor(%s, %s)", iterName.c_str(), expr.getType().getName().c_str());

        return _maker.make<forExpr>(iterName, expr, blk);
    }

    node* me::onWhile(const node& condition, const blockExpr& blk) {
        NAMU_DI("tokenEvent: onWhile(%s)", condition.getType().getName().c_str());

        return _maker.make<whileExpr>(condition, blk);
    }

    node* me::onAkaDefault(const getExpr& dotname, const std::string& newName) {
        NAMU_DI("tokenEvent: onAkaDefault(%s..., %s)", dotname.getSubName().c_str(), newName.c_str());

        return _maker.make<defAssignExpr>(newName, dotname);
    }

    ifExpr* me::onIf(const node& condition, const blockExpr& thenBlk) {
        NAMU_DI("tokenEvent: onIf(then)");

        ifExpr* ret = _maker.make<ifExpr>(condition, thenBlk);
        _outerIfStack.push_back(ret);

        return ret;
    }
    ifExpr* me::onElif(ifExpr& ifexpr, const node& elseIfCondition, const blockExpr& thenBlk) {
        NAMU_DI("tokenEvent: onElIf(ifexpr, condition, then)");
        ifExpr* ret = _maker.make<ifExpr>(elseIfCondition, thenBlk);

        ifexpr.setElseBlk(*new blockExpr(*ret));
        return ret;
    }
    ifExpr* me::onElse(ifExpr& ifexpr, const blockExpr& elseBlk) {
        NAMU_DI("tokenEvent: onElse(ifexpr, elseBlk)");

        ifexpr.setElseBlk(elseBlk);
        return onEndOfIf();
    }
    ifExpr* me::onEndOfIf() {
        ifExpr* ret = _outerIfStack.back();
        _outerIfStack.pop_back();
        return ret;
    }

    void me::onParseErr(const std::string& msg, const nchar* symbolName) {
        onErr(_srcArea.start, errCode::SYNTAX_ERR, msg.c_str(), symbolName);
    }

    me::loweventer() { rel(); }

    tstr<slot>& me::getSlot() {
        return _slot;
    }

    tstr<obj>& me::getSubPack() {
        return _subpack; // TODO: can I remove subpack variable?
    }

    tstr<errReport>& me::getReport() {
        return _report;
    }

    tokenDispatcher& me::getDispatcher() {
        return _dispatcher;
    }

    std::vector<ncnt>& me::getIndents() {
        return _indents;
    }

    const area& me::getArea() const {
        return _srcArea;
    }

    nbool me::isInit() const {
        return _mode;
    }

    void me::rel() {
        _report.bind(dummyErrReport::singletone);
        _slot.rel();
        _nameMap.clear();
        _states.clear();
        _states.push_back(0); // 0 for default state
        prepareParse();
    }

    void me::prepareParse() {
        _mode = nullptr;
        _subpack.rel();
        _isIgnoreWhitespace = false;
        _dispatcher.rel();
        _indents.clear();
        _srcArea.rel();
        _outerIfStack.clear();
        _maker.rel();
        _maker.setSrc(*new src("__filename__"));
    }

    int me::pushState(int newState) {
        NAMU_I("push state %d -> %d", _states.back(), newState);
        _states.push_back(newState);
        return _states.back();
    }

    int me::popState() {
        int previous = _states.back();
        _states.pop_back();
        NAMU_I("pop state %d <- %d", _states.back(), previous);
        return _states.back();
    }
}
