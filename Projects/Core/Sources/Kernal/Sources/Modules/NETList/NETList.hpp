#pragma once

#include "../NETClass/NETClass.hpp"
#include "../NETCollector/NETCollector.hpp"
#include "NETList.inl"
#include "innerclass/Iterator/Iterator.hpp"
#include "innerclass/PointerIterator/PointerIterator.hpp"
#include "NETListWithBinder.hpp"
#include "innerclass/PointerIteratorWithPolyMorphism/PointerIteratorWithPolyMorphism.hpp"

namespace NE
{
#define _TEMPLATE	template <typename T, type_bool pointerUseNewInstance>
#define _NAME		NETList<T, ptrAsHeap>::Node

	NE_DEFINE_CLASS_ONLY_2(NE_MACRO_FUNNEL(_NAME), NE_MACRO_FUNNEL(_TEMPLATE))

	_TEMPLATE
	type_bool _NAME::operator==(const This& rhs) const
	{
		return	_value == rhs._value	&&
				_next == rhs._next		&&
				_back == rhs._back		;
	}

	_TEMPLATE
	type_bool _NAME::operator!=(const This& rhs) const
	{
		return ! operator==(rhs);
	}

	_TEMPLATE
	type_result _NAME::isValid() const
	{
		if(	! _value.isBinded()					&&
			_back.isBinded() && _next.isBinded())
			return RESULT_TYPE_WARNING;

		return RESULT_SUCCESS;
	}

	_TEMPLATE
	void _NAME::release()
	{
		Super::release();

		_value.release();
		_next.release();
		_back.release();
	}

	_TEMPLATE
	NEBinaryFileSaver& _NAME::serialize(NEBinaryFileSaver& saver) const
	{
		return saver << _value << _next << _back;
	}

	_TEMPLATE
	NEBinaryFileLoader& _NAME::serialize(NEBinaryFileLoader& loader)
	{
		return loader >> _value >> _next >> _back;
	}

	_TEMPLATE
	NETBinder<T>& _NAME::getValueBinder()
	{
		return _value;
	}

	_TEMPLATE
	const NETBinder<T>& _NAME::getValueBinder() const
	{
		return _value;
	}

	_TEMPLATE
	NETBinder<Node>& _NAME::getNextBinder()
	{
		return _next;
	}

	_TEMPLATE
	const NETBinder<Node>& _NAME::getNextBinder() const
	{
		return _next;
	}

	_TEMPLATE
	NETBinder<Node>& _NAME::getBackBinder()
	{
		return _back;
	}

	_TEMPLATE
	const NETBinder<Node>& _NAME::getBackBinder() const
	{
		return _back;
	}

#undef _NAME	
#define _NAME	NETList<T, ptrAsHeap>::Iterator

	NE_DEFINE_CLASS_ONLY_2(NE_MACRO_FUNNEL(_NAME), NE_MACRO_FUNNEL(_TEMPLATE))

	_TEMPLATE
	_NAME::Iterator(const _This& owner, Node& new_cursor)
		: Super(), _owner(owner), _cursor(new_cursor)
	{

	}			

	_TEMPLATE
	type_bool _NAME::operator==(const This& rhs) const
	{
		//	When these binders are designating same object each other,
		//	we'll return true.
		return	_owner.get() == rhs._owner.get()	&&
				_cursor.get() == rhs._cursor.get()	;
	}

	_TEMPLATE
	const T& _NAME::get() const
	{
		const T* nullpointer = NE_NULL;
		if( ! _cursor.isBinded())
			return *nullpointer;

		return *_cursor->getValueBinder();
	}

	_TEMPLATE
	T& _NAME::get()
	{
		T* nullpointer = NE_NULL;
		if( ! _cursor)
			return nullpointer;

		return *_cursor->getValueBinder();
	}
	
	_TEMPLATE
	type_bool _NAME::isEnd() const
	{
		if( ! _cursor.isBinded())
			//	Oops. Now, we got trouble.
			return false;

		return ! _cursor->getNextBinder().isBinded();
	}
	
	_TEMPLATE
	type_result _NAME::next(type_index step_for_next)
	{
		for(int n=0; n < step_for_next ;n++)
		{
			if(isEnd())
				return RESULT_TYPE_WARNING;

			_cursor.bind(*_cursor->getNextBinder());
		}
		return RESULT_SUCCESS;
	}

