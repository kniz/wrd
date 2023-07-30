#include "slot.hpp"
#include "func.hpp"
#include "../visitor/visitor.hpp"
#include "../type/mgdType.hpp"

namespace namu {

    NAMU(DEF_ME(slot), DEF_VISIT())


    nbicontainer& me::subs() {
        return getPack().subs();
    }

    nbool me::canRun(const args& a) const {
        return getPack().canRun(a);
    }

    str me::run(const args& a) {
        return getPack().run(a);
    }

    str me::getEval() const {
        return getPack().getEval();
    }

    me::slot(const manifest& manifest): _manifest(manifest) {
        _rel();
        _pak.bind(new obj(new mgdType(_manifest.name)));
    }

    me::slot(const manifest& manifest, const obj& pack): _manifest(manifest) {
        _rel();
        _pak.bind(pack);
    }

    void me::_rel() {
        _dependents.rel();
        _pak.rel();
        _isValid = true;
    }

    obj& me::getPack() {
        return *_pak;
    }

    manifest& me::getManifest() { return _manifest; }
    const manifest& me::getManifest() const { return _manifest; }
    nbool me::isValid() const { return _isValid; }

    void me::rel() {
        _rel();
    }

    void me::addDependent(slot& dependent) {
        _dependents.add(dependent);
    }

    const tnarr<slot>& me::getDependents() const {
        return _dependents;
    }

    void me::_setValid(nbool valid) {
        _isValid = valid;
    }

    nbool me::_invalidate() {
        _setValid(false);

        // propagate result only if it's not valid.
        for(auto& e : _dependents)
            e._invalidate();
        return true;
    }

    str me::_onRunSub(node& sub, const args& a) {
        return getPack()._onRunSub(sub, a);
    }
}
