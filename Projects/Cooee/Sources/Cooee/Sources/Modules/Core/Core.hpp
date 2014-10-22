#pragma once

#include "../Commander/Commander.hpp"
#include "../ModuleSetTerminal/ModuleSetTerminal.hpp"

class Core
{
public:	
	class onObjectFound
	{
	public:
		virtual void onKeyFound(NEKey& target) {}
		virtual void onNodeFound(NENode& target) {}
		virtual void onModuleFound(NEModule& module) {}
	};

	static NEObject& getCurrentObjectFromPath()
	{
		return getObjectBy(path);
	}
	static type_result setPathToBeCopied(const NEString& new_path)
	{
		NEObject& obj = getObjectBy(new_path);
		if( ! &obj)
			return RESULT_TYPE_WARNING | RESULT_ABORT_ACTION;

		path_to_be_copied = new_path;

		is_cutting_off = false;
		return RESULT_SUCCESS;
	}
	static type_result setPathToBeCutOff(const NEString& new_path)
	{
		type_result r = setPathToBeCopied(new_path);
		if( ! NEResult::hasError(r))
			is_cutting_off = true;

		return r;
	}
	static void pushMessage(const NEString& msg)
	{
		::LG::Core::windows.pushFront(LG::MessageWindow(msg, WHITE, LIGHTRED));
	}
	static void openModifierFrom(const NEString& path, NEKey* real_key = 0);
	static void openModifierFrom(NEKey& key);
	static type_index _getPositionTypeAndIndex(const NEString& position, int& type)
	{
		int index_to_be_returned = position.getLength() > 1 ? position.extract(0, position.getLengthLastIndex()).toInt() : NE_INDEX_ERROR;

		switch(position[0])
		{
		case 'n':	type = 1;	break;	// node
		case 'm':	type = 2;	break;	// module
		case 'k':	type = 3;	break;	// key
		default:	type = 0;			// auto
		}

		return index_to_be_returned;
	}

	static NEObject& _searchModuleSet(NEModuleCodeSet& moduleset, NEStringSet& work_position, onObjectFound& handler = onObjectFound())
	{
		//	pre:
		NEObject* null_pointer = NE_NULL;	
		if( ! &work_position)
		{
			pushMessage("작업위치 문자열이 없습니다.");
			return *null_pointer;
		}
		if(work_position.getLength() <= 0)
			return moduleset;
		NEString& position = work_position[0];
		int type = 0; // 0은 unknown을 의미한다.
		int index = _getPositionTypeAndIndex(position, type);
		if(type != 0 && type != 2)
		{
			pushMessage(NEString("잘못된 위치 문자열입니다 : " + position + "타입이 모듈이 아닙니다."));
			return *null_pointer;
		}
		if(index < 0 || index > moduleset.getLengthLastIndex())
		{
			pushMessage("잘못된 인덱스. 0보다 작거나, 모듈셋의 크기를 넘습니다.");
			return *null_pointer;
		}



		//	main:
		NEModule& module = moduleset[index];
		handler.onModuleFound(module);
		work_position.popFront();			
		if(work_position.getLength() > 0)
			pushMessage("어라? 모듈은 경로가 마지막에 해당하는데, 뒤에 또 뭐가 있네요? 경로가 꼬인듯?\n일단, 모듈만 반환합니다.");

		return module;
	}

	static NEObject& _searchKeySet(NEKeyCodeSet& keyset, NEStringSet& work_position, onObjectFound& handler = onObjectFound())
	{
		//	pre:
		NEObject* null_pointer = NE_NULL;	
		if( ! &work_position)
		{
			pushMessage("작업위치 문자열이 없습니다.\n");
			return keyset;
		}
		if(work_position.getLength() <= 0)
			return keyset;
		NEString& position = work_position[0];
		int type = 0; // 0은 unknown을 의미한다.
		int index = position.toInt();


		//	main:
		NEKey& key = keyset[index];
		handler.onKeyFound(key);

		work_position.popFront();
		if(key.isSubClassOf(NEType::NEMODULE_CODESET_KEY))
		{
			NEModuleCodeSetKey& modulesetkey = static_cast<NEModuleCodeSetKey&>(key);
			NEModuleCodeSet& moduleset = modulesetkey.getValue();

			return _searchModuleSet(moduleset, work_position, handler);
		} 
		else if(key.isSubClassOf(NEType::NENODE_CODESET_KEY))
		{
			NENodeCodeSetKey& managed_nodesetkey = static_cast<NENodeCodeSetKey&>(key);
			NENodeCodeSet& nodeset = managed_nodesetkey.getValue();

			return _searchNodeSet(nodeset, work_position, handler);
		}

		return key; 
	}

	static NEObject& _searchNodeSet(NENodeCodeSet& nodeset, NEStringSet& work_position, onObjectFound& handler = onObjectFound())
	{
		//	pre:
		NEObject* null_pointer = NE_NULL;	
		if( ! &work_position)
		{
			pushMessage("작업위치 문자열이 없습니다.");
			return *null_pointer;
		}
		if(work_position.getLength() <= 0)
			return nodeset;
		NEString& position = work_position[0];
		int type = 0; // 0은 unknown을 의미한다.
		int index = _getPositionTypeAndIndex(position, type);
		if(type != 0 && type != 1)
		{
			pushMessage(NEString("잘못된 위치 문자열입니다 : " + position + "타입이 노드가 아닙니다."));
			return *null_pointer;
		}
		if(index < 0 || index > nodeset.getLengthLastIndex())
		{
			pushMessage("잘못된 인덱스. 0보다 작거나, 노드셋의 크기를 넘습니다.");
			return *null_pointer;
		}



		//	main:
		NENode& node = nodeset[index];
		handler.onNodeFound(node);

		work_position.popFront();
		if(work_position.getLength() > 0)
		{
			NEModuleCodeSet& moduleset = node.getModuleSet();

			return _searchModuleSet(moduleset, work_position);
		}
		else
			return node;
	}
	static NEObject& getObjectBy(const NEString& path, onObjectFound& handler = onObjectFound());
	static NEString createPathBy(const NEObject& target);

	static Commander commander;
	static NEString path;
	static NEString path_to_be_copied;
	static bool is_cutting_off;
};