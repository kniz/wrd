#include "../../NEExportable.hpp"

namespace NE
{
	//	---------------------------------------------------------------------------------	
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NE_DLL NEExportable::Header::Header()
		: Identifier()
	{
		release();
	}



	//	---------------------------------------------------------------------------------	
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NE_DLL NEExportable::Header::Header(const Header& source)
		: Identifier(source)
	{
		_assign(source);
	}



	//	---------------------------------------------------------------------------------	
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NE_DLL NEExportable::Header::~Header()
	{

	}

	

	//	---------------------------------------------------------------------------------	
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	const NEExportable::Header NE_DLL &NEExportable::Header::operator=(const Header& source)
	{
		NEExportable::Identifier::operator=(source);

		return _assign(source);
	}



	//	---------------------------------------------------------------------------------	
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	type_bool NE_DLL NEExportable::Header::operator==(const Header& source) const
	{
		if(this == &source) return true;
		if(NEExportable::Identifier::operator!=(source)) return false;
		if(_release_date != source._release_date) return false;
		if(_version != source._version) return false;
		if(_comment != source._comment) return false;
		if(_contact!= source._contact) return false;

		return true;
	}



	//	---------------------------------------------------------------------------------	
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	type_bool NE_DLL NEExportable::Header::operator!=(const Header& source) const
	{
		return !(operator==(source));
	}

	

	//	---------------------------------------------------------------------------------	
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	void NE_DLL NEExportable::Header::release()
	{
		NEExportable::Identifier::release();
		_release_date.release();
		_version.release();
		_comment.release();
		_contact.release();
	}



	//	---------------------------------------------------------------------------------	
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	type_result NE_DLL NEExportable::Header::isValid() const
	{
		type_result result = NEExportable::Identifier::isValid();
		if(NEResult::hasError(result) == true) return result;
		result = _release_date.isValid();
		if(NEResult::hasError(result) == true) return result;
		result = _version.isValid();
		if(NEResult::hasError(result) == true) return result;
		result = _comment.isValid();
		if(NEResult::hasError(result) == true) return result;
		result = _contact.isValid();
		if(NEResult::hasError(result) == true) return result;

		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------	
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NEBinaryFileSaver NE_DLL &NEExportable::Header::serialize(NEBinaryFileSaver& saver) const
	{
		NEExportable::Identifier::serialize(saver);

		return saver << _release_date << _version << _comment << _contact;
	}



	//	---------------------------------------------------------------------------------	
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NEBinaryFileLoader NE_DLL &NEExportable::Header::serialize(NEBinaryFileLoader& loader)
	{
		NEExportable::Identifier::serialize(loader);

		return loader >> _release_date >> _version >> _comment >> _contact;
	}



	//	---------------------------------------------------------------------------------	
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	const NEExportable::Header& NEExportable::Header::_assign(const Header& source)
	{
		if(this == &source) return *this;
		
		_release_date = source._release_date;
		_version = source._version;
		_comment = source._comment;
		_contact = source._contact;

		return *this;
	}
	
	const NETString NE_DLL &NEExportable::Header::getContact() const
	{
		return _contact;
	}

	NETString NE_DLL &NEExportable::Header::getContact()
	{
		return _contact;
	}

	const NETString NE_DLL &NEExportable::Header::getComment() const
	{
		return _comment;
	}

	NETString NE_DLL &NEExportable::Header::getComment()
	{
		return _comment;
	}

	const NETString NE_DLL &NEExportable::Header::getVersion() const
	{
		return _version;
	}

	NETString NE_DLL &NEExportable::Header::getVersion()
	{
		return _version;
	}

	const NETString NE_DLL &NEExportable::Header::getReleaseDate() const
	{
		return _release_date;
	}

	NETString NE_DLL &NEExportable::Header::getReleaseDate()
	{
		return _release_date;
	}
}
