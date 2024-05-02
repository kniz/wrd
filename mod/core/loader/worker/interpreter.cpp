#include "../../ast/exprs/blockExpr.hpp"
#include "../../ast/func.hpp"
#include "interpreter.hpp"
#include "../../visitor/graphVisitor.hpp"
#include "../../visitor/preEvaluator.hpp"
#include "../../frame/threadUse.hpp"

namespace namu {

    NAMU_DEF_ME(interpreter)

    me::interpreter(): _isParsed(false), _isLogStructure(false) {}

    me& me::setReport(errReport& report) {
        _rpt.bind(report);
        return *this;
    }

    me& me::setSlot(slot& tray) {
        _slot.bind(tray);
        return *this;
    }

    me& me::addSupply(const srcSupply& supply) {
        _pser.addSupply(supply);
        return *this;
    }

    me& me::relSupplies() {
        _pser.relSupplies();
        return *this;
    }

    me& me::setLogStructure(nbool enable) {
        _isLogStructure = enable;
        return *this;
    }

    me& me::setVerbose(nbool isVerbose) {
        _isVerbose = isVerbose;
        return *this;
    }

    nbool me::isParsed() const {
        return _isParsed;
    }

    nbool me::isVerified() const {
        return isParsed() && (_rpt && !_rpt->hasErr());
    }

    node& me::getSubPack() {
        return _pser.getSubPack();
    }

    slot& me::getSlot() {
        return _pser.getSlot();
    }

    errReport& me::getReport() {
        return *_rpt;
    }

    slot& me::interpret() {
        threadUse thr;
        _parse();
        if(*_rpt)
            return *_slot;
        _preEval();
        _verify();

        auto& info = _veri.getErrFrame();
        _logFrame(info); std::cout << "\n";
        _logStructure(info); std::cout << "\n";
        _log();

        return *_slot;
    }

    void me::rel() {
        _isParsed = false;
        _rpt.rel();
        _veri.rel();
        _pser.rel();
        _slot.rel();
    }

    void me::log() const {
        if(!_rpt && !*_rpt) return;
        if(!_isVerbose) return;

        _rpt->log();
    }

    nbool me::_isPackExist() {
        return !nul(_pser.getSubPack()) && _slot;
    }

    void me::_parse() {
        NAMU_DI("======================================");
        NAMU_DI("                parse                 ");
        NAMU_DI("======================================");

        _pser.setReport(*_rpt)
             .setTask(*_slot)
             .work();

        if(!_slot)
            _slot.bind(_pser.getTask());

        _isParsed = _isPackExist() && !_rpt->hasErr();
    }

    void me::_preEval() {
        NAMU_DI("======================================");
        NAMU_DI("                preEval               ");
        NAMU_DI("======================================");

        if(!_slot)
            return NAMU_E("_slot is null"), void();

        preEvaluator evaler;
        evaler.setReport(*_rpt)
              .setLog(false)
              .setTask(_slot->getPack())
              .work();
    }

    void me::_verify() {
        NAMU_DI("======================================");
        NAMU_DI("                verify                ");
        NAMU_DI("======================================");

        if(!_slot)
            return NAMU_E("_slot is null"), void();

        // verify:
        _veri.setReport(*_rpt)
             .setLog(true)
             .setTask(_slot->getPack())
             .work();
    }

    void me::_logStructure(frame& info) {
        if(!_isLogStructure) return;

        if(!nul(_pser.getSubPack()) && _slot) {
            std::cout << " - structure:\n";
            graphVisitor().setTask(_slot->getPack())
                          .work();
        }
    }

    void me::_logFrame(const frame& info) const {
        using platformAPI::foreColor;
        if(!_isLogStructure) return;
        if(nul(info))
            return std::cout << "    null\n", void();

        std::cout << foreColor(LIGHTGREEN) << " - frame:\n";

        int n=0;
        for(auto e = info.subs().begin(); e ;++e)
            std::cout << "    [" << n++ << "]: '" << e.getKey() << "' " << e.getVal().getType().getName().c_str() << "\n";

        std::cout << foreColor(LIGHTGRAY);
    }

    void me::_log() const {
        if(!_isVerbose) return;
        std::cout << " - err:\n";
        log();
    }
}
