#include "arr.hpp"
#include "../../../visitor/visitor.hpp"

namespace namu {

    NAMU(DEF_ME(arr), DEF_VISIT())

    me::arr(): _arr(new narr()), _type(&ttype<node>::get()) {}

    me::arr(const type& newType): _arr(new narr()), _type(&newType) {}

    node& me::operator[](nidx n) {
        return _arr->operator[](n);
    }

    tnarr<node>& me::getNative() {
        return *_arr;
    }

    const tnarr<node>& me::getNative() const {
        return *_arr;
    }

    ncnt me::len() const {
        return _arr->len();
    }

    nbool me::has(nidx n) const {
        return _arr->has(n);
    }

    node& me::get(nidx n) {
        return _arr->get(n);
    }

    nbool me::set(const iter& at, const node& new1) {
        if(!new1.isSub(*_type)) return false;
        return _arr->set(at, new1);
    }
    nbool me::set(nidx n, const node& new1) {
        if(!new1.isSub(*_type)) return false;
        return _arr->set(n, new1);
    }

    nbool me::add(const iter& at, const node& new1) {
        if(!new1.isSub(*_type)) return false;
        return _arr->add(at, new1);
    }

    nbool me::add(nidx n, const node& new1) {
        if(!new1.isSub(*_type)) return false;
        return _arr->add(n, new1);
    }

    void me::add(const iter& here, const iter& from, const iter& to) {
        _arr->add(here, from, to);
    }

    //  del:
    nbool me::del(nidx n) {
        return _arr->del(n);
    }

    nbool me::del(const iter& it) {
        return _arr->del(it);
    }

    nbool me::del(const iter& from, const iter& to) {
        return _arr->del(from, to);
    }

    //  etc:
    me* me::deepClone() const {
        me* ret = clone();
        tstr<tnarr<node>> cloned = _arr->deepClone();
        ret->_arr->add(*cloned);
        return ret;
    }

    void me::rel() {
        _arr.rel();

       super::rel();
    }

    me::iteration* me::_onMakeIteration(ncnt step) const {
        return _arr->_onMakeIteration(step);
    }
}
