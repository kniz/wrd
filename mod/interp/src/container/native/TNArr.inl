#include "TNArr.hpp"

namespace wrd {

#define TEMPL template <typename T>
#define THIS TNArr<T>

    TEMPL
    template <typename E>
    void THIS::each(const TIter<T>& from, const TIter<T>& to, std::function<wbool(TIter<T>&, E&)> l) {
        for(TIter<T> e=from; e == to ;++e) {
            E& t = e->cast<E>();
            if(nul(t)) continue;

            if(!l(e, t)) return;
        }
    }

    TEMPL
    template <typename E>
    void THIS::each(std::function<wbool(TIter<T>&, E&)> l) {
        each(head(), tail(), l);
    }

    TEMPL
    template <typename E>
    void THIS::each(const TIter<T>& from, const TIter<T>& to, std::function<wbool(const TIter<T>&, const E&)> l) const {
        for(TIter<T> e=from; e == to ;++e) {
            E& t = e->cast<E>();
            if(nul(t)) continue;

            if(!l(e, t)) return;
        }
    }

    TEMPL
    template <typename E>
    void THIS::each(std::function<wbool(const TIter<T>&, const E&)> l) const {
        each(head(), tail(), l);
    }

#undef TEMPL
#undef THIS
}
