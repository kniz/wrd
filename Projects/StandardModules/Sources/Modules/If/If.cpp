#include "If.hpp"
#include "define/define.hpp"

namespace NE
{
	NE_DLL If::If()
		: NEModule()
	{
				
	}

	NE_DLL If::If(const If& source)
		: NEModule(source)
	{
		
	}

	NE_DLL If::~If()
	{

	}
	
	type_result NE_DLL If::initialize()
	{
		getKeySet().create(_KEY_NUMBER);
		getKeySet().push(NEModuleCodeSetKey());
		getKeySet().push(NEModuleCodeSetKey());
		getKeySet().push(NEUByteKey());
		
		return RESULT_SUCCESS;
	}

	type_result NE_DLL If::execute()
	{
		NEModuleCodeSet&	conditionset	= (static_cast<NEModuleCodeSetKey&>(	getKeySet()[0]	)).getValue(),
						 &	actionset		= (static_cast<NEModuleCodeSetKey&>( 	getKeySet()[1]	)).getValue();
		//	플래그:
		//		플래그의 종류:
		//			1	:	condition의 모든 값이 RESULT_FALSE일때
		//			2	:	condition의 모든 값이 RESULT_TRUE일대
		//			3	:	condition중에서 최소 하나라도 RESULT_FALSE가 있을때
		//			4	:	condition중에서 최소 하나라도 RESULT_TRUE가 있을때
		//			5	:	항상 실행
		type_ubyte 		flag 			= (static_cast<const NEUByteKey&>( 	getKeySet()[2]	)).getValue();



		//	main:
		//		분기 루핑:
		/*			
					속도 개선을 위해서 루프안에 분기를 두지 않고, 분기마다 루프를
					작성하여 코드 최적화
		*/
		switch(flag)
		{
		case _ALWAYS:
			//	모든 값이 RESULT_FALSE일때:
			//		(= 하나라도 RESULT_TRUE이면 종료)
			return actionset.execute();

		case _EVERYTHING_IS_FALSE:			
			//	모든 값이 RESULT_FALSE일때:
			//		(= 하나라도 RESULT_TRUE이면 종료)
			for(int n=0; n < conditionset.getLength() ;n++)
				if( NEResult::hasTrue(conditionset[n].execute()) )
					return RESULT_SUCCESS | RESULT_ABORT_ACTION | RESULT_TRUE;
			
			return actionset.execute();


		case _EVERYTHING_IS_TRUE:
			//	모든 값이 RESULT_TRUE일때:
			//		(= 하나라도 RESULT_FALSE이면 종료)
			for(int n=0; n < conditionset.getLength() ;n++)
				if( NEResult::hasFalse(conditionset[n].execute()) )
					return RESULT_SUCCESS | RESULT_ABORT_ACTION | RESULT_FALSE;
			
			return actionset.execute();


		case _HAS_FALSE:
			//	최소 하나라도 RESULT_FALSE가 있을때:
			//		(= 하나라도 RESULT_FALSE가 나타나면 actionset 실행)
			for(int n=0; n < conditionset.getLength() ;n++)
				if( NEResult::hasFalse(conditionset[n].execute()) )
					return actionset.execute();		

			return RESULT_SUCCESS | ACTION_HAS_NOT_BEEN_ACTIVATED;
			

		case _HAS_TRUE:
			//	최소 하나라도 RESULT_TRUE가 있을때:
			//		(= 하나라도 RESULT_TRUE가 나타나면 actionset 실행)
			for(int n=0; n < conditionset.getLength() ;n++)
				if( NEResult::hasTrue(conditionset[n].execute()) )
					return actionset.execute();		
			return RESULT_SUCCESS | ACTION_HAS_NOT_BEEN_ACTIVATED;
			

		default:
			ALERT_ERROR(" : 조건 상수가 잘못되었습니다.\n%d < 조건상수 < %d가 만족되지 않습니다.\n잘못된 조건 상수 : %d", flag);
			return RESULT_TYPE_ERROR | INVALID_CONDITION_FLAG;
		}		
	}

	NEObject NE_DLL &If::clone() const
	{
		return *(new If(*this));
	}

	const NEExportable::ModuleHeader NE_DLL &If::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName() = _T( _NAME );
			_header.getDeveloper() = _T( _DEVELOPER );
			_header.setRevision(_REVISION);
			_header.getComment() = _T( _COMMENT );
			_header.getVersion()  = _T( _VERSION );
			_header.getReleaseDate() = _T( _DATE );
			_header.getModuleDependencies().create(_DEPENDENCIES_COUNT);
			_header.setMaxErrorCodeCount(If::ERROR_CODE_END - 1);
		}

		return _header;
	}

	LPCTSTR NE_DLL If::getErrorMessage(type_errorcode errorcode) const
	{
		switch(errorcode)
		{
		case MODULE_IS_NOT_VALID:
			return _T("잘못된 Module");

		case ACTION_HAS_NOT_BEEN_ACTIVATED:
			return _T("");

		case INVALID_CONDITION_FLAG:
			return _T("");

		default:
			return _T("알수 없는 에러");
		}
	}
}
