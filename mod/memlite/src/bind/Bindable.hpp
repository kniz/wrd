#pragma once

#include "../interface/Instance.hpp"

namespace wrd {

	class Bindable {
        WRD_DECL_THIS(Bindable)
        WRD_INIT_META(This)

	public:
        //  Bindable:
        const Instance* operator->() const;
        Instance* operator->();
        const Instance& operator*() const;
        Instance& operator*();
        operator wbool() const;

		wbool bind(const Instance& it);
        virtual wbool unbind() = 0;
		wbool canBind(const Instance& it);
		virtual wbool canBind(const Type& it) const = 0;
		virtual wbool isBind() const = 0;
		Instance& get();
		const Instance& get() const;

		template <typename T> T& get() {
			Instance& got = WRD_GETS(_get());
            WRD_NUL(got, nulr<T>())

            if(!got.getType().isSub(TType<T>::get()))
                return nulr<T>();

            return (T&) got;
		}

		template <typename T> const T& get() const WRD_UNCONST_FUNC(get<T>())

	protected:
        //  Bindable:
		virtual Instance& _get() = 0;
        virtual wbool _bind(const Instance& it) = 0;
	};
}