	_TEMPLATE
	type_result _NAME::back(type_index step_for_back)
	{
		for(int n=0; n < step_for_back ;n++)
		{
			if(isEnd())
				return RESULT_TYPE_WARNING;

			_cursor.bind(*_cursor->getBackBinder());
		}
		return RESULT_SUCCESS;
	}
	
	_TEMPLATE
	type_index _NAME::getLength() const
	{
		return _owner.isBinded() ? _owner->getLength() : 0;
	}
	
	_TEMPLATE
	void _NAME::release()
	{
		_owner.release();
		_cursor.release();
	}

	_TEMPLATE
	NEBinaryFileSaver& _NAME::serialize(NEBinaryFileSaver& saver) const
	{
		return saver << _owner << _cursor;
	}
	
	_TEMPLATE
	NEBinaryFileLoader& _NAME::serialize(NEBinaryFileLoader& loader)
	{
		return loader >> _owner >> _cursor;
	}
	
	_TEMPLATE
	type_result _NAME::isValid() const
	{
		if( ! _owner.isBinded()) return RESULT_TYPE_ERROR;
		if( ! _cursor.isBinded()) return RESULT_TYPE_ERROR;
		if(	NEResult::hasError(_owner.isValid())	|| 
			NEResult::hasError(_cursor.isValid())	)
			return RESULT_TYPE_WARNING;		
		if(_cursor->getNextBinder().isBinded() && ! _cursor->getValueBinder().isBinded())
			return RESULT_TYPE_ERROR;

		return RESULT_SUCCESS;
	}

	_TEMPLATE
	NETWeakBinder<_This>& _NAME::getOwner()
	{
		return _owner;
	}

	_TEMPLATE
	const NETWeakBinder<_This>& _NAME::getOwner() const
	{
		return _owner;
	}

	_TEMPLATE
	NETWeakBinder<Node>& _NAME::getCursor()
	{
		return _cursor;
	}

	_TEMPLATE
	const NETWeakBinder<Node>& _NAME::getCursor() const
	{
		return _cursor;
	}

#undef _NAME	
#define _NAME	NETList<T, ptrAsHeap>::CIterator

	NE_DEFINE_CLASS_ONLY_2(NE_MACRO_FUNNEL(_NAME), NE_MACRO_FUNNEL(_TEMPLATE))

	_TEMPLATE
	_NAME::CIterator(const _This& owner, Node& new_cursor)
		: Super(), _owner(owner), _cursor(new_cursor)
	{

	}			

	_TEMPLATE
	type_bool _NAME::operator==(const This& rhs) const
	{
		//	When these binders are designating same object each other,
		//	we'll return true.
		return	_owner.get() == rhs._owner.get()	&&
				_cursor.get() == rhs._cursor.get()	;
	}

	_TEMPLATE
	const T& _NAME::get() const
	{
		const T* nullpointer = NE_NULL;
		if( ! _cursor.isBinded())
			return *nullpointer;

		return *_cursor->getValueBinder();
	}

	_TEMPLATE
	type_bool _NAME::isEnd() const
	{
		if( ! _cursor.isBinded())
			//	Oops. we got trouble.
			return false;

		return ! _cursor->getNextBinder().isBinded();
	}

	_TEMPLATE
	type_result _NAME::next(type_index step_for_next)
	{
		for(int n=0; n < step_for_next ;n++)
		{
			if(isEnd())
				return RESULT_TYPE_WARNING;

			_cursor.bind(*_cursor->getNextBinder());
		}
		return RESULT_SUCCESS;
	}

	_TEMPLATE
	type_result _NAME::back(type_index step_for_back)
	{
		for(int n=0; n < step_for_back ;n++)
		{
			if(isEnd())
				return RESULT_TYPE_WARNING;

			_cursor.bind(*_cursor->getBackBinder());
		}
		return RESULT_SUCCESS;
	}

	_TEMPLATE
	type_index _NAME::getLength() const
	{
		return _owner.isBinded() ? _owner->getLength() : 0;
	}

