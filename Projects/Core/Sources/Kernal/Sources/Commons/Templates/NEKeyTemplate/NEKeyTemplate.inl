namespace NE
{
	template <typename T, NEType::Type type>
	NEKeyTemplate<T, type>::NEKeyTemplate(const T& source, const NETString& keyname)
		: SuperClass(keyname), _value(source)
	{

	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T, NEType::Type type>
	NEKeyTemplate<T, type>::NEKeyTemplate(const NEKeyTemplate<T, type>& source)		
		: SuperClass(source)
	{
		_value = source._value;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T, NEType::Type type>
	NEKeyTemplate<T, type>::NEKeyTemplate(const NEKey& source)
		: SuperClass(source.getName())
	{
		assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T, NEType::Type type>
	NEKeyTemplate<T, type>::~NEKeyTemplate() 
	{
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T, NEType::Type type>
	const NEKeyTemplate<T, type>  & NEKeyTemplate<T, type>::operator=(const NEKeyTemplate<T, type>& source)
	{
		if(this == &source) return *this;

		SuperClass::operator=(source);
		_value = source._value;
		return *this;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T, NEType::Type type>
	NEKeyTemplate<T, type>   NEKeyTemplate<T, type>::operator+(const NEKeyTemplate<T, type>& source) const
	{
		return NEKeyTemplate<T, type>(_value + source._value);
	}	



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T, NEType::Type type>
	const NEKeyTemplate<T, type>  & NEKeyTemplate<T, type>::operator+=(const NEKeyTemplate<T, type>& source)
	{
		_value += source._value;
		return *this;
	}	



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T, NEType::Type type>
	bool   NEKeyTemplate<T, type>::operator==(const NEKeyTemplate<T, type>& source) const
	{
		if(this == &source) return true;

		return (_value == source._value);
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T, NEType::Type type>
	bool NEKeyTemplate<T, type>::operator!=(const NEKeyTemplate<T, type>& source) const
	{
		return !(operator==(source));
	}



	template <typename T, NEType::Type type>
	T& NEKeyTemplate<T, type>::getValue()
	{
		return _value;
	}
	
	template <typename T, NEType::Type type>
	const T& NEKeyTemplate<T, type>::getValue() const
	{
		return _value;	
	}
	
	

	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T, NEType::Type type>
	NEObject& NEKeyTemplate<T, type>::clone() const
	{
		return *(new NEKeyTemplate<T, type>(*this));
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T, NEType::Type type>
	void  NEKeyTemplate<T, type>::release() 
	{
		_value = static_cast<T>(NE_NULL);
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T, NEType::Type type>
	NEBinaryFileSaver& NEKeyTemplate<T, type>::serialize(NEBinaryFileSaver& saver) const  
	{
		SuperClass::serialize(saver);

		return saver << _value;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T, NEType::Type type>
	NEBinaryFileLoader& NEKeyTemplate<T, type>::serialize(NEBinaryFileLoader& loader)
	{
		SuperClass::serialize(loader);

		return loader >> _value;
	}	



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	template <typename T, NEType::Type type>
	NEType::Type NEKeyTemplate<T, type>::getType() const
	{
		return type;
	}



	//	---------------------------------------------------------------------------------
	//	설명	:	NEKey끼리 캐스팅 없이 할당이 가능하게 하는 함수. operator=와 같다.
	//				내부에서 캐스팅을 실시한다.
	//	동작조건:
	//	메모	:
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	/*		
		NEManagedNodeSet과 NEManagedModuleSet은 기본 assign을 사용한다.
	*/	
	template <typename T, NEType::Type type>
	type_result NEKeyTemplate<T, type>::assign(const NEKey& source)
	{
		if( ! &source) return RESULT_TYPE_ERROR | RESULT_WRONG_PARAMETER | RESULT_WRONG_POINTER_ADDRESSING | RESULT_ABORT_ACTION;
		if(this == &source) return RESULT_SUCCESS | RESULT_ABORT_ACTION | RESULT_ABORT_ACTION;
		if(isSubClassOf(NEType::NENODE_SELECTOR_BASE)) return RESULT_TYPE_ERROR | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if( ! isEqualTypeWith(source)) return RESULT_TYPE_ERROR | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;

		SuperClass::assign(source);
		_value = static_cast<const NEKeyTemplate<T, type>&>(source).getValue();

		return RESULT_SUCCESS;
	}
	template <>
	type_result NEKeyTemplate<type_bool, NEType::NEBOOLEAN_KEY>::assign(const NEKey& source);
	template <>
	type_result NEKeyTemplate<type_byte, NEType::NEBYTE_KEY>::assign(const NEKey& source);
	template <>
	type_result NEKeyTemplate<type_ubyte, NEType::NEUBYTE_KEY>::assign(const NEKey& source);
	template <>
	type_result NEKeyTemplate<type_char, NEType::NECHAR_KEY>::assign(const NEKey& source);
	template <>
	type_result NEKeyTemplate<type_wchar, NEType::NEWCHAR_KEY>::assign(const NEKey& source);
	template <>
	type_result NEKeyTemplate<type_short, NEType::NESHORT_KEY>::assign(const NEKey& source);
	template <>
	type_result NEKeyTemplate<type_ushort, NEType::NEUSHORT_KEY>::assign(const NEKey& source);
	template <>
	type_result NEKeyTemplate<type_int, NEType::NEINT_KEY>::assign(const NEKey& source);
	template <>
	type_result NEKeyTemplate<type_uint, NEType::NEUINT_KEY>::assign(const NEKey& source);
	template <>
	type_result NEKeyTemplate<type_float, NEType::NEFLOAT_KEY>::assign(const NEKey& source);
	template <>
	type_result NEKeyTemplate<type_double, NEType::NEDOUBLE_KEY>::assign(const NEKey& source);
	template <>
	type_result NEKeyTemplate<NEString, NEType::NESTRING_KEY>::assign(const NEKey& source);
	template <>
	type_result NEKeyTemplate<NEWString, NEType::NEWSTRING_KEY>::assign(const NEKey& source);
	template <>
	type_result NEKeyTemplate<type_int64, NEType::NEINT64_KEY>::assign(const NEKey& source);
	

	template <typename T, NEType::Type type>
	type_result NEKeyTemplate<T, type>::extend(const NEKey& source)
	{
		//	pre:
		/*
				this == &source의 경우에도 이상없이 진행이 가능한 코드이므로
				따로 검사하지 않는다.
		*/
		//	main:
		//		버퍼 생성:
		NEKeyTemplate<T, type> buffer;
		//		버퍼에 할당:
		type_result result = buffer.assign(source);
		if(NEResult::hasError(result))
			return result;



		//	post:
		_value += buffer._value;
		return RESULT_SUCCESS;
	}

	template <typename T, NEType::Type type>
	bool NEKeyTemplate<T, type>::isEqualTo(const NEKey& source) const
	{
		//	pre:
		if(this == &source) return true;



		//	main:
		//		버퍼 생성:
		NEKeyTemplate<T, type> buffer;
		//		버퍼에 할당:
		type_result result = buffer.assign(source);
		if(NEResult::hasError(result))
			return false;



		//	post:
		return _value == buffer._value;
	}

	template <typename T, NEType::Type type>
	bool NEKeyTemplate<T, type>::isLower(const NEKey& source) const
	{
		//	pre:
		if( ! &source) return false;



		//	main:
		//		버퍼 정의:
		NEKeyTemplate<T, type> buffer;
		//		버퍼에 할당:
		type_result result = buffer.assign(source);
		if(NEResult::hasError(result))
			return false;



		//	post:
		return _value < buffer._value;
	}
	template <>
	bool NEKeyTemplate<NEString, NEType::NESTRING_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEWString, NEType::NEWSTRING_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEBooleanSet, NEType::NEBOOLEAN_SET_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEByteSet, NEType::NEBYTE_SET_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEUByteSet, NEType::NEUBYTE_SET_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEShortSet, NEType::NESHORT_SET_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEUShortSet, NEType::NEUSHORT_SET_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEIntSet, NEType::NEINT_SET_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEUIntSet, NEType::NEUINT_SET_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEInt64Set, NEType::NEINT64_SET_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEFloatSet, NEType::NEFLOAT_SET_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEDoubleSet, NEType::NEDOUBLE_SET_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEStringSet, NEType::NESTRING_SET_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEWStringSet, NEType::NEWSTRING_SET_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NEModuleCodeSet, NEType::NEMODULE_CODESET_KEY>::isLower(const NEKey& source) const;
	template <>
	bool NEKeyTemplate<NENodeCodeSet, NEType::NENODE_CODESET_KEY>::isLower(const NEKey& source) const;	
}