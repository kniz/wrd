//	---------------------------------------------------------------------------------
//	클래스명:	NEResult
//	설명	:	인터페이스간의 결과를 알려준다.
//	관계	:	기반 패키지인 FileAccess를 사용한다.
//	특성	:	Kernal 전반에서 사용되는 인터페이스간의 결과를 관리하는 클래스.
//				속도를 위해서 생성자를 만들지 않는다.
//	사용방법:	
//	메모	:	
//	히스토리:	2011-07-18	이태훈	개발 완료.
//	---------------------------------------------------------------------------------
#pragma once

//	include:
#include "define/define.hpp"
#include "../../../Includes/Includes.hpp"

namespace NE
{
	namespace NEResult
	{
#include "innerclass/Level/Level.hpp"

		type_bool NE_DLL hasTrueOnly(type_result result);
		type_bool NE_DLL hasFalseOnly(type_result result);
		type_bool NE_DLL hasTrueAndFalseBoth(type_result result);
		type_bool NE_DLL hasTrue(type_result result);
		type_bool NE_DLL hasFalse(type_result result);
		type_bool NE_DLL hasError(type_result result);
		type_bool NE_DLL hasWarning(type_result result);
		type_bool NE_DLL hasInformation(type_result result);
		type_bool NE_DLL isNotImplemented(type_result result);
		type_bool NE_DLL isWrongParameter(type_result result);
		type_bool NE_DLL isOutOfMemory(type_result result);
		type_bool NE_DLL isActionAborted(type_result result);
		type_bool NE_DLL isCriticalError(type_result result);
		type_bool NE_DLL isAccessDenied(type_result result);
		type_bool NE_DLL isThereNoProperInterface(type_result result);
		type_bool NE_DLL isWrongPointerAddressing(type_result result);
		type_bool NE_DLL isUnExpectedError(type_result result);
		type_bool NE_DLL CouldNotFindData(type_result result);
		void NE_DLL addErrorFlag(type_result& source, type_result target);
		type_errorcode NE_DLL getErrorCode(type_result result);
		type_result NE_DLL setErrorCode(type_result source, type_errorcode target);
	}
}
