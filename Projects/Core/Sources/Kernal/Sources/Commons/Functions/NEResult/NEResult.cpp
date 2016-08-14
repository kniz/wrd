#include "NEResult.hpp"

namespace NE
{
	namespace NEResult
	{
		type_bool NE_DLL hasTrueOnly(type_result result)
		{
			return (result & RESULT_TRUE) != RESULT_FALSE && (result & RESULT_FALSE) == NE_FALSE;
		}
		type_bool NE_DLL hasFalseOnly(type_result result)
		{
			return (result & RESULT_TRUE) == NE_FALSE && (result & RESULT_FALSE) != NE_FALSE;
		}
		type_bool NE_DLL hasTrueAndFalseBoth(type_result result)
		{
			return (result & RESULT_TRUE) != NE_FALSE && (result & RESULT_FALSE) != NE_FALSE;
		}
		type_bool NE_DLL hasTrue(type_result result)
		{
			return (result & RESULT_TRUE) != NE_FALSE;
		}
		type_bool NE_DLL hasFalse(type_result result)
		{
			return (result & RESULT_FALSE) != NE_FALSE;
		}
		type_bool NE_DLL hasError(type_result result)
		{
			/*
				이렇게 한데 묶어 놓으면 릴리즈모드에서 컴파일 최적화가 되지 않을까.
				아무튼, 저중에서 하나라도 존재한다면 에러 판정이 된다.
			*/
			return (result & RESULT_TYPE_ERROR) != 0;
		}
		type_bool NE_DLL hasWarning(type_result result)
		{
			return (result & RESULT_TYPE_WARNING) != 0;
		}
		type_bool NE_DLL hasInformation(type_result result)
		{
			return (result & RESULT_TYPE_INFORMATION) != 0;
		}
		type_bool NE_DLL isNotImplemented(type_result result)
		{
			return (result & RESULT_NOT_IMPLEMENTED) != 0;
		}
		type_bool NE_DLL isWrongParameter(type_result result)
		{
			return (result & RESULT_WRONG_PARAMETER) != 0;
		}
		type_bool NE_DLL isOutOfMemory(type_result result)
		{
			return (result & RESULT_OUT_OF_MEMORY) != 0;
		}
		type_bool NE_DLL isActionAborted(type_result result)
		{
			return (result & RESULT_ABORT_ACTION) != 0;
		}
		type_bool NE_DLL isCriticalError(type_result result)
		{
			return (result & RESULT_CRITICAL_ERROR) != 0;
		}
		type_bool NE_DLL isAccessDenied(type_result result)
		{
			return (result & RESULT_ACCESS_DENIED) != 0;
		}
		type_bool NE_DLL isWrongBoundary(type_result result)
		{
			return (result & RESULT_WRONG_BOUNDARY) != 0;
		}
		type_bool NE_DLL isWrongPointerAddressing(type_result result)
		{
			return (result & RESULT_WRONG_POINTER_ADDRESSING) != 0;
		}
		type_bool NE_DLL isUnExpectedError(type_result result)
		{
			return (result & RESULT_UNEXPECTED_ERROR) != 0;
		}
		type_bool NE_DLL CouldNotFindData(type_result result)
		{
			return (result & RESULT_COULDNOT_FIND_DATA) != 0;
		}
		type_bool NE_DLL CouldNotFindFile(type_result result)
		{
			return (result & RESULT_COULDNOT_FIND_FILE) != 0;
		}
		type_bool NE_DLL isInvalidData(type_result result)
		{
			return (result & RESULT_INVALID_DATA) != 0;
		}
		Level NE_DLL getErrorType(type_result result)
		{
			if(result & RESULT_TYPE_ERROR) 
				return LEVEL_ERROR;
			else if(result & RESULT_TYPE_WARNING)
				return LEVEL_WARNING;
			else if(result & RESULT_TYPE_INFORMATION)
				return LEVEL_INFORMATION;

			return LEVEL_UNDEFINED;
		}
		type_errorcode NE_DLL getErrorCode(type_result result)
		{
			return static_cast<type_errorcode>(result);
		}
		type_result NE_DLL setErrorCode(type_result source, type_errorcode target)
		{
			type_result assigned = target;
			addErrorFlag(assigned, source);

			return assigned;
		}
		void NE_DLL addErrorFlag(type_result& source, type_result target)
		{	
			if(target & RESULT_NOT_IMPLEMENTED)
				source = source | RESULT_NOT_IMPLEMENTED;
			if(target & RESULT_WRONG_PARAMETER)
				source = source | RESULT_WRONG_PARAMETER;
			if(target & RESULT_OUT_OF_MEMORY)
				source = source | RESULT_OUT_OF_MEMORY;
			if(target & RESULT_ABORT_ACTION)
				source = source | RESULT_ABORT_ACTION;
			if(target & RESULT_CRITICAL_ERROR)
				source = source | RESULT_CRITICAL_ERROR;
			if(target & RESULT_ACCESS_DENIED)
				source = source | RESULT_ACCESS_DENIED;
			if(target & RESULT_WRONG_BOUNDARY)
				source = source | RESULT_WRONG_BOUNDARY;
			if(target & RESULT_WRONG_POINTER_ADDRESSING)
				source = source | RESULT_WRONG_POINTER_ADDRESSING;
			if(target & RESULT_UNEXPECTED_ERROR)
				source = source | RESULT_UNEXPECTED_ERROR;
			if(target & RESULT_COULDNOT_FIND_DATA)
				source = source | RESULT_COULDNOT_FIND_DATA;
			if(target & RESULT_COULDNOT_FIND_FILE)
				source = source | RESULT_COULDNOT_FIND_FILE;
			if(target & RESULT_INVALID_DATA)
				source = source | RESULT_INVALID_DATA;
			if(target & RESULT_TYPE_INFORMATION)
				source = source | RESULT_TYPE_INFORMATION;
			if(target & RESULT_TYPE_WARNING)
				source = source | RESULT_TYPE_WARNING;
			if(target & RESULT_TYPE_ERROR)
				source = source | RESULT_TYPE_ERROR;
		}

	}
}
