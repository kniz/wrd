//	---------------------------------------------------------------------------------
//	DLL명	:	Kernal.dll
//	설명	:	노드엔진 실행 커널. 노드엔진이 실행되기 위한 최소한의 구성으로 이루어져
//				있다.
//	관계	:	기반 패키지.	FileAccess
//	특성	:	
//	외부포함:	
//	클래스	:	
//				commons
//					NEResult
//						NEResult::Level
//					NEType
//					NEBinaryTemplates
//						NEBinarySaverTemplates
//						NEBinaryLoaderTemplates
//				module
//					NEObject
//						NECollectorTemplate
//							NEArrayTemplate
//								NEKeySet
//								NEModuleSet
//								NENodeSet
//								NETString
//								NEShortCutSet
//							NEListTemplate
//								NEListTemplate::Iterator
//								NEListTemplate::Iterator
//								NEModuleList
//								NEShortCut
//								NENodeList
//						NEUnit
//							NENamedUnit
//								NEModule
//									NEExportable::Header
//									NEDebugManager
//									NEINIManager
//									NEModuleManager
//										NEModuleManager::DLLHeader
//									NENodeManager
//										NENodeManager::LocalStack
//									NEScriptManager
//									Kernal							
//								NENode
//						NEKey
//							NEKeyTemplate
//							NENodeSelector
//								NEModuleSelectorTemplate
//									NEKeySelectorTemplate
//	메모	:
//	히스토리:	2011-06-04	이태훈	개발 완료
//	히스토리:	2011-07-23	이태훈	추가
//					:	1	노드셋키 추가
//						2	노드셋키 추가로 인한 노드 셀럭터 fetch 알고리즘 변경
//						3	모듈셀렉터 저장 기능 추가
//						4	모듈셀렉터 저장 기능 추가하기 위한 모듈 셀렉터 fetch 알고리즘 변경
//						5	노드셋키 추가로 인한 스택공간의 변화
//						6	스크립트로드시, 모듈의 identifier를 통해서 로드
//							pc환경과, 모듈의 종류가 달라져도 인식 및 정상적인 로드가 가능해졌다
//						7	데이터파일 로드 알고리즘의 간소화. 용량이 줄고, 속도가 빨라졌다.
//							데이터파일은 이미 만들어진 스크립트로부터 복사가 된다.
//						8	데이터파일 로드시, 데이터 파일에 맞는 스크립트파일인지 판단하는 기능
//							추가.
//							7번에 의해서 데이터 파일은 스크립트 파일에 의존성을 갖게 되었다.
//						9	알맞는 스크립트 파일이 아닐 경우, INI에서 지정한 스크립트 디렉토리
//							에서 스크립트를 검색하는 기능.
//						10	스크립트 파일별로, 헤더를 추가
//						11	모듈별로 헤더와 식별자를 추가
//						12	노드엔진의 헤더를 추가. INI로부터 추가 정보를 받도록 구현했다.
//						13	새로운 에러객체를 정의. (= type_result )
//						14	FileAccess에 이진파일의 위치를 지정하고 가져오는 함수 추가
//						15	6번에서 가져온 모듈의 식별자가 현재PC에 존재하지 않을 경우 데이터
//							로드를 진행 할 수 없다. (모듈의 데이터 길이를 알수 없으므로, 다음
//							모듈의 로드할 위치를 알 수 없다)
//							이를 해결하고, 스킵이 가능하도록 추가했다.
//						16	모듈의 Connector 문제를 해결했다.
//							기존의 Connector는 한번 push된 데이터가 pop되고 push되면 작동이 안되는
//							형태였다.
//							따라서, 셀렉터 같은 경우는 Connector에 적용이 힘들었다.
//							새로 바뀐 Connector 알고리즘은, 모듈의 키셋의 _data의 주소를 바꾸지만
//							않는다면, 키를 pop, push해도 인식이 가능하도록 작성되었다.
//						17	호환버전의 개념추가.
//							실제 버전은 revision 이란 개념으로 하나씩 증분해가되, 버전의 표기는
//							version의 문자열로 하도록 만들었다.
//						18	타입의 구조가 유연해졌다.
//							이제 새로운 타입을 추가/삭제하는 게 편해지고, 보다 구조적으로 변경
//							되었다.
//						19	INI를 통해서 DLL 파일을 로드 안할 수 있는 기능이 추가되었다.
//							한번 로드를 실시하면, DLL의 경로와 함께 TRUE,FALSE를 줄수 있다.
//						20	모듈에서 카테고리를 "이름" 에서 "식별자"로 변경했다.
//	---------------------------------------------------------------------------------
#pragma once

//	include:
//	TODO: 다른 헤더파일도 include.

#include "../Modules/NERootNodeCodeSet/NERootNodeCodeSet.hpp"
#include "../Modules/Kernal/Kernal.hpp"

#include "../Commons/Typedefs/NETStringKey/NETStringKey.hpp"
#include "../Commons/Typedefs/NETString/NETString.hpp"
#include "../Commons/Typedefs/NETCharKey/NETCharKey.hpp"
#include "../Commons/Typedefs/NETStringSetKey/NETStringSetKey.hpp"

