class Thing {
	//	Thing은 World의 최상위 객체
	virtual Result& release();
	template <typename T, typename S>
	friend class TVisitation;
	//	_tour를 위한 것이다.
	wbool isNull() const { return !this; }
	wbool isExist() const { return this; }
	virtual Result& _tour(Visitor& visitor) const { return Success; }
	//	Visitor에 의해서 하위 구성요소(ownee)들을 어떻게 순회시킬지를 정한다.
	virtual Result& _tour(Visitor& visitor) {
		const This* consted = const_cast<const This*>(this);
		return consted->_onTour(visitor);
	}
	virtual wbool isSuper(const Class& it) const {
		return getClass().isSuper(it);
	}
	wbool isSuper(const Thing& it) const {
		return getClass().isSuper(it.getClass());
	}
	template <typename T>
	wbool isSuper() const {
		return getClass().isSuper(T::getStaticClass());
	}
	wbool isSub(const Thing& it) const {
		return it.getClass().isSuper(getClass());
	}
	wbool isSub(const Class& it) const {
		return it.isSuper(getClass());
	}
	template <typename T>
	wbool isSub() const {
		return T::getStaticClass().isSuper(getClass());
	}
	const Class& getClass();
	virtual Refer to(const Class& cls) {
		return Refer();
	}
	Refer to(const Class& cls) const {
		This& unconsted = const_cast<T&>(*this);
		wbool should_const = cls.isSuper(getClass());
		return Refer(unconsted.to(cls), should_const);
	}
	template <typename T>
	TRefer<T> to() {
		return Refer<T>(to(T::getStaticClass()));
	}
	template <typename T>
	TRefer<T> to() const {
		return Refer<T>(to(T::getStaticClass()));
	}

	//	구체클래스로 자동 캐스트한다. dynamic_cast와 동급이다.
	//	invisible이다.
	template <typename T>
	T& subcast() {
		if(isSub(T::getStaticClass()))
			return (T&) *this;
		return TNuller<T>::ref;
	}

	//	가상할당자이다. 할당연산자는 virtual이 안되기 때문에 제대로 할당을 하고 싶다면 항상 구체타입을 알고 있어야만 한다.
	virtual Result& assign(const Thing& it) {
		if(it.isNull()) return NullError;
	}
}
		//	Usage:
		//		to:
		A& a = ...;
		Refer ra = a.to(TClass<AChild>());
		ra.isConst(); // false
		ra = a.to(TClass<const AChild>()); // TClass<const T> is compile err
		ra = a.to(TClass<Integer>());
		Integer casted = *ra.to<Integer>(); // T of to<T> should be a sort of Node.
		//	At this moment, the returned value from ra.to<>() copied to casted variable,
		//	and was deleted automatically.
		TRefer<const AChild> cra = ra.to<const AChild>();
		//	cra자체는 const가 아니다. World는 const Reference라는 게 없기 때문이다. ref가 가리키는게 const냐 아니냐만 존재한다.

		cra->nonconstMethod(); // compile err. TRefer<const>는 operator->() const만 있다.
		const TRefer<AChild> cra2 = cra;
		cra2->nonconstMethod(); // compile err.
		const A& ca = a;
		Refer cra3 = ca.to(TClass<AChild>());
		cra3.isConst(); // true
		cra3.call(...); // 본래 이렇게 하는게 정상이다.

		//	Refer.isConst()의 의미는 const T* 이다.
		//	const Refer& ref 의 의미도 const T* 이나, 이것은 엄밀히 말하면 Managed 인스턴스를 한번더 const 로 래핑한것과 같다. 왜냐하면 본래 모든 Managed객체는 C++에서는 nonconst로 접근가능하기 때문이다. 따라서 이러한 용법은 native에서 const임을 한번더 못박은거라고 보면 된다.


class Instance : Thing {
	//	Instance는 World에서 인스턴스 관리를 대신해준다. 여기서부터 bind가 가능하다.
	union ID {
		windex type;
		//	클래스의 종류
		windex index;
		//	해당 클래스 인스턴스 벡터의 몇번째인가.type과 index를 조합하면 어디서든 객체에 접근할 수 있다.
		wint64 number;
	} _id;

	Instance() {
		//	TODO: we need to optimize this. this ganna hotspot.
		Nexus::get().getInstancer().stamp(*this);
	}

	virtual TStrong<?> clone() const;
	ID getID() const { return _id; }
	wcount getSerial() const { 
		Nexus::get().getInstancer()[_id].getSerial();
	}
	const InstanceBlock& getBlock() const {
		WRD_IS_THIS(InstanceBlock)
		return Nexus::get().getInstancer()[_id];
	}
	wbool isHeap() const {
		MemoryMap& mmap = Nexus::get().getInstancer().getMemoryMap();
		WRD_IS_NULL(mmap, false)

		return 	mmap.getStartAddress() <= this 	&&
				this << mmap.getEndAddress()	;
	}
}
class Msg : public Thing {
	String _name;
	String& getName() {
		WRD_IS_THIS(String)
		return _name;
	}
	const String& getName() const {
		WRD_IS_THIS(const String)
		return _name;
	}

	Args _args;
	Args& getArgs() {
		WRD_IS_THIS(Args)
		return _args;
	}
	const Args& getArgs() const {
		WRD_IS_THIS(const Args)
		return _args;
	}

	static Object* _thisptr; // Message는_name_thisptr_args를_모두_하나의_Array로_구성한다 
	friend class Object; // _thisptr 조작을 위해
	static Object& getThis() { return *_thisptr; }
	static Result& _setThis(Object& newone) { _thisptr = &newone; }

	static Method* _me;
	friend class Method;
	static const Method& getMe() const { return *_me; }
	static Result& _setMe(Method& newone) { _me = newone; }
}

namespace {
}

class Node : public ? {
	virtual wbool isOccupiable() const { return false;/*default*/ }
	virtual const Container& getMembers() const = 0; // invisible
	Container& _getMember() {
		return const_cast<Container&>(getMember());
	}
	//	getMembers:
	//		모든 Container의 index는 World와 Native모두 1부터 시작한다.
	//		invisible하다.
	Node& operator[](windex n) { return getMember(n); }
	const Node& operator[](windex n) const { return getMember(n); }
	Node& getMember(windex n) {
		WRD_IS_THIS(Node)
		return _getMembers()[n];
	}
	const Node& getMember(windex n) const {
		WRD_IS_THIS(Node)
		return getMembers()[n];
	}
	virtual Refer call(const Msg& msg) { return _call<This>(msg, getMembers()); }
	virtual Refer call(const Msg& msg) const { return _call<const This>(msg, _getMembers()); }
	template <typename T, typename S>
	Refer _call(const Msg& msg, S& members);
}
	//	implement this only inside of cpp file
	template <typaname T, typename S>
	Refer Node::_call(const Msg& msg, S& members) {
		WRD_IS_NULL(msg)
		WRD_IS_NULL(members)

		Refer ret;
		members.template each<T>([&ret, &msg](T& e) {
			if(e.isConst() != TConstChecker<T>::IS)
				continue;
			if(ret = e.call(msg))
				return false; // stop eaching.
			return true;
		});
		return ret;
	}
