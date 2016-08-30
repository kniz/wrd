#pragma once

#include "../../NETList.inl"

namespace NE
{
#define _NAME NETList<T*, true>::Iterator

	NE_DEFINE_INTERFACE_ONLY_2(NE_MACRO_FUNNEL(_NAME), template <typename T>)

	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T>
	_NAME::Iterator(const T* source, typename _NAME* next)
		: _next(next)
	{
		_value = static_cast<T*>( &(source->clone()) );
	}

	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T>
	_NAME::~Iterator()
	{
		if(_value)
			delete _value;
		_value = NE_NULL;
	}

	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T>
	typename const _NAME& _NAME::operator=(typename const _NAME& source)
	{
		_next = source._next;
		return operator=(source._value);
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T>
	typename const _NAME& _NAME::operator=(const T* source)
	{
		if(_value) 
		{
			delete _value;
			_value = NE_NULL;
		}

		_value = static_cast<T*>( &(source->clone()) );

		return *this;
	}

	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T>
	typename T& _NAME::getValue()
	{
		return *_value;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T>
	typename const T& _NAME::getValue() const
	{
		return *_value;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T>
	void _NAME::setValue(const T* const source)
	{
		if(_next)
		{
			delete _next;
			_next = NE_NULL;
		}

		_value = static_cast<T*>( &(source->clone()) );
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T>
	typename const _NAME* _NAME::getNext() const
	{
		return _next;
	}

	template <typename T>
	typename _NAME* _NAME::getNext()
	{
		return _next;
	}

#undef _NAME
}
