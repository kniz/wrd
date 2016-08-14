//	---------------------------------------------------------------------------------
//	클래스명:	NEObject
//	설명	:	노드 엔진에서 사용중인 모든 객체를 나타낸다.
//	관계	:	기반 패키지인 FileAccess를 사용한다.
//	특성	:	Kernal의 모든 객체의 기반클래스.
//				가상함수 등의 인터페이스를 정의한다.
//	알고리즘:	
//	사용방법:	모든 객체는 clone, release, isValid, serialize의 5가지 override 가능한
//				함수를 갖는다.
//					1. clone		
//						:	가상 복사 생성자. 단, NEObject&의 형태로 나오므로 주의.
//					2. release
//						:	객체의 모든 데이터를 초기화한다. 처음 인스턴스가 만들어진 상태로
//							되돌려지므로, 상태변수 또한 초기화 된다.
//					3. isValid
//						:	이 인스턴스가 올바른 인스턴스인지 가능한한 검증해준다. 만약, 에러
//							가 발생할 경우 NEDebugManager를 통해서 에러가 기록된다.
//					4. serialize(saver, loader)
//						:	이진파일에 저장과 로드를 수행한다.
//				
//				NEObject의 고유 인터페이스는 다음과 같다.
//					1. getType		:	NEType::Type를 반환한다.						
//					2. isEqualClassTo	:	NEType::Type이 같은지 반환한다.
//					3. isUndefined	:	NEType::Type이 UNDEFINED인지 확인한다.
//										노드엔진에서 UNDEFINED는 정상적으로는 나올 수 없는 
//										뭔가의 오류를 의미한다.
//	메모	:	
//	히스토리:	2011-06-05	이태훈	개발 완료.
//	---------------------------------------------------------------------------------
#pragma once

//	include:
#include "../../Commons/Functions/NEResult/NEResult.hpp"
#include "../NEAdam/NEAdam.hpp"
#include "../../Commons/Functions/NEBinarySaverTemplates/NEBinarySaverTemplates.hpp"
#include "../../Commons/Functions/NEBinaryLoaderTemplates/NEBinaryLoaderTemplates.hpp"
#include "../NETString/NETString.hpp"

namespace NE
{
	class NE_DLL NEObject
	{
		NE_DECLARE_INTERFACE_ONLY(NEObject, NEAdam)

		//	생성자:
	public:
		NEObject();
		NEObject(const NEObject& source);

		//	소멸자:
	public:
		virtual ~NEObject();

		//	접근자:
	public:
		type_bool isEqualClassTo(const This& source) const;
		type_bool isSuperClassOf(const This& parent) const;
		type_bool isSuperClassOf(const NEClassBase& parent) const;
		type_bool isSubClassOf(const This& parent) const;
		type_bool isSubClassOf(const NEClassBase& parent) const;
		const NETString& getClassName() const;

		//	인터페이스:
	public:
		virtual const NEClassBase& getClass() const = 0;
		virtual NEObject& clone() const = 0;
		virtual void release() = 0; 
		virtual	type_result isValid() const = 0;
		virtual	NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const = 0;
		virtual	NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader) = 0;
	};		
}
