#include "genericObj.hpp"
#include "../builtin/container/native/tndumMap.hpp"
#include "../visitor/generalizer.hpp"
#include "args.hpp"
#include "../visitor/verifier.hpp"
#include "func.hpp"
#include "../type/mgdType.hpp"

namespace namu {

    NAMU(DEF_ME(genericObj), DEF_VISIT())

    me::genericObj(const obj& orgObj, const strings& paramNames): _orgObj(orgObj),
            _paramNames(paramNames) {}

    const obj& me::getOrigin() const {
        return *_orgObj;
    }

    const me::strings& me::getParamNames() const {
        return _paramNames;
    }

    str me::getEval() const {
        return _orgObj;
    }

    const std::map<std::string, tstr<obj>>& me::getCache() const {
        return _cache;
    }

    void me::inFrame(const bicontainable& args) {}

    void me::outFrame() {}

    str me::_onRunSub(node& sub, const args& a) {
        return str();
    }

    void me::_setSrc(const src& newSrc) {
        _src.bind(newSrc);
    }

    void me::_setPos(const point& new1) {
        _pos = new1;
    }

    nbicontainer& me::subs() {
        static ndumMap inner;
        return inner;
    }

    nbool me::canRun(const args& a) const {
        std::string key = _makeKey(a);
        if(key.empty()) return false;
        if(!_cache.count(key)) return false;
        // after verification, making GenericObject is supressed.

        return true;
    }

    str me::run(const args& a) {
        std::string key = _makeKey(a);
        if(key.empty()) return NAMU_E("key is empty"), nulOf<obj>();

        if(!_cache.count(key))
            _cache.insert({key, _defGeneric(key, a)});

        return _cache[key];
    }

    tstr<obj> me::_defGeneric(const std::string& key, const args& a) const {
        // this making generic object will be done by preEvaluator:
        //  when I run verification step, there are all of generic objects inside _cache.
        //  so, don't worry and verify genenated objects here.
        tstr<obj> gen = _makeGeneric(a);
        return gen;
    }

    std::string me::_makeKey(const args& a) const {
        if(a.len() != _paramNames.size()) return NAMU_E("len of args doesn't match to _paramNames"), std::string();
        return a.asStr();
    }

    /// make a generic object.
    tstr<obj> me::_makeGeneric(const args& a) const {
        if(!_orgObj) return NAMU_E("_orgObj is null"), tstr<obj>();

        NAMU_DI("=========================");
        NAMU_DI("        make generic     ");
        NAMU_DI("=========================");
        tstr<obj> ret = (obj*) _orgObj->deepClone();
        // update origin:
        //  genericObj makes an origin object. but ret->getOrigin() is pointing _orgObj now.
        //  I need to update it.
        ret->_setOrigin(&ret.get());
        // clone type:
        if(ret->_type) {
            mgdType* newType = new mgdType(*ret->_type);
            newType->getBeans() = a;
            ret->_setType(newType);
        }

        ncnt n = 0;
        generalizer g;
        for(auto& e : a)
            g.add(*new param(_paramNames[n++], e));

        g.setRoot(*ret).start();
        NAMU_DI("=========================");
        return ret;
    }
}
