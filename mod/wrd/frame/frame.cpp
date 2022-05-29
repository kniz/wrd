#include "frame.hpp"
#include "../ast/obj.hpp"
#include "../ast/func.hpp"

namespace wrd {

    WRD_DEF_ME(frame)

    me::~frame() {
        _rel();
    }

    wbool me::pushLocal(nbicontainer* con) { return pushLocal(*con); }
    wbool me::pushLocal(nbicontainer& con) { return pushLocal(*scopes::wrap(con)); }
    wbool me::pushLocal(scopes* new1) { return _local.push(*new1); }
    wbool me::pushLocal(scopes& new1) {
        wbool ret = _local.push(new1);
        if(ret && _local.chainLen() == 1 && _obj)
            new1.link(_obj->subs());
        return ret;
    }
    wbool me::pushLocal(const std::string& name, node& n) {
        scopes& top = *_local.getTop();
        if(nul(top))
            return WRD_E("couldn't push new node. the top scope is null"), false;

        return top.add(name, n);
    }
    wbool me::pushLocal(const std::string& name, node* n) {
        return pushLocal(name, *n);
    }

    scopes& me::getTop() { return *_local.getTop(); }

    tstr<scopes> me::popLocal() { return _local.pop(); }
    // I won't provide API for poping a single node from the scope.

    void me::setObj(const obj& new1) {
        _obj.bind(new1);
        scopes& bottom = *_local.getBottom();
        if(!nul(bottom)) {
            if(nul(new1))
                bottom.unlink();
            else
                bottom.link(new1.subs());
        }
    }

    const obj& me::getObj() const { return *_obj; }
    obj& me::getObj() { return *_obj; }

    void me::setFunc(func& new1) {
        _func.bind(new1);
    }

    func& me::getFunc() {
        return *_func;
    }

    void me::clearObj() {
        setObj(nulOf<obj>());
    }

    // node:
    nbicontainer& me::subs() {
        scopes& top = *_local.getTop();
        return nul(top) ? _obj->subs() : top;
    }

    wbool me::canRun(const ucontainable& args) const { return false; }

    str me::run(const ucontainable& args) { return str(); }

    void me::rel() {
        _rel();
        super::rel();
    }

    wbool me::pushReturn(const str& toReturn) { return _ret.bind(*toReturn); }
    wbool me::isReturned() const { return _ret.isBind(); }
    str me::popReturn() {
        str ret = _ret;
        _ret.rel();
        return ret;
    }

    void me::_rel() {
        _obj.rel();
        _func.rel();
        _local.rel();
    }
}
