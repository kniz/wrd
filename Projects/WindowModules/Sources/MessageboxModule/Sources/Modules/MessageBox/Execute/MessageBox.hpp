#pragma once

#include "../../../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL MessageBox : public NEModule
	{
	public:
		#include "innerclass/ErrorCode/ErrorCode.hpp"

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const;
		virtual LPCTSTR getErrorMessage(type_errorcode errorcode) const;

	public:
		virtual NEObject& clone() const;

	private:
		virtual type_result _onExecute();
		virtual type_result _onFetchModule();
		virtual type_result _onFetchArguments(NEArgumentList& tray);

	private:
		NETArgument<NETStringKey> _message;
		NETArgument<NETStringKey> _title;
	};
}
