#include "forExpr.hpp"
#include "../../builtin.hpp"
#include "../../visitor/visitor.hpp"
#include "../../frame/thread.hpp"
#include "../../frame/frameInteract.hpp"

namespace namu {

    NAMU(DEF_ME(forExpr), DEF_VISIT())

    me::forExpr(const std::string localName, const node& container, const blockExpr& blk):
        _container(container), _name(localName), _blk(blk) {}

    const std::string& me::getLocalName() const { return _name; }
    blockExpr& me::getBlock() { return *_blk; }
    str me::getContainer() { return _container->as<node>(); }

    str me::run(const args& a) {
        if(!_blk || !_container)
            return NAMU_E("blk or container is null"), str();

        str ased = _container->as<node>();
        if(!ased) return NAMU_E("ased is null"), str();
        str iter = ased->run("iterate", args{narr{*new nInt(0)}});
        if(!iter) return NAMU_E("iter is null"), str();

        str ret;
        frame& fr = thread::get()._getNowFrame();
        while(!iter->run("isEnd")->cast<nbool>()) {
            str elem = iter->run("get");
            if(!elem)
                return NAMU_E("elem is null"), str();

            frameInteract f1(*_blk); {
                fr.pushLocal(_name, *elem);

                ret = _blk->run();
                const retState& state = fr.getRetState();
                if(state  == frame::FUNC_RETURN)
                    return ret;
                else if(state == frame::BLK_BREAK) {
                    fr.setRet(frame::BLK_EMPTY);
                    return ret;
                }
            }

            iter->run("next", args{narr{*new nInt(1)}});
        }

        return ret;
    }

    const node& me::getEval() const {
        str ased = _container->as<node>();
        str elemType = ased->run("getElemType");
        if(!elemType) return NAMU_E("elemType == null"), nulOf<node>();

        frameInteract f1(*_blk); {
            thread::get()._getNowFrame().pushLocal(getLocalName(), *elemType);
            return _blk->getEval();
        }
    }
}
