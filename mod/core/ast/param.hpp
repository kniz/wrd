#pragma once

#include "../common.hpp"
#include "immutableTactic.hpp"
#include "../type/ntype.hpp"

namespace nm {
    class _nout param : public instance {
        NAMU(CLASS(param, instance))

    public:
        param(const std::string& newName, const node& org);
        param(const std::string& newName, const node* org);

    public:
        const std::string& getName() const;
        void setName(const std::string& new1);
        const node& getOrigin() const;
        void setOrigin(const node& newType);

    public:
        std::string _name;
        str _org;
    };
}
