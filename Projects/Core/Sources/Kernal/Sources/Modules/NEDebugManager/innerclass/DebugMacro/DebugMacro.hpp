//	---------------------------------------------------------------------------------
//	파일명	:	DebugMacro.hpp
//	설명	:	NEDebugManger를 이용해서 디버그 메세지를 출력한다.
//	관계	:	선언되야 하는 헤더.	#include "Kernal.hpp"
//	사용방법:	모든 메세지는 printf 타입의 문자열을 사용할 수 있다. 
//				또한 디버그모드에서만 컴파일되는 매크로도 있는 반면, 항상 출력되는
//				매크로 또한 존재한다.
//	메모	:	
//	히스토리:	2011-05-29	이태훈	개발 완료
//	---------------------------------------------------------------------------------
#pragma once

#ifdef DEVELOPER
#	define	KERNAL_ERROR(printf_string, ...)	\
	Kernal::getInstance().getDebugManager().alert(Kernal::getInstance().getHeader(), NEResult::LEVEL_ERROR, \
	_T("\n-----------------------------------------------------------")## \
	_T("\n")##_T(printf_string)## \
	_T("\n-----------------------------------------------------------")## \
	_T("\ndomain :\tKernal")## \
	_T("\ntype :\t\tError")## \
	_T("\nfunction :\t")##_T(__FUNCSIG__)## \
	_T("\nline :\t\t%d ")## \
	_T("\n")##_T(__FILE__)##_T("(%d)")## \
	_T("\n-----------------------------------------------------------\n"), \
	__VA_ARGS__, __LINE__, __LINE__);

#	define	KERNAL_WARNING(printf_string, ...)	\
	Kernal::getInstance().getDebugManager().alert(Kernal::getInstance().getHeader(), NEResult::LEVEL_WARNING, \
	_T("\n-----------------------------------------------------------")## \
	_T("\n")##_T(printf_string)## \
	_T("\n-----------------------------------------------------------")## \
	_T("\ndomain :\tKernal")## \
	_T("\ntype :\t\tWarning")## \
	_T("\nfunction :\t")##_T(__FUNCSIG__)## \
	_T("\nline :\t\t%d ")## \
	_T("\n")##_T(__FILE__)##_T("(%d)")## \
	_T("\n-----------------------------------------------------------\n"), \
	__VA_ARGS__, __LINE__, __LINE__);
	
#	define	KERNAL_INFORMATION(printf_string, ...)	\
	Kernal::getInstance().getDebugManager().alert(Kernal::getInstance().getHeader(), NEResult::LEVEL_INFORMATION, \
	_T("\n-----------------------------------------------------------")## \
	_T("\n")##_T(printf_string)## \
	_T("\n-----------------------------------------------------------")## \
	_T("\ndomain :\tKernal")## \
	_T("\ntype :\t\tInformation")## \
	_T("\nfunction :\t")##_T(__FUNCSIG__)## \
	_T("\nline :\t\t%d ")## \
	_T("\n")##_T(__FILE__)##_T("(%d)")## \
	_T("\n-----------------------------------------------------------\n"), \
	__VA_ARGS__, __LINE__, __LINE__);
	
#	define	ALERT_ERROR(printf_string, ...)	\
	Kernal::getInstance().getDebugManager().alert(getHeader(), NEResult::LEVEL_ERROR, \
	_T("\n-----------------------------------------------------------")## \
	_T("\n")##_T(printf_string)## \
	_T("\n-----------------------------------------------------------")## \
	_T("\ndomain :\tKernal")## \
	_T("\ntype :\t\tError")## \
	_T("\nfunction :\t")##_T(__FUNCSIG__)## \
	_T("\nline :\t\t%d ")## \
	_T("\n")##_T(__FILE__)##_T("(%d)")## \
	_T("\n-----------------------------------------------------------\n"), \
	__VA_ARGS__, __LINE__, __LINE__);
	
#	define	ALERT_WARNING(printf_string, ...)	\
	Kernal::getInstance().getDebugManager().alert(getHeader(), NEResult::LEVEL_WARNING, \
	_T("\n-----------------------------------------------------------")## \
	_T("\n")##_T(printf_string)## \
	_T("\n-----------------------------------------------------------")## \
	_T("\ndomain :\tKernal")## \
	_T("\ntype :\t\tWarning")## \
	_T("\nfunction :\t")##_T(__FUNCSIG__)## \
	_T("\nline :\t\t%d ")## \
	_T("\n")##_T(__FILE__)##_T("(%d)")## \
	_T("\n-----------------------------------------------------------\n"), \
	__VA_ARGS__, __LINE__, __LINE__);
	
#	define	ALERT_INFORMATION(printf_string, ...) \
	Kernal::getInstance().getDebugManager().alert(getHeader(), NEResult::LEVEL_INFORMATION, \
	_T("\n-----------------------------------------------------------")## \
	_T("\n")##_T(printf_string)## \
	_T("\n-----------------------------------------------------------")## \
	_T("\ndomain :\tKernal")## \
	_T("\ntype :\t\tInformation")## \
	_T("\nfunction :\t")##_T(__FUNCSIG__)## \
	_T("\nline :\t\t%d ")## \
	_T("\n")##_T(__FILE__)##_T("(%d)")## \
	_T("\n-----------------------------------------------------------\n"), \
	__VA_ARGS__, __LINE__, __LINE__);
	