	_TEMPLATE
	void _NAME::release()
	{
		_owner.release();
		_cursor.release();
	}

	_TEMPLATE
	NEBinaryFileSaver& _NAME::serialize(NEBinaryFileSaver& saver) const
	{
		return saver << _owner << _cursor;
	}

	_TEMPLATE
	NEBinaryFileLoader& _NAME::serialize(NEBinaryFileLoader& loader)
	{
		return loader >> _owner >> _cursor;
	}

	_TEMPLATE
	type_result _NAME::isValid() const
	{
		if( ! _owner.isBinded()) return RESULT_TYPE_ERROR;
		if( ! _cursor.isBinded()) return RESULT_TYPE_ERROR;
		if(	NEResult::hasError(_owner.isValid())	|| 
			NEResult::hasError(_cursor.isValid())	)
			return RESULT_TYPE_WARNING;		
		if(_cursor->getNextBinder().isBinded() && ! _cursor->getValueBinder().isBinded())
			return RESULT_TYPE_ERROR;

		return RESULT_SUCCESS;
	}

	_TEMPLATE
	NETWeakBinder<_This>& _NAME::getOwner()
	{
		return _owner;
	}

	_TEMPLATE
	const NETWeakBinder<_This>& _NAME::getOwner() const
	{
		return _owner;
	}

	_TEMPLATE
	NETWeakBinder<Node>& _NAME::getCursor()
	{
		return _cursor;
	}

	_TEMPLATE
	const NETWeakBinder<Node>& _NAME::getCursor() const
	{
		return _cursor;
	}

#undef _NAME
#define _NAME	NETList<T, ptrAsHeap>

