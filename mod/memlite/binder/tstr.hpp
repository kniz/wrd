#pragma once

#include "tweak.hpp"
#include "strTactic.hpp"

namespace namu {

    template <typename T, typename TACTIC = strTactic>
    class tstr : public tweak<T, TACTIC> {
        typedef tweak<T, TACTIC> _super_;
        NAMU_DECL_ME(tstr, _super_)
        NAMU_INIT_META(me)
        friend class ref;

    public:
        //  tstr:
        tstr();
        /// @param subType  subType is should be sub type of 'T' or type 'T' will be used from base
        ///                 class.
        tstr(const type& subtype);
        tstr(const T& it);
        tstr(const T* it);
        tstr(const me& rhs);
        tstr(const binder& rhs);

        using super::operator=;
    };
}