#include "../Modules/NEBooleanKey/NEBooleanKey.hpp"
#include "../Modules/NECharKey/NECharKey.hpp"
#include "../Modules/NEByteKey/NEByteKey.hpp"
#include "../Modules/NEUByteKey/NEUByteKey.hpp"
#include "../Modules/NEWCharKey/NEWCharKey.hpp"
#include "../Modules/NEShortKey/NEShortKey.hpp"
#include "../Modules/NEUShortKey/NEUShortKey.hpp"
#include "../Modules/NEIntKey/NEIntKey.hpp"
#include "../Modules/NEUIntKey/NEUIntKey.hpp"
#include "../Modules/NEInt64Key/NEInt64Key.hpp"
#include "../Modules/NEFloatKey/NEFloatKey.hpp"
#include "../Modules/NEDoubleKey/NEDoubleKey.hpp"
#include "../Modules/NEBooleanSetKey/NEBooleanSetKey.hpp"
#include "../Modules/NEByteSetKey/NEByteSetKey.hpp"
#include "../Modules/NEUByteSetKey/NEUByteSetKey.hpp"
#include "../Modules/NEShortSetKey/NEShortSetKey.hpp"
#include "../Modules/NEUShortSetKey/NEUShortSetKey.hpp"
#include "../Modules/NEIntSetKey/NEIntSetKey.hpp"
#include "../Modules/NEUIntSetKey/NEUIntSetKey.hpp"
#include "../Modules/NEInt64SetKey/NEInt64SetKey.hpp"
#include "../Modules/NEFloatSetKey/NEFloatSetKEy.hpp"
#include "../Modules/NEDoubleSetKey/NEDoubleSetKey.hpp"
#include "../Modules/NEStringSetKey/NEStringSetKey.hpp"
#include "../Modules/NEWStringSetKey/NEWStringSetKey.hpp"
#include "../Modules/NENodeSelector/NENodeSelector.hpp"
#include "../Modules/NEModuleSelector/NEModuleSelector.hpp"
#include "../Modules/NEKeySelector/NEKeySelector.hpp"
#include "../Modules/NEModuleCodeSetKey/NEModuleCodeSetKey.hpp"
#include "../Modules/NENodeCodeSetKey/NENodeCodeSetKey.hpp"
#include "../Modules/NEKeySelector/NEKeySelector.hpp"
#include "../Commons/Typedefs/NETStringSetKey/NETStringSetKey.hpp"
#include "../Commons/Typedefs/NETStringKey/NETStringKey.hpp"
#include "../Commons/Typedefs/NETString/NETString.hpp"
#include "../Commons/Templates/NETArgument/NETArgument.hpp"

//	NEModule이 kernal에 있어야 하는 이유
//	템플릿클래스이기 때문이다. 템플릿클래스는 특성상 인스턴스가 선언하면서 생성된다. NE_DLL은 이미 존재하는 DLL의 인스턴스를 가져오는 매크로.
//	때문에 템플릿 클래스를 DLL에서 사용하고 싶다면 2가지를 택해야한다.
//		1. DLL 내부에 인스턴스를 미리 생성하고 _declspec(dllimport)로 외부에서 가져와 쓴다
//		2. 외부에서 템플릿 클래스 인스턴스를 생성한다. 이때 템플릿클래스에 NE_DLL을 쓰면 외부에서 include할때 _declspec(dllimport)로 변환되므로
//		NE_DLL 없이 헤더를 작성해야 하고, 그렇게되면 내부에서 생성한 클래스템플릿을 export해야하는 방법을 생각해야 할것이다.
//	여기서는 템플릿클래스가 선언된 DLL에 같이 NEModule을 선언함으로써 번거로운 2번 대신, 1번을 택했다.

#ifndef KERNAL_EXPORTS		  
#	ifdef CORES_BUILD_MODE
#		ifdef DEVELOPER
#			ifdef _DEBUG
#				ifdef _UNICODE
#					pragma comment(lib, "../../../Binaries/Core/Kernal.dev.dbg.lib")
#				else
#					pragma comment(lib, "../../../Binaries/Core/Kernal.dev.dbg.mbcs.lib")
#				endif
#			else
#				ifdef _UNICODE
#					pragma comment(lib, "../../../Binaries/Core/Kernal.dev.lib")
#				else
#					pragma comment(lib, "../../../Binaries/Core/Kernal.dev.mbcs.lib")
#				endif
#			endif
#		else
#			ifdef _UNICODE
#				pragma comment(lib, "../../../Binaries/Core/Kernal.lib")
#			else
#				pragma comment(lib, "../../../Binaries/Core/Kernal.mbcs.lib")
#			endif
#		endif
#	else
#		ifdef DEVELOPER
#			ifdef _DEBUG
#				ifdef _UNICODE
#					pragma comment(lib, "Kernal.dev.dbg.lib")
#				else
#					pragma comment(lib, "Kernal.dev.dbg.mbcs.lib")
#				endif
#			else
#				ifdef _UNICODE
#					pragma comment(lib, "Kernal.dev.lib")
#				else
#					pragma comment(lib, "Kernal.dev.mbcs.lib")
#				endif
#			endif
#		else
#			ifdef _UNICODE
#				pragma comment(lib, "Kernal.lib")
#			else
#				pragma comment(lib, "Kernal.mbcs.lib")
#			endif
#		endif
#	endif
#endif
