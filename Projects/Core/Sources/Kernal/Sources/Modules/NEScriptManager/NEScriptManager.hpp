//	---------------------------------------------------------------------------------
//	클래스명:	NEScriptManager
//	설명	:	스크립트를 관리하는 관리자 클래스
//	관계	:	기반클래스.	NEModule
//	특성	:	노드엔진에 반드시 포함되는 모듈의 일종
//	알고리즘:	
//	사용방법:	모듈의 독립성을 제공하는 모듈매니져와 달리 스크립트는 어떤 환경에서도
//				인덱스가 같으므로 인덱스만으로도 접근이 가능하다.
//				1.	getScriptLength	스크립트의 갯수 파악
//				2.	getScriptCode		인덱스를 통해서 스크립트에 접근
//	메모	:	
//	히스토리:	2011-07-07	이태훈	개발 완료	
//	---------------------------------------------------------------------------------
#pragma once

//	include:
#include "../NEEnlistableManager/NEEnlistableManager.hpp"
#include "../../Commons/Interfaces/NEScriptHeader/NEScriptHeader.hpp"

namespace NE
{
	class NE_DLL NEScriptManager : public NEEnlistableManager
	{
		NE_DECLARE_INTERFACE(NEScriptManager, NEEnlistableManager)

		//	프렌드 선언:
		friend class NEScriptEditor;

		//	생성자:
	public:
		NEScriptManager();	
		NEScriptManager(const This& source);

		//	소멸자:
	public:
		virtual ~NEScriptManager();
		
		//	접근자:
	public:
		virtual NETString getScriptFilePath() const = 0;
		virtual const NEScriptHeader& getScriptHeader() const = 0;
		virtual const NERootNodeCodeSet& getScriptNodes() const = 0;
		
	protected:
		//	왜 const로 공개하지 않는가:
		//		NESynchronizer에서 사용해야하기 때문이다.
		//		NESynchronizer는 This를 수정 및 조정이 가능한 클래스다. 이게 가
		//		능한 이유는 This가 NEScynchronizer를 friend로 선언하기 때문이다
		//		friend는 상속이 안되므로, 여기에 미리 인터페이스를 만들어둘 필요가 있을
		//		것이다.
		virtual NERootNodeCodeSet& _getScriptNodes() = 0;
		virtual NEScriptHeader& _getScriptHeader() = 0;
	};
}
