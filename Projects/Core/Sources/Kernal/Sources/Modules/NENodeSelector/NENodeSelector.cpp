#include "NENodeSelector.hpp"
#include "../Kernal/Kernal.hpp"
#include "../NEShortCutSet/NEShortCutSet.hpp"
#include "../NEIndexedNodeSet/NEIndexedNodeSet.hpp"

namespace NE
{
	NEEnlistableManager& _defaultManagerInterface(NEType::Type type)
	{
		//	main:
		switch(type)
		{
		case NEType::NENODE_MANAGER:	return Kernal::getInstance().getNodeManager();
		case NEType::NESCRIPT_MANAGER:	return Kernal::getInstance().getScriptManager();
		default:
			NEEnlistableManager* nullpointer = 0;
			return *nullpointer;
		};
	}

	NENode* NENodeSelector::_recent = 0;
	NENodeSelector::GET_MANAGER_INTERFACE NENodeSelector::_interface = _defaultManagerInterface;

	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NENodeSelector::NENodeSelector(const NETString& keyname)
		: NENodeSelectorBase(keyname)
	{	
		_release();
		initializeNodeSelectorReferingPoint();
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NENodeSelector::NENodeSelector(const NENodeSelector& source)
		: NENodeSelectorBase(source)
	{		
		_assign(source);
	}

	NENodeSelector::NENodeSelector(NEType::Type manager_type, const NETString& keyname)
		: NENodeSelectorBase(keyname)
	{
		_release();
		setManager(manager_type);
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NENodeSelector::~NENodeSelector()
	{

	}		



	const NENodeSelector  &NENodeSelector::operator=(const NENodeSelector& source)
	{
		NENodeSelectorBase::operator=(source);

		return _assign(source);
	}

	const NENodeSelector& NENodeSelector::operator=( const NEKey& source )
	{
		assign(source);

		return *this;
	}

	type_bool  NENodeSelector::operator==(const NENodeSelector& source) const
	{
		if(this == &source) return true;
		if(NENodeSelectorBase::operator!=(source)) return false;
		if(_searching_index != source._searching_index) return false;
		if(_searching_code_index != source._searching_code_index) return false;
		if( _manager_type != source._manager_type) return false;
		if(_binder != source._binder) return false;
		if(_use_auto_binding != source._use_auto_binding) return false;
		if(_is_returned_once != source._is_returned_once) return false;

		return true;
	}



	type_bool  NENodeSelector::operator!=(const NENodeSelector& source) const
	{
		return !(operator==(source));
	}




	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	//	 function by NEObject
	NEObject  &NENodeSelector::clone() const
	{
		return *(new NENodeSelector(*this));
	}



	type_result  NENodeSelector::initialize()
	{
		setManager(NEType::NENODE_MANAGER);
		_binder.release();
		return initializeReferingPoint();
	}



	type_result  NENodeSelector::initializeNodeSelectorReferingPoint()
	{
		_is_returned_once = false;

		if(_binder.isBinded()) return RESULT_SUCCESS;

		_count = 0;
		_searching_code_index = 0;
		_searching_index = -1;

		return RESULT_SUCCESS;
	}


	type_result NENodeSelector::isValid() const
	{
		type_result result = NENodeSelectorBase::isValid();
		if(NEResult::hasError(result)) return result;
		if(_searching_code_index < 0) return RESULT_TYPE_ERROR | RESULT_INVALID_DATA | RESULT_WRONG_BOUNDARY;
		if( ! isManagerTypeValid(_manager_type)) return RESULT_TYPE_ERROR;
		result = _binder.isValid();
		if(NEResult::hasError(result)) return result;

		return RESULT_SUCCESS;
	}


	void NENodeSelector::releaseValue()
	{
		NENodeSelectorBase::releaseValue();

		_release();
	}

	void NENodeSelector::release()
	{
		NENodeSelectorBase::release();

		return _release();
	}



	NEBinaryFileSaver& NENodeSelector::serialize(NEBinaryFileSaver& saver) const
	{
		NENodeSelectorBase::serialize(saver);

		return saver << _searching_code_index << _searching_index << _manager_type << _binder << _use_auto_binding << _is_returned_once;
	}



	NEBinaryFileLoader& NENodeSelector::serialize(NEBinaryFileLoader& loader)
	{
		NENodeSelectorBase::serialize(loader);

		return loader >> _searching_code_index >> _searching_index >> _manager_type >> _binder >> _use_auto_binding >> _is_returned_once;
	}



	//	---------------------------------------------------------------------------------
	//	설명	:	NEKey끼리 캐스팅 없이 할당이 가능하게 하는 함수. operator=와 같다.
	//				내부에서 캐스팅을 실시한다.
	//	동작조건:
	//	메모	:
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	type_result  NENodeSelector::assign(const NEKey& source)
	{
		if(this == &source) return RESULT_SUCCESS;
		if( ! isEqualTypeWith(source)) return RESULT_TYPE_ERROR;

		*this = static_cast<const NENodeSelector&>(source);

		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	설명	:	NEKey끼리 캐스팅 없이 덧셈 할당을 실행한다. operator+=와 같다.
	//				셀렉터키는 해당되지 않는다.
	//	동작조건:
	//	메모	:
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	type_result  NENodeSelector::extend(const NEKey& source)
	{
		return RESULT_TYPE_ERROR;
	}



	//	---------------------------------------------------------------------------------
	//	설명	:	서로 같은 값을 가졌는지를 판단한다.
	//				먼저 타입을 확인하고, 같은 타입일 경우 operator==를 호출한다.
	//	동작조건:
	//	메모	:
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	type_bool  NENodeSelector::isEqualTo(const NEKey& source) const
	{
		if( ! isEqualTypeWith(source)) return false;

		return operator==( static_cast<const NENodeSelector&>(source) );
	}



	//	---------------------------------------------------------------------------------
	//	설명	:	지정된 조건(멤버변수)를 통해서 노드를 가져온다.
	//	알고리즘:	_countlimit가 0인지 아닌지를 통해서 2가지 방식으로 검색을 실시한다.
	//	동작조건:	스크립트코드가 최소한 1개 이상 있어야 한다.
	//	메모	:	변수들의 자세한 설명은 클래스의 헤더파일을 참조하라.
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NENode  &NENodeSelector::getNode()
	{	
		//	pre:
		NENode* to_return = NE_NULL;
		//		바인딩되어있는가:
		if(_binder.isBinded())
			return _getNodeByBinder();
		//		널포인터:


		//	main:	
		const NECodeType& type = getCodes().getCodeType();
		switch(type.getCodeType())
		{
		case NECodeType::ME:
		case NECodeType::RECENT:		
			to_return = &_getNodeByRecent();
			break;

		case NECodeType::ALL:
		case NECodeType::GROUP:
		case NECodeType::NAME:
		case NECodeType::SCRIPT:
		case NECodeType::PRIORITY:
			_is_returned_once = false;	//	이 때는 사용하지 않는다.
			to_return = &_getNodeByCode();
			break;

		case NECodeType::UNDEFINED:
		default:
			KERNAL_ERROR(" : ")
		}		

		if(to_return)
		{
			_recent = to_return;

			if(	_use_auto_binding	&& 
				! _binder.isBinded())
				NENodeSelector::bind();
		}
		return *to_return;
	}	

	void NENodeSelector::_release()
	{
		_searching_code_index = 0;
		_searching_index = -1;
		_manager_type = NEType::NENODE_MANAGER;
		_binder.release();
		_use_auto_binding = false;
		_is_returned_once = false;
	}



	//	---------------------------------------------------------------------------------
	//	설명:		노드를 반환함과 동시에 포인터를 하나 이동시키는 getNode()와 달리, 포인
	//				터를 이동시키지 않는다.
	//	히스토리:	2011-07-17	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NENode  &NENodeSelector::peekNode()
	{
		//	pre:
		if(_binder.isBinded())
			return _binder.getBinded();
		//		널 레퍼런스:
		NENode* null_pointer = NE_NULL;		
		//		노드매니져:
		NEEnlistableManager& manager = getManager();
		if( ! &manager)
		{
			KERNAL_ERROR(" : ");
			return *null_pointer;
		}		
		const NECodeType& node_type = getCodesType();
		//		RECENT 방식인지 검사:
		if(node_type == NECodeType::UNDEFINED)
		{
			KERNAL_ERROR(" : ")

				return *null_pointer;
		}
		if(node_type == NECodeType::RECENT)
		{
			return *_recent;
		}
		if(node_type == NECodeType::ME)
		{
			if( ! manager.isSubClassOf(NETClass<NENodeManager>()))
			{
				KERNAL_ERROR(" : ");
				return *null_pointer;
			}
			NENodeManager& noder = static_cast<NENodeManager&>(manager);
			return noder.getLocalStack().getRecentNode();
		}
		//		멤버변수 검사:
		if(_searching_index <= -1) return *null_pointer;
		//		타겟팅:
		NEShortCut& shortcut = _getShortCut();
		type_index index = shortcut.getElement(_searching_index);


		//	main:
		//		노드 추출:		
		return manager._getNodeSet().getElement(index);
	}



	//	---------------------------------------------------------------------------------
	//	설명:		노드를 반환함과 동시에 포인터를 하나 이동시키는 getNode()와 달리, 포인
	//				터를 이동시키지 않는다.
	//	히스토리:	2011-08-04	이태훈	작성
	//	---------------------------------------------------------------------------------
	const NENode  &NENodeSelector::peekNode() const
	{
		//	pre:
		if(_binder.isBinded())
			return _binder.getBinded();
		//		널 레퍼런스:
		NENode* null_pointer = NE_NULL;
		//		타겟팅:
		const NEEnlistableManager& manager = getManager();
		const NECodeType& node_type = getCodesType();
		if( ! &manager)
		{
			KERNAL_ERROR(" : ");
			return *null_pointer;
		}
		//		멤버변수 Validation:
		if(node_type == NECodeType::UNDEFINED)
		{
			KERNAL_ERROR(" : ")
				return *null_pointer;
		}
		//		RECENT 방식인지 검사:
		if(node_type == NECodeType::RECENT)
		{				
			if( ! manager.isSubClassOf(NETClass<NENodeManager>()))
			{
				KERNAL_ERROR(" : ");
				return *null_pointer;
			}
			const NENodeManager& noder = static_cast<const NENodeManager&>(manager);
			return noder.getLocalStack().getRecentNode();
		}
		//		멤버변수 검사:
		if(_searching_index <= -1) return *null_pointer;
		//		타겟팅:
		const NEShortCut& shortcut = _getShortCut();
		type_index index = shortcut.getElement(_searching_index);




		//	main:
		//		노드 추출:
		return manager.getNodeSet().getElement(index);
	}



	//	---------------------------------------------------------------------------------
	//	설명	:	노드를 루프형식으로 검색한다.
	//				기본적으로 조건에 맞는 노드를 무한히 루프를 돌면서 검색한다.
	//				(인덱스 끝에 도달하면 검색을 끝내는 것이 아니라, 다시 처음으로 돌아간다)
	//				단, _countlimit 번째 노드를 검색한 순간 "널"을 내보낸다.
	//	동작조건:	_countlimit가 최소 1이상이어야 한다.
	//	메모	:
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//				2014-12-02	이태훈	ALL일때 발생하는 오류 수정
	//	---------------------------------------------------------------------------------
	NENode& NENodeSelector::_getNextByLimit()
	{	//	배경:		manager.shortcutset[ 스크립트 번호 ][ 0 ~ N ]
		//				의 형태로 NENode*이 들어있다.
		//				예를들어, manager.shortcutset[1][4]를 하면 
		//				1번스크립트를 가지고있는 4번째 NodeUnit이 반환된다.
		//	
		//				스크립트 코드는 중복이 없다.
		//				하나의 노드는 하나의 스크립트 코드만을 갖는다.

		//	pre:
		//		널 레퍼런스:
		NENode* null_pointer = NE_NULL;
		//		타겟팅:
		//			숏컷셋:
		NEShortCutSet& shortcutset = _getShortCutSet();
		if( ! &shortcutset)
			return *null_pointer;
		//		관련 멤버변수 검사:
		//			검색 횟수:
		if(_count >= _countlimit) return *null_pointer; // 탐색안하고 바로 종료사인



		//	main:
		//		초기 위치정보 저장:	무한루프를 방지 하기 위해서 설정한다. 
		//							모든 노드를 검색하면 1이 증가한다. 
		//							만약 무한루프중에서, 다시 시작지점으로 돌아오면 그 자리에서 끝으로 감지, 리턴한다
		type_count how_many_reach_end_of_nodeset = 0;

		//		검색:
		while(1)
		{	
			//		뒤늦은 포인터 이동:	현재 _searching_index는 이전 return했던 위치다. _searching_index++를 하지 않으면
			//							이전에 return했던 노드가 다시 return 되어버린다.
			_searching_index++;
			//		조건1:	해당 스크립트코드에는 더이상 NodeUnit이 없다면 (= 해당 스크립트코드의 마지막인덱스일경우 )
			NEShortCut& shortcut = _getShortCut();
			if( ! &shortcut || _searching_index > shortcut.getLengthLastIndex()	)
			{
				//		다음 스크립트코드셋으로 옮김:
				++_searching_code_index; // 이 변수는 this->_codes에 속해있다.
				//		처음상태:	해당 네임코드셋의 0번부터 읽는다
				_searching_index = -1; // 이 변수는 manager.shortcutset에 속해있다.	manager.shortcutset[ my_scriptcode ][ this->_searching_index ]
				//		조건1-1:	다음 네임코드셋이 없다면
				if(getCodesType() == NECodeType::ALL)
				{
					if (_searching_code_index > _getShortCutSet().getLengthLastIndex())
					{
						//		처음 스크립트코드셋으로 옮김:	끝에 도달했다.
						//			위치 처음으로 되돌리기:	왜, releaseNodePosition()을 사용하지 않는가?
						//				releaseNodePosition()에는 _count도 0으로 되돌리기 때문이다.
						_searching_code_index = 0;
						_searching_index = -1;
						//			끝에 한번도달했음을 알리기:
						how_many_reach_end_of_nodeset++;
					}
				}
				else
				{
					if (_searching_code_index > _codes.getLengthLastIndex())
					{	//		처음 스크립트코드셋으로 옮김:	끝에 도달했다.
						//			위치 처음으로 되돌리기:	왜, releaseNodePosition()을 사용하지 않는가?
						//				releaseNodePosition()에는 _count도 0으로 되돌리기 때문이다.
						_searching_code_index = 0;
						_searching_index = -1;
						//			끝에 한번도달했음을 알리기:
						how_many_reach_end_of_nodeset++;
					}
				}
			}
			else
			{		
				type_index real_node_index = shortcut[_searching_index]; // 일단 선택하고 다음에 선택할 인덱스를 할당한다.
				NEEnlistableManager& manager = getManager();
				if( ! &manager)

				{
					KERNAL_ERROR(" : ");
					return *null_pointer;
				}

				NENode& node = manager._getNodeSet().getElement(real_node_index);
				if(_isPassedFilter(&node))
				{
					_count++;
					return node;
				}
			}

			//	무한 루프 탐지:
			if(how_many_reach_end_of_nodeset >= 2)
				//	에러:	바로 탐색을 "실패" 로 종료한다.									
				return *null_pointer;
		}		
	}



	//	---------------------------------------------------------------------------------
	//	설명	:	노드를 Boundary 형식으로 검색한다.
	//				처음부터 조건에 맞는 노드를 전부 검색하되, 만약 마지막 인덱스에 다다르면
	//				널을 내보낸다.
	//	동작조건:
	//	메모	:
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NENode& NENodeSelector::_getNextByNoLimit() // 검색 스크립트코드목록 끝에서 true를 리턴한다
	{
		//	배경:		manager.shortcutset[ 스크립트 번호 ][ 0 ~ N ]
		//				의 형태로 NENode*이 들어있다.
		//				예를들어, manager.shortcutset[1][4]를 하면 
		//				1번스크립트를 가지고있는 4번째 NodeUnit이 반환된다.
		//	
		//				스크립트 코드는 중복이 없다.
		//				하나의 노드는 하나의 스크립트 코드만을 갖는다.

		//	pre:		
		//		널 레퍼런스:
		NENode* null_pointer = NE_NULL;
		//		타겟팅:
		//			숏컷셋:
		NEShortCutSet& shortcutset = _getShortCutSet();
		if( ! &shortcutset)
			return *null_pointer;
		//		관련 멤버변수 검사:
		//			검색 위치:
		const NECodeType& node_type = getCodesType();
		if(_searching_code_index < 0) return *null_pointer;
		if(	node_type != NECodeType::ALL								&&
			_searching_code_index > _codes.getLengthLastIndex()	) 
			return *null_pointer;


		//	main:		
		//		검색:
		while(1)
		{	
			//		뒤늦은 포인터 이동:	현재 _searching_index는 이전 return했던 위치다. _searching_index++를 하지 않으면
			//							이전에 return했던 노드가 다시 return 되어버린다.		
			_searching_index++;
			//		타입 추출:	검색하고자 하는 노드의 스크립트코드를 가져온다
			//			버퍼 정의:
			NEShortCut* shortcut = NE_NULL;			
			//			버퍼에 할당:
			if(node_type == NECodeType::ALL)
				shortcut = &(shortcutset.getElement(_searching_code_index));
			else
			{
				type_code code = _codes[_searching_code_index].getCode();
				shortcut = &(shortcutset.getElement(code));
			}			
			if( ! shortcut)
			{	//		에러:	끝에 도달했으므로 NULL을 리턴
				initializeNodeSelectorReferingPoint(); // 내부적으로 _count를 0으로 되돌리지만, _count을 사용하지 않으므로 상관없다.			
				return *null_pointer;
			}



			//		조건1:	해당 스크립트코드에는 더이상 NodeUnit이 없다면 (= 해당 스크립트코드의 마지막인덱스일경우 )
			if(_searching_index > shortcut->getLengthLastIndex()) // 해당 스크립트코드의 마지막인덱스일경우,
			{	//		다음 네임코드셋으로 옮김:	다음 네임코드목록중에서 비어있는지 여부를 판단한다.								
				++_searching_code_index;
				//		처음상태:	해당 네임코드셋의 0번부터 읽는다
				_searching_index = -1;

				if(node_type != NECodeType::ALL)
				{
					if(_searching_code_index > _codes.getLengthLastIndex())
					{
						initializeNodeSelectorReferingPoint();

						return *null_pointer;
					}
				}
				else
				{
					if(_searching_code_index > shortcutset.getLengthLastIndex())
					{
						initializeNodeSelectorReferingPoint();

						return *null_pointer;
					}
				}
			}	
			else
			{
				type_index real_node_index = shortcut->getElement(_searching_index); // 선택을 먼저하고, 그 뒤에 다음에 검색할인덱스를 할당한다
				NEEnlistableManager& manager = getManager();
				if( ! &manager)
				{
					KERNAL_ERROR(" : ");
					return *null_pointer;
				}					

				NENode& node = manager._getNodeSet().getElement(real_node_index);
				if(_isPassedFilter(&node))
					return node;
			}
		}
	}



	NEShortCutSet& NENodeSelector::_getShortCutSet()
	{
		//		타겟팅:
		NEEnlistableManager& manager = getManager();		
		if( ! &manager) 
		{
			NEShortCutSet* nullpointer = 0;
			return *nullpointer;
		}


		const NECodeType& type = getCodesType();
		switch(type.getCodeType())
		{
		case NECodeType::SCRIPT:
		case NECodeType::ALL:
		case NECodeType::NAME:
		case NECodeType::GROUP:
		case NECodeType::PRIORITY:
			return manager._getShortCutSet(type);

		default:
			KERNAL_ERROR(" : ");
			return manager._getShortCutSet(NECodeType::SCRIPT);
		}
	}


	const NEShortCutSet& NENodeSelector::_getShortCutSet() const
	{
		ThisClass* unconsted = const_cast<ThisClass*>(this);

		return unconsted->_getShortCutSet();
	}



	NENode& NENodeSelector::_getNodeByRecent()
	{
		//	pre:
		//		널 레퍼런스:
		NENode* nullpoint = NE_NULL;	
		//	카운트 검사:
		//		카운트 증가:	한번 실행마다 NE_NULL을 내보내야 한다.
		if(_count++ > _countlimit)
		{	//	2번이상 실행된 경우:
			initializeNodeSelectorReferingPoint();

			return *nullpoint;
		}
		else if(getCodesType() == NECodeType::ME)
		{
			NEEnlistableManager& manager = getManager();
			if( ! &manager)
			{
				KERNAL_ERROR(" : ");
				return *nullpoint;
			}
			if( ! manager.isSubClassOf(NETClass<NENodeManager>()))
			{
				KERNAL_ERROR(" : ");
				return *nullpoint;
			}
			NENodeManager& noder = static_cast<NENodeManager&>(manager);
			//	첫번째 실행:	노드를 반환한다	
			return noder.getLocalStack().getRecentNode();
		}
		else			
			return *_recent;
	}



	NENode& NENodeSelector::_getNodeByCode()
	{
		//		널 레퍼런스:
		NENode* null_pointer = NE_NULL;
		//	그밖의 타입:		
		if(getCodesType() != NECodeType::ALL && _codes.getLength() <= 0) return *null_pointer;

		//	main:
		//		반환할 노드의 포인터 생성:
		//		조건1:	한번에 특정 횟수 이상, 검색을 해야 한다면
		if(_countlimit > 0)
		{	//		계속검색 모드:	찾은 위치를 저장하고, 다음에 그 위치부터 검색을 실시한다
			//						지정된 횟수만큼 찾을때까지는 NULL을 반환하지 않는다.
			//						예)	count=3	일 경우, 
			//							2번째 실행시 모든 노드를 다 검색했다면
			//							3번째 실행시는 처음으로 다시 돌아가, 이미 검색한 노드를 다시 반환한다.
			NENode& selected = _getNextByLimit();
			//		조건1-1:	찾은 노드의 포인터가 NULL 이라면
			if( ! &selected )
			{	//		에러 반환:	최종적으로 더미노드를 반환한다.
				_count = 0;

				return *null_pointer;
			}
			else
			{	//		노드 반환:
				return selected;
			}
		}
		else
		{	//		일반검색 모드:	찾은 위치를 저장하고, 다음에 그 위치부터 검색을 실시한다
			//						인덱스0부터 검색해서, 끝에 도달하면 NULL을 반환한다.
			NENode& selected = _getNextByNoLimit();
			//			조건2-1:	찾은 노드가 NULL 이라면 (= 모든 노드를 다 검색했다 )
			if( ! &selected )
			{	//			더미노드 반환:	끝에 도달했다는 정보를 전달한다
				//				검색 위치 정보 초기화:
				initializeNodeSelectorReferingPoint();
				//				더미노드 반환:
				return *null_pointer;
			}
			else
			{	//			노드 반환:
				return selected;
			}
		}
	}



	type_bool NENodeSelector::_isPassedFilter(NENode* extracted_node) const
	{
		//	pre:
		//		타입판단: 그룹 타입에서만 필터링이 존재한다.
		if(!extracted_node) return true;
		if(getCodesType() != NECodeType::GROUP) return true;



		//	main:
		//		use_and_operation 분기:	and를 사용할 경우와 아닐경우의 필터링이 달라진다.
		if(_use_and_operation)
		{	//		사용하는 경우:	추출한 노드가 셀렉터가 가진 코드를 모두 가지고 있어야 한다. (= 교집합)
			for(type_index n=0; n < _codes.getLength() ;n++)
				//	그룹코드 추출:
				//	검사:	코드가 없다면, 필터링 통과 실패 (= 모든 코드를 가지고 있어야 한다.
				if(extracted_node->getGroupCodes().find(_codes[n]) == NE_INDEX_ERROR)
					return false;
		}
		else
		{	//		사용하지 않는 경우:	이때에는 위와 반대로, 노드가 중첩된 코드를 가지고 있는지 검사해야한다.
			//	배경:	OR연산일때, 중첩된 그룹코드를 가지고 있는 노드는 여러번 반환된다.
			//			예를들어, 
			//			1, 3, 5 그룹코드를 가지고 있는 노드와 2 그룹코드를 가지고 있는 노드가 있을때
			//			OR연산으로 1,2,3을 검색한 경우,
			//			두번재 노드는 1번만 검색되지만, 첫번째 노드는 1일때와 3일때 두번 검색된다.
			//			이걸 막고자 한다.
			//
			//			원리는, 그룹코드를 0부터 순차적으로 검색한다는 점을 통해 이루어진다.
			//			노드가 들어왔을 때, 0 ~ 최근에 인덱스-1 까지의 그룹코드가 해당 노드에 있는지를 
			//			확인함으로써 해결이 가능하다.
			const NECodeSet& groupcode = extracted_node->getGroupCodes();
			for(type_index n=0; n < _searching_code_index ;n++) // **주의** _searching_groupcode_index - 1 까지 루프를 도는것에 주목하자
				if(groupcode.find(_codes[n]) > NE_INDEX_ERROR) // 일치되는 코드를 이미 갖고 있다면(=이미 검색했던 노드라면), (아무것도 없다면 -1이 나온다)
					return false;
			//		그리고 현재 탐색중인 _searching_code_index가 존재해야한다.
			if(groupcode.find(_codes[_searching_code_index]) <= NE_INDEX_ERROR)
				return false;
		}



		//	post:
		return true;
	}


	const NENodeSelector& NENodeSelector::_assign(const NENodeSelector& source)
	{
		if(this == &source) return *this;

		_searching_index = source._searching_index;
		_searching_code_index = source._searching_code_index;
		_manager_type = source._manager_type;
		_binder = source._binder;
		_use_auto_binding = source._use_auto_binding;
		_is_returned_once = source._is_returned_once;

		return *this;
	}

	NEEnlistableManager& NENodeSelector::getManager()
	{
		//	pre:
		NEEnlistableManager* nullpointer = NE_NULL;
		if( ! isManagerTypeValid(_manager_type))
		{
			KERNAL_ERROR(" : 주어진 Manager의 타입이 적절하지 않습니다.");
			return *nullpointer;
		}
		Kernal& kernal = Kernal::getInstance();
		if( ! &kernal)
		{
			KERNAL_ERROR(" : 커널이 없습니다.");
			return *nullpointer;
		}

		return _interface(_manager_type);
	}

	const NEEnlistableManager& NENodeSelector::getManager() const
	{
		//	pre:
		const NEEnlistableManager* nullpointer = NE_NULL;
		if( ! isManagerTypeValid(_manager_type))
		{
			KERNAL_ERROR(" : 주어진 Manager의 타입이 적절하지 않습니다.");
			return *nullpointer;
		}
		const Kernal& kernal = Kernal::getInstance();
		if( ! &kernal)
		{
			KERNAL_ERROR(" : 커널이 없습니다.");
			return *nullpointer;
		}

		return _interface(_manager_type);
	}

	type_result NENodeSelector::setManager(NEType::Type type)
	{
		if( ! isManagerTypeValid(type))
		{
			KERNAL_ERROR(" : 변경하려는 Manager 타입이 적절하지 않습니다.\n작업을 취소합니다.\n\t변경하려던 manager의 타입 : %d", type);
			return RESULT_TYPE_ERROR | RESULT_ABORT_ACTION;
		}



		//	main:
		_manager_type = type;

		return RESULT_SUCCESS;
	}

	NEType::Type NENodeSelector::getManagerType() const
	{
		return _manager_type;
	}

	type_bool NENodeSelector::isManagerTypeValid(NEType::Type type) const
	{
		if(	type == NEType::NENODE_MANAGER		||
			type == NEType::NESCRIPT_MANAGER	||
			type == NEType::NESCRIPT_EDITOR	)
			return true;

		return false;
	}

	type_result NENodeSelector::initializeReferingPoint()
	{
		return initializeNodeSelectorReferingPoint();
	}

	type_result NENodeSelector::bind()
	{
		//	pre:
		//		노드매니져:
		NEEnlistableManager& manager = getManager();
		if( ! &manager)
		{
			KERNAL_ERROR(" : ");
			return RESULT_TYPE_ERROR;
		}		
		//		RECENT 방식인지 검사:
		const NECodeType& node_type = getCodesType();
		if(node_type == NECodeType::UNDEFINED)
		{
			KERNAL_ERROR(" : ")
				return RESULT_TYPE_ERROR;
		}
		//		멤버변수 검사:
		if(_searching_index <= -1) return RESULT_TYPE_ERROR;



		//	main:
		//		Real Index 획득:
		if(node_type == NECodeType::RECENT)
		{
			if(manager.isSubClassOf(NEType::NENODE_MANAGER))
			{
				NENodeManager& noder = static_cast<NENodeManager&>(manager);
				NENodeManager::LocalStack& localstack = noder.getLocalStack();
				_binder._real_index = localstack.getRecentNodeRealIndex();
				_binder._comparing_id = localstack.getRecentNode().getId();
				return RESULT_SUCCESS;
			}

			KERNAL_ERROR(" : RECENT 타입을 바인딩할때는 manager가 노드 매니져여야만 가능합니다.");
			return RESULT_TYPE_ERROR;
		}

		NEShortCut* shortcut = 0x00;

		if (node_type == NECodeType::ALL)
			shortcut = &_getShortCutSet()[_searching_code_index];
		else
		{
			type_code code = _codes[_searching_code_index].getCode();
			shortcut = &_getShortCutSet().getElement(code);
		}
		if( ! shortcut)
			return KERNAL_ERROR(" : ShortCut 획득 실패");

		//		Binder 정의:
		//			Real Index:
		type_index real_index = shortcut->getElement(_searching_index);
		_binder._real_index = real_index;
		//			Id:
		NENode& node = manager._getNodeSet().getElement(real_index);
		if( ! &node)
		{
			_binder.release();
			return RESULT_TYPE_ERROR;
		}
		_binder._comparing_id = node.getId();



		//	post:
		//		Binder 초기화:
		_is_returned_once = false;
		_binder._manager_type = _manager_type;
		return RESULT_SUCCESS;
	}

	NENodeBinder& NENodeSelector::getBinder()
	{
		return _binder;
	}

	const NENodeBinder& NENodeSelector::getBinder() const
	{
		return _binder;
	}

	NENode& NENodeSelector::_getNodeByBinder()
	{
		NENode* nullpoint = NE_NULL;

		if(_is_returned_once)
		{
			_is_returned_once = false;
			return *nullpoint;
		}


		_is_returned_once = true;
		return _binder.getBinded();		
	}

	type_bool& NENodeSelector::isUsingAutoBinding()
	{
		return _use_auto_binding;
	}

	type_bool NENodeSelector::isUsingAutoBinding() const
	{
		return _use_auto_binding;
	}

	NEShortCut& NENodeSelector::_getShortCut()
	{
		if(getCodesType() == NECodeType::ALL)
			return _getShortCutSet()[_searching_code_index];

		const NECode& code = _codes[_searching_code_index];
		if( ! &code || code.getCode() == -1)
		{
			NEShortCut* nullpointer = 0x00;
			return *nullpointer;
		}

		return _getShortCutSet()[code.getCode()];
	}
	const NEShortCut& NENodeSelector::_getShortCut() const
	{
		ThisClass* unconsted = const_cast<ThisClass*>(this);

		return unconsted->_getShortCut();
	}
}