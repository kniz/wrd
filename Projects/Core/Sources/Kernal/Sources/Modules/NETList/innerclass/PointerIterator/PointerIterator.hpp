#pragma once

#include "../../NETList.inl"

namespace NE
{
#define _NAME	NETList<T*, false>::Iterator

	NE_DEFINE_INTERFACE_ONLY_2(NE_MACRO_FUNNEL(_NAME), 
		template <typename T>)

	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T>
	_NAME::Iterator(const T* source, typename _NAME* next)
		: _next(next)
	{
		T* const_casted_source = const_cast<T*>(source);
		_value = const_casted_source;
	}

	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T>
	_NAME::~Iterator()
	{

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
	void _NAME::setValue(T* source)
	{
		_value = source;
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