#include "NENamedUnit.hpp"
#include "../Kernal/Kernal.hpp"

namespace NE
{
	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NE_DLL NENamedUnit::NENamedUnit(const NECodeType& script_type, const NECodeType& name_type)
		: NEExecutableUnit(), 
		_scriptcodes(NECode(NE_HAS_NO_SCRIPTCODE, script_type)),
		_namecodes(NECode(NE_DEFAULT, name_type))
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NE_DLL NENamedUnit::NENamedUnit(const NENamedUnit& source)
		: NEExecutableUnit(source), _scriptcodes(source._scriptcodes),
		_namecodes(source._namecodes)
	{	
		_assign(source);		
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NE_DLL NENamedUnit::~NENamedUnit()
	{

	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	type_bool NE_DLL NENamedUnit::operator==(const NENamedUnit& source) const
	{
		if(this == &source) return true;
		if(NEExecutableUnit ::operator==(source) == false) return false;
		if(_scriptcodes != source._scriptcodes) return false;
		if(_namecodes != source._namecodes) return false;

		return true;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	type_bool NE_DLL NENamedUnit::operator!=(const NENamedUnit& source) const
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	const NENamedUnit NE_DLL &NENamedUnit::operator=(const NENamedUnit& source)
	{
		NEExecutableUnit ::operator=(source);

		return _assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	void NE_DLL NENamedUnit::release()
	{
		NEExecutableUnit ::release();

		_release();
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NEBinaryFileSaver NE_DLL &NENamedUnit::serialize(NEBinaryFileSaver& saver) const 
	{
		NEExecutableUnit::serialize(saver);

		return saver << _scriptcodes << _namecodes;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NEBinaryFileLoader NE_DLL &NENamedUnit::serialize(NEBinaryFileLoader& loader)
	{
		NEExecutableUnit ::serialize(loader);

		return loader >> _scriptcodes >>_namecodes;
	}



	type_result NE_DLL NENamedUnit::isValid() const
	{
		if(getNameCode() < 0) return RESULT_TYPE_ERROR;
		if(getScriptCode() < NE_HAS_NO_SCRIPTCODE) return RESULT_TYPE_ERROR;

		return RESULT_SUCCESS;
	}




	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	void NENamedUnit::_release()
	{
		//	_scriptcodes.release()는 사용해선 안된다.
		_namecodes.create(1);
		_namecodes.push(NECode(0, NECodeType::NAME));
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	const NENamedUnit& NENamedUnit::_assign(const NENamedUnit& source)
	{
		//	pre:
		if(this == &source) return *this;

		_scriptcodes = source._scriptcodes;
		_namecodes = source._namecodes;

		return *this;
	}

	const NECodeSet NE_DLL &NENamedUnit::getCodes(const NECodeType& type) const
	{
		if(_scriptcodes.getCodeType() == type)
			return _scriptcodes;
		else if(_namecodes.getCodeType() == type)
			return _namecodes;

		NECodeSet* nullpointer = 0x00;
		return *nullpointer;
	}

	type_result NENamedUnit::setCodes(const NECodeSet& source)
	{
		if(source.getLength() <= 0) return RESULT_TYPE_WARNING | RESULT_ABORT_ACTION | RESULT_WRONG_BOUNDARY;
		const NECode& code = source[0];
		const NECodeType& type = code;

		if (_scriptcodes.getCodeType() == type)
		{
			_scriptcodes.pop();
			_scriptcodes.push(code);				
		}			
		else if (_namecodes.getCodeType() == type)
		{
			_namecodes.pop();
			_namecodes.push(code);
		}
		else
			return RESULT_TYPE_WARNING | RESULT_INVALID_DATA;

		return RESULT_SUCCESS;
	}

	NE::type_code NENamedUnit::getNameCode() const
	{
		if (_namecodes.getLength() <= 0)
		{
			KERNAL_WARNING(" : ");
			return NE_HAS_NO_SCRIPTCODE;
		}

		return _namecodes[0].getCode();
	}

	type_result NENamedUnit::setNameCode(type_code new_code)
	{
		return setCodes(NECode(new_code, _namecodes.getCodeType()));
	}

	type_code NENamedUnit::getScriptCode() const
	{
		if (_scriptcodes.getLength() <= 0)
		{
			KERNAL_WARNING(" : ");
			return NE_HAS_NO_SCRIPTCODE;
		}
		return _scriptcodes[0].getCode();
	}

	NECodeSet NE_DLL &NENamedUnit::_getScriptCodes()
	{
		return _scriptcodes;
	}
	NECodeSet NE_DLL &NENamedUnit::_getNameCodes()
	{
		return _namecodes;
	}
}