	NE_DEFINE_CLASS_ONLY_2(NE_MACRO_FUNNEL(_NAME), NE_MACRO_FUNNEL(_TEMPLATE))

	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	_NAME::NETList()
		:Super()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	_NAME::NETList(const This& source)
		:Super()
	{
		_assign(source);
	}	



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	_NAME::~NETList()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	const _NAME& _NAME::operator=(const This& source) 
	{
		if(this == &rhs) return *this;

		//	Don't need to call Super. length will be synchronized automatically.

		return _assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	_NAME _NAME::operator+(const This& source) const
	{
		//	pre:
		//		파라메터 검사:
		if(source._length <= 0)	return This(*this);



		//	main:
		//		복사:
		//			임시 버퍼 생성:
		This buffer(*this);
		//			버퍼에 데이터 축적:		
		buffer.push(source);



		// post:
		return buffer;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	type_bool  _NAME::operator==(const This& source) const
	{
		//	pre:
		if(this == &source) return true;
		if(NETCollector<T>::operator==(source) == false) return false;


		//	main:
		NETIterator<T>	my_e = getIterator(),
						their_e = getIterator();
		for(; ! my_e.isEnd() ;my_e++, their_e++)
			if(	their_e.isEnd()	||
				*my_e != *their_e	)	
				return false;

		
		//	post:
		return true;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	type_bool  _NAME::operator!=(const This& source) const
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//				2011-11-20	이태훈	버그 수정	source가 *this일 때 발생하게될 오류 대처 추가
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	const _NAME& _NAME::operator+=(const This& source)
	{
		if(source._length == NE_NULL) return *this;

		push(source);

		return *this;	
	}


	_TEMPLATE
	NETBinder<NETIterator<T>> _NAME::getIterator(type_index index)
	{
		Iterator iterator(this, _head);
		iterator.next(index);

		//TODO:
		return NETBinder<NETIterator<T>>(iterator.clone());	//	new heap alloc at inside.
	}

	_TEMPLATE
	NETBinder<NETCIterator<T>> _NAME::getIterator(type_index index) const
	{
		CIterator iterator(this, _head);
		iterator.next(index);

		//TODO:
		return NETBinder<NETCIterator<T>>(iterator.clone());	//	new heap alloc at inside.
	}

	//	---------------------------------------------------------------------------------
	//	설명	:	데이터를 임의의 인덱스에 삽입한다.	
	//	동작조건:	중간에 값을 삽입. 길이 == 크기 일경우, 전체 크기가 + 1 증가한다.
	//	메모	:	만약 5에 데이터를 삽입하면, 5에 있던 데이터는 6으로 밀려난다.
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------	
	_TEMPLATE
	type_index _NAME::insert(const NETCIterator<T>& where, const T& source) 
	{
		//	pre:
		//		파라메터 검사:
		CIterator& wh = cast<CIterator>(where);
		if( ! &wh || ! &source)	return NE_INDEX_ERROR;
		if( &wh->getOwner() != this) return NE_INDEX_ERROR;
		//		Iterator valid check:
		if(NEResult::hasError(wh.isValid()))
			return NE_INDEX_ERROR;
		

		//	main:
		Node& next = *wh->getCursor(),
			& previous = *next.getBackBinder();
		Node node = *(new Node());
		node.getValueBinder().bind(source.clone());
		node.getBackBinder().bind(previous);
		node.getNextBinder().bind(next);
		
		NETBinder<Node>& previous_binder = &previous ? 
			previous.getNextBinder() : _head;
		previous_binder.getNextBinder().bind(node);
		next.getBackBinder().bind(node);


		//	post:
		//		길이 수정:	유닛을 추가했으므로, 길이 정보도 동기화 시킨다
		_length++;
		return index;
	}



	//	---------------------------------------------------------------------------------
	//	설명	:	임의의 인덱스에 위치한 데이터를 하나 삭제한다.
	//	동작조건:	지정한 인덱스가 최대 인덱스보다 작고, 0 이상이다.
	//	메모	:
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	type_index _NAME::remove(type_index index) 
	{
		//	pre:
		//		파라메터검사:
		if(index < 0) return NE_INDEX_ERROR;
		if(index >= _length) return NE_INDEX_ERROR;



		//	main:
		//		조건1:	처음 유닛을 삭제하려 한다면
		if(index == 0)
		{	//		_head의 next를 이용:
			Iterator* temp = _head;

			_head = _head->_next;


			delete temp;
		}
		else
		{	//		중간 유닛 삭제:	삭제할 위치의 전 Iterator의 next를 이용한다
			//			인덱스 변수 생성:
			type_index _index = 0;
			//			인덱스 검색:
			for(Iterator* iterator=_head; iterator ;iterator=iterator->_next)
			{	//		조건1-1:	인덱스를 찾았다면 (= 리스트는 한발 뒤에서 조작한다 )
				if(_index == (index - 1))
				{	//		삭제:
					Iterator* temp = iterator->_next;

					iterator->_next = iterator->_next->_next;

					delete temp;
					break;
				}

				_index++;

			}
		}



		//	post:		
		//		길이 수정:	유닛은 이미 삭제되었으므로, 길이정보만 수정한다
		_length--;
		return index;
	}



	//	---------------------------------------------------------------------------------
	//	설명	:	주어진 데이터와 일치하는 엘리먼트가 존재하는지 검색후, 인덱스를 전달
	//				한다.
	//	동작조건:	템플릿 인자 T가 operator==가 정의되어 있어야 한다.
	//	메모	:
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	type_index _NAME::match(const T& source) const
	{
		//	pre:
		//		관련 멤버변수 검사:
		if(_length <= 0) return NE_INDEX_ERROR;



		//	main:
		//		검색:		
		type_index index = 0;

		for(Iterator* iterator=_head; iterator ;iterator=iterator->_next)
		{
			if(iterator->getValue() == source) return index; // 찾았다

			index++;
		}



		//	post:
		//		에러반환:	못찾았다
		return NE_INDEX_ERROR; // -1
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	type_result _NAME::isValid() const  
	{
		//	pre:
		//		상위클래스 VALID 체크:
		type_result result =Super::isValid();
		if(NEResult::hasError(result) == true) return result;
		//		멤버변수 체크:
		if (_length != _getUnitCount()) return RESULT_TYPE_WARNING | RESULT_WRONG_BOUNDARY | RESULT_INVALID_DATA;



		//	post:
		return true;
	}			



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	void  _NAME::release()  
	{			
		_release();

		return Super::release();
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	NEBinaryFileSaver  &_NAME::serialize(NEBinaryFileSaver& saver) const 
	{	
		//	pre:
		NETCollector::serialize(saver);

		//	main:
		//		엘리먼트 저장:
		for(const Iterator* iterator = _head; iterator ;iterator=iterator->_next)
			saver << iterator->getValue();



		//	post:
		return saver;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	NEBinaryFileLoader& _NAME::serialize(NEBinaryFileLoader& loader)
	{
		//	pre:		
		This::release();
		NETCollector::serialize(loader);
		type_index count = _length;	
		NETCollector::release();



		//	main:
		//		길이 정보 초기화:		
		//		엘리먼트 로드:		
		for(type_index n=0; n < count ;n++)
		{
			T element = T(); // 그냥 T element()를 한다면 함수의 선언으로 컴파일러가 착각한다.

			loader >> element;
			push(element);
		}



		//	post:
		return loader;
	}



	//	---------------------------------------------------------------------------------
	//	설명	:	NETList의 중첩 클래스인, Iterator을 외부에 내보낸다. Iterator에는 다음 데이터를
	//				가리키는 next가 존재한다.
	//	동작조건:
	//	메모	:
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	typename _NAME::Iterator* _NAME::getIterator(type_index index)
	{
		//	pre:
		//		파라메터검사:
		if(index < 0) return NE_NULL;
		if(index > getLengthLastIndex()) return NE_NULL;
		//		관련  멤버변수 검사:
		if(_length <= 0) return NE_NULL;
		if(!_head) return NE_NULL;



		//	main:
		//		검색:
		Iterator* pointer = _head;

		for(type_index n=0; n < index ;n++)
		{
			pointer = pointer->_next;

			if(!pointer) return NE_NULL; // 인덱스에 도달하기도 전에, 끝에 도달했다
		}



		//	post:
		return pointer;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	const typename _NAME::Iterator*	_NAME::getIterator(type_index index) const
	{	
		//	pre:
		//		파라메터 검사:
		if(index < 0) return NE_NULL;
		if(index > getLengthLastIndex()) return NE_NULL;
		//		관련 멤버변수 검사:
		if(_length <= 0) return NE_NULL;
		if(!_head) return NE_NULL;



		//	main:
		Iterator* pointer = _head;

		for(type_index n=0; n < index ;n++)
		{
			pointer = pointer->_next;

			if(!pointer) return NE_NULL; // 인덱스에 도달하기도 전에 끝에 도달했다
		}



		//	post:
		return pointer;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	const _NAME& _NAME::_assign(const This& source)
	{	
		//	pre:
		release();
		//	main:
		push(source);
		//	post:
		return *this;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	void  _NAME::_release() 
	{	
		//	pre:
		if(!_head) return;



		//	main:
		type_count length = getLength(); // pop을 하면 _length가 하나씩 줄어들므로, for문 조건식에서 오작동을 일으킨다. 그러므로 미리 size를 가져옴

		for(type_index n=0; n < length ;n++)
			pop();
	}		



	//	---------------------------------------------------------------------------------
	//	설명	:	몇 개의 유닛이 존재하는지 셈한다.
	//	동작조건:
	//	메모	:
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	type_count _NAME::_getUnitCount() const
	{
		//	main:
		type_count count = 0;

		for(Iterator* iterator = _head; iterator ;iterator = iterator->_next)
			count++;				



		//	post:
		return count;
	}



	//	---------------------------------------------------------------------------------
	//	설명	:	맨 마지막 바로 앞의 Iterator에 접근한다.
	//	동작조건:
	//	메모	:	LinkedList의 특성상, 바로 앞의 Iterator이 삭제/삽입을 할 수 있기 때문이다.
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	typename _NAME::Iterator* _NAME::_getBeforeTail()
	{
		//	pre:				
		if(_length <= 0) return NE_NULL;
		if(_length == 1) return NE_NULL;



		//	post:
		return getIterator(getLengthLastIndex() - 1);
	}



	_TEMPLATE
	type_index _NAME::push(const T& source)
	{
		return insert(getLength(), source);
	}



	_TEMPLATE
	type_index _NAME::pop()
	{
		return remove(getLengthLastIndex());
	}



	_TEMPLATE
	type_index _NAME::pushFront(const T& source)
	{
		return insert(0, source);
	}



	_TEMPLATE
	type_index _NAME::popFront()
	{
		return remove(0);
	}

#undef _NAME
#undef _TEMPLATE
}