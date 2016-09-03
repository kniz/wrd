#pragma once

#include "../NEUnit/NEUnit.hpp"

namespace NE
{
	class NEIterator : public NEUnit
	{
		NE_DECLARE_INTERFACE_ONLY(Iterator, NEAdam)

			//	프렌드 선언:
			friend class NETList;

		//	생성자:
	public:
		Iterator(const T& source, Iterator* next);

	public:
		~Iterator();

		//	접근자:
	public:
		T& getValue();
		const T& getValue() const;
		void setValue(const T& source);
		const Iterator* getNext() const;
		Iterator* getNext();

		//	멤버변수:	
	protected:	
		T _value;
		Iterator* _next;
	};
}