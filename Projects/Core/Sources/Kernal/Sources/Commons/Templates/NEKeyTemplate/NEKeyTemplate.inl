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
	void  NEKeyTemplate<T, type>::release() 
	{
		SuperClass::release();

		_value = T();
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



	template <typename T, NEType::Type type>
	NEObject& NE::NEKeyTemplate<T, type>::clone() const
	{
		return *(new ThisClass(*this));
	}
}