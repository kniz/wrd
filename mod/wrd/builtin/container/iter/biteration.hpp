#include "biterable.hpp"

// nested class of tbicontainable.hpp
class iteration : public instance, public iterable, public clonable {
    WRD(INTERFACE(iteration, instance))
    friend class iter;

public:
    wbool isFrom(const tbicontainable& rhs) const override {
        return &this->getContainer() == &rhs;
    }
};
