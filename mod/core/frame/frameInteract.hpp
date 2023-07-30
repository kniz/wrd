#pragma once

#include "frameInteractable.hpp"

namespace namu {

    class frameInteract {
        NAMU(ME(frameInteract))

    public:
        frameInteract(frameInteractable& it, const bicontainable& args);
        frameInteract(frameInteractable& it);
        ~frameInteract();

    private:
        frameInteractable& _it;
    };
}
