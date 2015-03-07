#include "NENodeSelectorBase.hpp"
#include "../Kernal/Kernal.hpp"

namespace NE
{
	NE_DLL NENodeSelectorBase::NENodeSelectorBase(const NETString& keyname)
		: SuperClass(keyname)
	{
		_release();
		_codes.getCodeType().setCodeType(NECodeType::ME);
	}

	NE_DLL NENodeSelectorBase::NENodeSelectorBase(const ThisClass& source)
		: SuperClass(source), _codes(NECodeType(NECodeType::ME, false))
	{
		_assign(source);
	}

	NE_DLL NENodeSelectorBase::~NENodeSelectorBase()
	{

	}

	const NENodeSelectorBase NE_DLL &NENodeSelectorBase::operator=(const ThisClass& source)
	{
		return _assign(source);
	}

	bool NE_DLL NENodeSelectorBase::operator==(const ThisClass& source) const
	{
		if(this == &source) return true;
		if(_use_and_operation != source._use_and_operation) return false;
		if(_codes != source._codes) return false;
		if(_count != source._count) return false;
		if(_countlimit != source._countlimit) return false;

		return true;
	}

	bool NE_DLL NENodeSelectorBase::operator!=(const ThisClass& source) const
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-08-04	이태훈	작성
	//	---------------------------------------------------------------------------------
	type_result NE_DLL NENodeSelectorBase::setCodes(const NECodeSet& new_codeset)
	{
		_codes = new_codeset;
		return initializeReferingPoint();
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-08-04	이태훈	작성
	//	---------------------------------------------------------------------------------
	const NECodeSet NE_DLL &NENodeSelectorBase::getCodes() const
	{
		return _codes;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-08-12	이태훈	작성
	//	---------------------------------------------------------------------------------
	type_count NE_DLL NENodeSelectorBase::getCountLimit() const
	{
		return _countlimit;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	void NE_DLL NENodeSelectorBase::setCountLimit(type_count limit)
	{
		if(limit < 0) return;

		_countlimit = limit;
		initializeReferingPoint();
	}


	bool NE_DLL NENodeSelectorBase::isUsingAndOperation() const
	{
		return _use_and_operation;
	}



	void NE_DLL NENodeSelectorBase::setUsingAndOperation(bool enable)
	{
		_use_and_operation = enable;
		initializeReferingPoint();
	}



	void NE_DLL NENodeSelectorBase::release()
	{
		return _release();
	}

	type_result NE_DLL NENodeSelectorBase::isValid() const
	{
		type_result result = _codes.isValid();		
		if(NEResult::hasError(result) == true) return result;
		if(	NEResult::hasError(result = _codes.isValid())	|| 
			_codes.getCodeType() == NECodeType::UNDEFINED	) 
			return RESULT_TYPE_ERROR | RESULT_INVALID_DATA;

		return RESULT_SUCCESS;
	}

	NEBinaryFileSaver NE_DLL &NENodeSelectorBase::serialize(NEBinaryFileSaver& saver) const
	{
		NEKey::serialize(saver);

		return saver << _use_and_operation << _codes << _count << _countlimit;
	}


	NEBinaryFileLoader NE_DLL &NENodeSelectorBase::serialize(NEBinaryFileLoader& loader)
	{
		NEKey::serialize(loader);

		return loader >> _use_and_operation >> _codes >> _count >> _countlimit;
	}

	void NE_DLL NENodeSelectorBase::_release()
	{
		_use_and_operation = false;
		_count = 0;
		_countlimit = 0;
		_codes.release();
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	const NENodeSelectorBase& NENodeSelectorBase::_assign(const ThisClass& source)
	{	//	pre:
		if(this == &source) return *this;

		//	main:
		_use_and_operation = source._use_and_operation;
		_count = source._count;
		_countlimit = source._countlimit;
		_codes = source._codes;

		return *this;
	}

	const NECodeType& NENodeSelectorBase::getCodesType() const
	{
		return getCodes().getCodeType();
	}

	type_result NENodeSelectorBase::setCodesType(const NECodeType& new_one)
	{
		type_result result = _codes.getCodeType().setCodeType(new_one.getCodeType());

		return result |= initializeReferingPoint();
	}
}