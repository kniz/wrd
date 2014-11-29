//	---------------------------------------------------------------------------------
//	클래스명:	NENodeSelector
//	설명	:	키의 한 종류인 노드 선택자의 기반클래스이다.
//				어떠한 파생클래스가 상속되었느냐에 따라서  각기 다른 선택(Select)방법
//				을 적용한다.
//	관계	:	기반클래스.	NEKey
//	특성	:	추상 클래스.
//	알고리즘:	파생클래스의 알고리즘을 따른다.
//	사용방법:	getNode()로 파생클래스에서 정의된 선택방법을 통해서 노드를 하나씩 가져
//				온다.
//	메모	:	노드엔진은 셀렉터에게 부분 serialize를 지원하지 않는다.
//				부분 serialize란?
//					:	엔진 전체를 serialize를 하는게 아니라 Node 혹은 Selector
//						몇 개만 변칙적으로 serialize를 수행하는 것.
//				이유?
//					:	Selector가 NENode& 처럼 주소를 가지고 위치를 저장하기 때문이다.
//						위치 저장이란, 셀렉터가 노드를 찾고 그 노드를 반환하기 전에
//						다음에 무슨 노드를 검색한다고 하는 정보를 저장해야한다.
//						그렇지 않다면 매번 검색을 시도할때마다 처음부터 검색을 할것이다.
//						이 경우, "조건에 맞는 노드 3개만 검색하고 싶다" 고 했을때
//						같은 노드 3개의 주소가 나오게 되므로 실패다.
//
//							그래서 위치를 저장해야 하는데, 여기서 인덱스를 사용할 수가 없다.
//						모듈셋키나, 노드셋키 와 같은 것 때문이다.
//						인덱스만 가지고는 모듈안에 모듈셋안에 모듈셋과 같은 복잡한 트리
//						구조에서 정확한 위치를 "인덱스만으로" 표현하기가 어려울 뿐더러,
//							표현은 가능하나 "접근 속도가 너무 느리다"
//						따라서 여기서는 주소를 스택형태로 관리해서 위치를 저장한다(NEShortCutSet)
//
//							그런데, 왜 부분적으로 serialize가 안되는가.
//						셀렉터를 serialize(loader)할 때는 최소한 2번의 과정을 거쳐야 한다.
//							1. 모든 노드나 모듈의 인스턴스를 우선 다 만든다.
//							2. 만들어진 노드나 모듈에게 주소를 셀렉터에게 할당한다.
//						존재하지도 않는 인스턴스의 주소를 셀렉터에 할당할 수는 없기 때문이다.
//						따라서, 하나의 함수를 연쇄적으로 serialize를 하는 현재의 체계에서는 
//						2개를 나누어서 처리하기가 불가능하므로 하나의 함수를 더 만들어야 한다.
//
//						만약, 부분적으로 어떻게든 serialize(loader)를 하고 싶다면,
//						1	처음에 NEModule.serialize를 실행한다. NEModule.serialize에서는 키가
//							셀렉터일경우 해당 키는 serialize 하지 않는다.
//						2	그리고 나중에 수작업으로 모든 모듈의 키를 검사해서, 셀렉터일 경우,
//							serialize를 하면 된다.
//
//	히스토리:	2011-06-05	이태훈	개발 완료.
//				2013-10-24	이태훈	setManager로 탐색할 Manager를 지정할 수 있는 기능을
//									일부 축소변경했다. 이제는 NEType::Type으로만
//									줄 수 있다. 차후에 시스템에는 하나의 Kernal만
//									있도록 구성할 생각이다.
//	---------------------------------------------------------------------------------
#pragma once

//	include:
#include "../NENodeSelectorBase/NENodeSelectorBase.hpp"
#include "../../Commons/Units/NENodeBinder/NENodeBinder.hpp"

namespace NE
{
	class NENode;	
	class NENodeManager;
	class NEShortCutSet;
	class NEEnlistableManager;

	class NE_DLL NENodeSelector : public NENodeSelectorBase
	{
	public:
		friend NEEnlistableManager& NEBinderBase::getManager();				//	_interface를 위해 공개
		friend const NEEnlistableManager& NEBinderBase::getManager() const;	//				"

	public:
		typedef NENodeSelector ThisClass;
		typedef NENodeSelectorBase SuperClass;
		typedef ThisClass Trait;

		//	생성자:
	public:
		NENodeSelector(const NETString& keyname = "");
		NENodeSelector(NEType::Type manager_type, const NETString& keyname = "");
		NENodeSelector(const NENodeSelector& source);

		//	소멸자:
	public:
		virtual ~NENodeSelector();

	public:
		const NENodeSelector& operator=(const NENodeSelector& source);
		const NENodeSelector& operator=(const NEKey& source);
		bool operator==(const NENodeSelector& source) const;
		bool operator!=(const NENodeSelector& source) const;

	public:
		Trait& getValue() { return *this; }	//	NEKey에 대한 클래스 템플릿 호환을 위해 만들어둔다.
		const Trait& getValue() const { return *this; }

		//	인터페이스:
	public:
		virtual type_result bind();

	public:
		virtual type_result initializeReferingPoint();

	public:
		virtual type_result initialize();

	public:
		virtual type_result assign(const NEKey& source);
		virtual type_result extend(const NEKey& source);
		virtual bool isEqualTo(const NEKey& source) const;

	public:
		virtual void release();
		virtual type_result isValid() const;
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual NEObject& clone() const;
		virtual NEType::Type getType() const;
		
	public:
		NENode& getNode();
		NENode& peekNode();
		const NENode& peekNode() const;
		type_result initializeNodeSelectorReferingPoint();
		type_result setManager(NEType::Type type);
		NEType::Type getManagerType() const;
		NEEnlistableManager& getManager();
		const NEEnlistableManager& getManager() const;
		bool isManagerTypeValid(NEType::Type type) const;
		NENodeBinder& getBinder();
		const NENodeBinder& getBinder() const;
		type_bool& isUsingAutoBinding();
		type_bool isUsingAutoBinding() const;

		//	내부 가상 함수:	Editor패키지와 Kernal 패키지 양쪽에서 사용할 수 있게 virtual로 선언한다.
	private:
		virtual NEShortCutSet& _getShortCutSet();
		virtual const NEShortCutSet& _getShortCutSet() const;

	private:
		const NENodeSelector& _assign(const NENodeSelector& source);
		void _release();		
		type_result _setManagerToNodeManager();

	private:
		NENode& _getNextByNoLimit();
		NENode& _getNextByLimit();
		NENode& _getNodeByRecent();
		NENode& _getNodeByCode();
		NENode& _getNodeByBinder();
		bool _isPassedFilter(NENode* extracted_node) const;
		
	protected:
		type_index _searching_code_index;
		type_index _searching_index;
		NEType::Type _manager_type;
		NENodeBinder _binder;
		type_bool _use_auto_binding;

	private:
		typedef NEEnlistableManager& (*GET_MANAGER_INTERFACE)(NEType::Type);
		static NENode* _recent;
		static GET_MANAGER_INTERFACE _interface;
		friend class Editor;
	};
}