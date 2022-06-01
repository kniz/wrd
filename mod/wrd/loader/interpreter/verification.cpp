#include "verification.hpp"
#include "../../ast/func.hpp"
#include "../../ast/node.hpp"
#include "verifier.hpp"
#include "../../frame/frame.hpp"

namespace wrd {

    WRD_DEF_ME(verification)

    void me::verify(node& it) {
        _getVerifier().verify(it);
    }

    wbool me::logFrameInfo(const frame& newFr) {
        tstr<frame>& info = _verifier->getFrameInfo();
        if(nul(info)) return false;

        return info.bind(newFr);
    }

    void me::verify(verifier& veri, node& it) {
        _it = &it;
        _verifier = &veri;
    }

    verifier& me::_getVerifier() { return *_verifier; }
    node& me::_getIt() { return *_it; }
}
