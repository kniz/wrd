#include "sobj.hpp"
#include "nullSobj.hpp"

namespace nm {

    NAMU_DEF_ME(sobj)

    me& me::sub(const std::string& name) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-undefined-compare"
        if (this == nullptr)
            return nulOf<me>();
#pragma clang diagnostic pop
        tstr<me>& ret = _subs[name];
        if(!ret)
                return nullSobj::get();

        return *ret;
    }

    me::sobj(std::initializer_list<me*> subs, const std::string& name): _name(name) {
        add(subs);
    }

    me::sobj(const me& rhs, const std::string& name): _subs(rhs._subs), _name(name) {}

    me::sobj(const std::string& name): _name(name) {}

    me& me::operator[](const std::string& name) {
        return sub(name);
    }

    me& me::operator[](nidx n) {
        return *std::next(begin(), n)->second;
    }

    me::operator nbool() const {
        return isExist();
    }

    nbool me::has(const std::string& name) const {
        return _subs.find(name) != _subs.end();
    }

    void me::add(const sobj& new1) {
        if(nul(new1)) return;

        _subs.insert(make_pair(new1.getName(), tstr<me>(new1)));
    }

    void me::add(std::initializer_list<sobj*> subs) {
        for(auto e : subs)
            add(*e);
    }

    void me::del(const sobj& it) {
        del(it.getName());
    }

    void me::del(const std::string& name) {
        _subs.erase(name);
    }

    const std::string& me::getName() const {
        return _name;
    }

    void me::setName(const std::string& newName) {
        _name = newName;
    }

    ncnt me::len() const {
        return _subs.size();
    }

    me::iterator me::begin() {
        return _subs.begin();
    }

    me::iterator me::end() {
        return _subs.end();
    }

    const type& me::getType() const {
        return ttype<me>::get();
    }

    nint me::asInt() const { return 0; }

    nchar me::asChar() const { return '\0'; }

    const std::string& me::asStr() const {
        static std::string inner;
        return inner;
    }

    nbool me::asBool() const { return false; }

    nbool me::isExist() const {
        return true;
    }
}
