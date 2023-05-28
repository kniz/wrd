#pragma once

#include "../common.hpp"

namespace namu {
    /// @remark type returning ttype<type> as result of getType()
    class _nout type {
        NAMU_DECL_ME(type)

    public:
        virtual ~type() {}

        virtual nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;

        virtual nbool isTemplate() const = 0;
        virtual nbool isAbstract() const = 0;
        virtual const std::string& getName() const = 0;

        /// @brief  create an instance to be refered this type.
        /// @remark available when the type defines a ctor without any params.
        /// @return return an address of new instance, however, if ctor without any params
        ///         isn't defined, then returns null.
        virtual void* make() const = 0;
        template <typename T>
        T* makeAs() const {
            // c++ is not allow for covariant against void*.
            // that's is why I make another variant func of already made make().
            return (T*) make();
        }

        virtual ncnt size() const = 0;
        virtual nbool init();
        virtual nbool rel();
        virtual const type& getSuper() const = 0;
        virtual const nbool& isInit() const = 0;

        /// returns all most derived class from this class.
        const types& getLeafs() const;
        const types& getSubs() const;
        const types& getSupers() const;

        nbool isSuper(const type& it) const;
        template <typename T> nbool isSuper() const;
        nbool isSub(const type& it) const;
        template <typename T> nbool isSub() const;
        const type& getStatic() const NAMU_UNCONST_FUNC(_getStatic())

        virtual const void* getExtra() const;

    protected:
        //  type:
        virtual types& _getSubs() = 0;
        virtual types& _getSupers() = 0;
        virtual type& _getStatic() const = 0;
        void _setInit(nbool newState);
        virtual void _onAddSubClass(const me& subClass);
        virtual types** _onGetLeafs() const = 0;
        void _setLeafs(types* newLeafs) const;

    private:
        nbool _logInitOk(nbool res);
        void _findLeafs(const type& cls, types& tray) const;
    };
}
