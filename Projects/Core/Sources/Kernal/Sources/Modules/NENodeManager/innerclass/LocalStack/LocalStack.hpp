//	---------------------------------------------------------------------------------
//	클래스명:	NENodeManager::LocalStack
//	설명	:	노드매니져의 스택공간. OS의 스택공간 처럼, 최근의 사용한 데이터들이
//				임시로 저장되는 공간이다.
//				최근의 사용된 모듈주소, 최근에 사용된 노드주소, 최근에 나온 에러코드
//				등이 존재한다.
//	관계	:	기반클래스.	NEObject
//	특성	:	중첩클래스. 노드매니져에 friend 선언.
//				ReadOnly. 외부에서 값이 수정되서는 안되기 때문이다.
//	알고리즘:	
//	사용방법:	
//	메모	:	
//	히스토리:	2011-07-07	이태훈	개발 완료	
//	---------------------------------------------------------------------------------
class NE_DLL LocalStack : public NEObject
{
	//	프렌드 선언:
public:
	friend class NEModuleCodeSet;
	friend class NENodeCodeSet;
	friend class NENode;
	friend class NEModule;
	
public:
	LocalStack();
	LocalStack(const LocalStack& source);

public:
	virtual ~LocalStack();

	//	접근자:
public:
	virtual NENode& getRecentNode() = 0;
	virtual const NENode& getRecentNode() const = 0;
	virtual NEModule& getRecentModule() = 0;
	virtual const NEModule& getRecentModule() const = 0;
	virtual NEModuleCodeSet& getRecentModuleSet() = 0;
	virtual const NEModuleCodeSet& getRecentModuleSet() const = 0;
	virtual type_index getRecentModuleSetIndex() const = 0;
	virtual type_result getRecentResultCode() const = 0;
	virtual const NECode& getRecentPriorityCode() const = 0;
	virtual type_count getFrameCount() const = 0;
	virtual NENodeCodeSet& getRecentNodeSet() = 0;
	virtual const NENodeCodeSet& getRecentNodeSet() const = 0;
	virtual type_index getRecentNodeSetIndex() const = 0;
	virtual type_index getRecentNodeRealIndex() const = 0;
	virtual type_index getRecentModuleRealIndex() const = 0;

protected:	
	virtual void _setRecentNode(NENode& new_node) = 0;
	virtual void _setRecentModuleSet(NEModuleCodeSet& new_moduleset) = 0;
	virtual void _setRecentModuleSetIndex(type_index moduleset_index) = 0;
	virtual void _setRecentResultCode(type_result new_result) = 0;
	virtual void _setRecentPriorityCode(type_code new_priority) = 0;
	virtual void _setFrameCount(type_count new_frame_count) = 0;
	virtual void _setRecentNodeSet(NENodeCodeSet& new_node_codeset) = 0;
	virtual void _setRecentNodeSetIndex(type_index nodeset_index) = 0;
	virtual void _setRecentNodeRealIndex(type_index real_node_index) = 0;

public:
	virtual NEType::Type getType() const;
};
