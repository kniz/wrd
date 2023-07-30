#include "node.inl"
#include "scope.inl"
#include "../builtin/container/native/tnarr.inl"
#include "../type/as.hpp"
#include "args.hpp"
#include "../visitor/visitor.hpp"

namespace namu {

    NAMU(DEF_ME(node), DEF_VISIT())

    node& me::operator[](const std::string& name) const {
        return sub(name);
    }

    str me::run() {
        return run(args());
    }

    str me::run(const std::string& name) {
        return run(name, args());
    }

    str me::run(const std::string& name, const args& a) {
        if(name.empty()) return run(a);
        me& found = sub(name, a);
        if(nul(found)) return str();

        return _onRunSub(found, a);
    }

    nbool me::is(const typeProvidable& to) const {
        if(nul(to)) return false;
        return is(to.getType());
    }

    nbool me::is(const type& to) const {
        return getType().is(to);
    }

    str me::as(const typeProvidable& to) const {
        if(nul(to)) return str();
        return as(to.getType());
    }

    str me::as(const type& to) const {
        return getType().as(*this, to);
    }

    nbool me::isImpli(const typeProvidable& to) const {
        if(nul(to)) return false;
        return isImpli(to.getType());
    }

    nbool me::isImpli(const type& to) const {
        return getType().isImpli(to);
    }

    str me::asImpli(const typeProvidable& to) const {
        if(nul(to)) return str();
        return asImpli(to.getType());
    }

    str me::asImpli(const type& to) const {
        return getType().asImpli(*this, to);
    }

    str me::getEval() const {
        return str(this);
    }

    str me::_onRunSub(node& sub, const args& a) {
        return sub.run(a);
    }

    const point& me::getPos() const {
        static point inner;
        return inner;
    }

    clonable* me::deepClone() const {
        return (me*) clone();
    }

    nbool me::isComplete() const {
        return true;
    }

    void me::inFrame(const bicontainable& args) {}

    void me::outFrame() {}

    void me::_setPos(const point& new1) {}
}