#	define	ALERT_ERROR_IN_SPECIFIED_MODULE(identifier, printf_string, ...)	\
	Kernal::getInstance().getDebugManager().alert((identifier), NEResult::LEVEL_ERROR, \
	_T("\n-----------------------------------------------------------")## \
	_T("\n")##_T(printf_string)## \
	_T("\n-----------------------------------------------------------")## \
	_T("\ndomain :\tKernal")## \
	_T("\ntype :\t\tError")## \
	_T("\nfunction :\t")##_T(__FUNCSIG__)## \
	_T("\nline :\t\t%d ")## \
	_T("\n")##_T(__FILE__)##_T("(%d)")## \
	_T("\n-----------------------------------------------------------\n"), \
	__VA_ARGS__, __LINE__, __LINE__);
	
#	define	ALERT_WARNING_IN_SPECIFIED_MODULE(identifier, printf_string, ...)	\
	Kernal::getInstance().getDebugManager().alert((identifier), NEResult::LEVEL_WARNING, \
	_T("\n-----------------------------------------------------------")## \
	_T("\n")##_T(printf_string)## \
	_T("\n-----------------------------------------------------------")## \
	_T("\ndomain :\tKernal")## \
	_T("\ntype :\t\tWarning")## \
	_T("\nfunction :\t")##_T(__FUNCSIG__)## \
	_T("\nline :\t\t%d ")## \
	_T("\n")##_T(__FILE__)##_T("(%d)")## \
	_T("\n-----------------------------------------------------------\n"), \
	__VA_ARGS__, __LINE__, __LINE__);
	
#	define	ALERT_INFORMATION_IN_SPECIFIED_MODULE(identifier, printf_string, ...)	\
	Kernal::getInstance().getDebugManager().alert((identifier), NEResult::LEVEL_INFORMATION, \
	_T("\n-----------------------------------------------------------")## \
	_T("\n")##_T(printf_string)## \
	_T("\n-----------------------------------------------------------")## \
	_T("\ndomain :\tKernal")## \
	_T("\ntype :\t\tInformation")## \
	_T("\nfunction :\t")##_T(__FUNCSIG__)## \
	_T("\nline :\t\t%d ")## \
	_T("\n")##_T(__FILE__)##_T("(%d)")## \
	_T("\n-----------------------------------------------------------\n"), \
	__VA_ARGS__, __LINE__, __LINE__);
	
#	define ASSERT(expression)							\
	if(expression)										\
	{													\
		KERNAL_ERROR(#expression ## _T("is asserted!"))	\
		__asm int 3;									\
	}

///	IS_NULL:
///		checks given macro argument is a nulled REFERENCE or not. 
///		Because this macro is overloaded, you can let it know the specific type
///		which will be returned.
///		if return_type was not defined, is's set down to TYPE_RESULT of the 
///		KERNAL_WARNING macro's returning. (@see IS_NULL_1)
///		
///		Usage:
///		#1	as condition statement of if:	IS_NULL_1
///
///			type_index doSomthing(NEModule& module)
///			{
///				// without IS_NULL:
///				if( ! &module)
///				{
///					KERNEL_WARNING("module is nulled reference");
///					return NE_INDEX_ERROR;
///				}
///		
///				// with IS_NULL:
///				//	Only what you have to do is handling the return value.
///				if(IS_NULL(module))	return NE_INDEX_ERROR;	//	of course this calls KERNEL_WARNING, too.
///				
///				//	Do something..
///			}
///
///
///		#2	inline null check&returns:		IS_NULL_2
///			even if you're boring to handle returning value, you can do all 
///			of this in a single line.
///
///			type_index doSomething(NEModule& module)
///			{
///				//	without IS_NULL_2:
///				if( ! &module)
///				{
///					ALERT_WARNING("module is null reference.");
///					return NE_INDEX_ERROR;
///				}
///				
///				//	with IS_NULL_2:
///				IS_NULL(module, NE_INDEX_ERROR)
///			}
///
///			void doSomething(NEModule& module)
///			{
///				IS_NULL(module, ;)	//	uses ; when void.
///				//	you've to specify 2nd argument if you want to use macro as 
///				//	second usage. if you don't, macro recognize it as way#1, not 2.
///			}

	OVERLOADER(IS_NULL)

#	define IS_NULL_2(ref, return_type)							\
	(	! &(ref)											&&	\
		KERNAL_WARNING(#expression ## _T(" is null!")),	1	&&	\
		return_type											)

#	define IS_NULL_1(ref)								\
	if( ! &ref)											\
		return KERNAL_WARNING(#ref ## _T(" is null!"))


#else
#	define	ALERT_ERROR(printf_string, ...)				;
#	define	ALERT_WARNING(printf_string, ...)			;
#	define	ALERT_INFORMATION(printf_string, ...)		;
#	define	KERNAL_ERROR(printf_string, ...)			;
#	define	KERNAL_WARNING(printf_string, ...)			;
#	define	KERNAL_INFORMATION(printf_string, ...)		;
#	define	ASSERT(expression)							;
#endif
