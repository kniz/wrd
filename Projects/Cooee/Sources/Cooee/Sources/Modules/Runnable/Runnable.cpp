#include "Runnable.hpp"

Runnable* Runnable::_runnable = NE_NULL;

namespace 
{
	void print(const NEShortCutSet& set);
}

void Runnable::_printPrompt() const
{	
	cout << NETString(_current_work_position + " > ");
}

void Runnable::_analyzeCommandAndBranch(const NETString& commandline)
{
	NETStringSet splited;
	commandline.split(" ", splited);
	if(splited.getLength() <= 0) return;

	NETString command = splited[0];

	Interface& keyword = _commander.getInterface(command);
	if(! &keyword)
		return printError(_T("알수 없는 명령어. 명령어를 알고 싶다면 help를 사용하세요."));
	
	
		
	//	post:	
	splited.popFront();
	type_result result = keyword.run(splited);
	
	return _analyzeBranchResult(command, result);		
}

NETString Runnable::_fetchCommandLine() const
{
	TCHAR buffer[_MAX_BUFFER];

	TIN.getline(buffer, _MAX_BUFFER);

	return buffer;
}

void Runnable::printError(LPCTSTR contents) const
{
	cout << "\tERROR! " << contents << endl;
}

void Runnable::printError(const NETString& contents) const
{
	cout << "\tERROR! " << contents << endl;
}

void Runnable::_analyzeBranchResult(const NETString& command, type_result result) const
{
	if( ! NEResult::hasError(result)) return;
	
	
	
	NETString message = "\t at " + command + "\tcode :\t" + NETString((int)result);

	return printError(message);
}

NETString& Runnable::getCurrentWorkPosition()
{
	return _current_work_position;
}

const NETString Runnable::getCurrentWorkPosition() const
{
	return _current_work_position;
}

bool Runnable::isStillRunning() const
{
	return _is_still_running;
}

void Runnable::setRunning( bool runnable )
{
	_is_still_running = runnable;
}

Runnable::Runnable()
{
	_current_work_position = "/";
	_is_still_running = true;
	_kernal = NE_NULL;
	_editor = NE_NULL;
}

Commander& Runnable::getCommander()
{
	return _commander;
}

const Commander& Runnable::getCommander() const
{
	return _commander;
}

void Runnable::execute()
{
	_initialize();
	
	Interface& show_version = _commander.getInterface("version");
	if( &show_version)
		show_version.run(NETStringSet());

	while(isStillRunning())
	{
		_printPrompt();
		NETString& commandline = _fetchCommandLine();
		_analyzeCommandAndBranch(commandline);

		cout << endl;
	}
}

void Runnable::printModuleHeader(const NEExportable::ModuleHeader& header) const
{
	_printHeader(header);
	const NETStringSet& dependencies = header.getModuleDependencies();
	for(int dep_n=0; dep_n < dependencies.getLength() ;dep_n++)
	{
		cout << dependencies[dep_n] << endl;
		if(dep_n != dependencies.getLengthLastIndex())
			cout << _T("\t\t\t\t");
	}
	
	cout	<< _T("\n\t버전호환\t:\t");
	const NEIntSet& versionset = header.getRevisionCompatibilities();
	for(int ver_n=0; ver_n < versionset.getLength() ;ver_n++)
		cout << versionset[ver_n] << " ";

	cout << endl;
}

void Runnable::_initialize()
{
	cout << _T("부팅을 시작합니다...\n");
	cout << _T("팩토리 생성...\n");
	Kernal::Factory factory;
	
	cout << _T("\t팩토리 설정에 들어갑니다...\n");
	Kernal::ModifiableSettings& settings = factory.getDefaultSettings();
	cout << _T("\t디폴트 세팅 입력...\n");
	
	settings.getINIFullPath() = _T("./NodeCUI.ini");
	settings.setModuleDirectory(_T(".\\Modules"));
	settings.setLogDirectory(_T(".\\Logs"));
	settings.setScriptDirectory(_T(".\\Scripts"));
	settings.setNodeDirectory(_T(".\\Nodes"));

#ifdef DEVELOPER
#	ifdef _DEBUG
	cout << _T("\t매니져 모듈 식별자 할당...\n\tNodeManager\t:\tNEStandardNodeManager\n\tScriptManager\t:\tNEStandardScriptManager\n");
	factory.getNodeManagerIdentifier() = NEExportable::Identifier(_T("NEStandardNodeManager"), _T("haku"), 1);
	factory.getScriptManagerIdentifier() = NEExportable::Identifier(_T("NEStandardScriptManager"), _T("haku"), 1);
#	else 
	cout << _T("\t매니져 모듈 식별자 할당...\n\tNodeManager\t:\tNEStandardNodeManager\n\tScriptManager\t:\tNEStandardScriptManager.dev\n");
	factory.getNodeManagerIdentifier() = NEExportable::Identifier(_T("NEStandardNodeManager"), _T("haku"), 1);
	factory.getScriptManagerIdentifier() = NEExportable::Identifier(_T("NEStandardScriptManager"), _T("haku"), 1);
#	endif
#endif
	cout << _T("\t변경 여부 확인...\n");
	while(true)
	{
		cout	<< "\t\t" << settings.getINIFullPath() << endl
				<< "\t\t" << settings.getModuleDirectory() << endl
				<< "\t\t" << settings.getLogDirectory() << endl
				<< "\t\t" << settings.getScriptDirectory() << endl
				<< "\t\t" << settings.getNodeDirectory() << endl;
			
		cout << _T("\t\t변경 합니까? (Y / N)");
		NETString& inputed = _fetchCommandLine();
		if(inputed == _T("Y") || inputed == _T("y"))
		{
			cout << endl << endl;
			cout << _T("\t\t새로운 INI 파일명과 경로(skip 입력시 통과) : \n\t\t");
			NETString& new_inipath = _fetchCommandLine();
			if(new_inipath != _T("skip"))
				settings.getINIFullPath() = new_inipath;
				
			cout << _T("\t\t새로운 모듈 디렉토리(skip 입력시 통과) : \n\t\t");
			NETString& new_module = _fetchCommandLine();
			if(new_module != _T("skip"))
				settings.setModuleDirectory(new_module);
				
			cout << _T("\t\t새로운 로그 디렉토리(skip 입력시 통과) : \n\t\t");
			NETString& new_log = _fetchCommandLine();
			if(new_log != _T("skip"))
				settings.setLogDirectory(new_log);
				
			cout << _T("\t\t새로운 스크립트 디렉토리(skip 입력시 통과) : \n\t\t");
			NETString& new_script = _fetchCommandLine();
			if(new_script != _T("skip"))
				settings.setScriptDirectory(new_script);
				
			cout << _T("\t\t새로운 노드 디렉토리(skip 입력시 통과) : \n");
			NETString& new_node = _fetchCommandLine();
			if(new_node != _T("skip"))
				settings.setNodeDirectory(new_node);
		}
		if(inputed == _T("N") || inputed == _T("n"))
			break;
	}
	
	cout << _T("\t팩토리 설정 완료...\n");
	cout << _T("커널 생성...\n커널 할당\n");
	
	_kernal = new Kernal(factory.create());
	
	Kernal::setInstance(*_kernal);
	
	cout << _T("커널을 초기화합니다.\n");
	Kernal::getInstance().initialize();

	cout << _T("초기화를 완료했습니다.\n에디터를 초기화합니다.");
	_fetchCommandLine();
	
	//////////////////////////////////////////////////////////////////////////
	
	
	cout << _T("에디터 팩토리 생성...\n");
	Editor::Factory editor_factory;

	cout << _T("\t매니져 모듈 식별자 할당...\n\tNEPanelManager\t:\tNEStandardPanelManager\n\tScriptEditer\t:\tNEStandardScriptEditer\n\tNEEventHandler\t:\tCooeeHandler\n");
#ifdef DEVELOPER
#	ifdef _DEBUG
	editor_factory.getPanelManagerIdentifier() = NEExportable::Identifier(_T("NEStandardPanelManager"), _T("haku"), 1);
	editor_factory.getScriptEditorIdentifier() = NEExportable::Identifier(_T("NEStandardScriptEditor"), _T("haku"), 1);
	editor_factory.getEventHandlerIdentifier() = NEExportable::Identifier(_T("CooeeHandler"), _T("haku"), 1);
#	else
	editor_factory.getPanelManagerIdentifier() = NEExportable::Identifier(_T("NEStandardPanelManager"), _T("haku"), 1);
	editor_factory.getScriptEditorIdentifier() = NEExportable::Identifier(_T("NEStandardScriptEditor"), _T("haku"), 1);
	editor_factory.getEventHandlerIdentifier() = NEExportable::Identifier(_T("CooeeHandler"), _T("haku"), 1);
#	endif
#endif

#ifdef DEVELOPER
#	ifdef _DEBUG
	editor_factory.getDefaultSettings().setPanelDirectory(_T(".\\Panels"));
#	else 
	editor_factory.getDefaultSettings().setPanelDirectory(_T(".\\Panels"));
#	endif
#endif
	editor_factory.getDefaultSettings().setLoadFromINI(true);
	

	cout << _T("에디터 팩토리 생성 완료...\n");
	cout << _T("에디터 생성...\n에디터 할당\n");
	
	_editor = new Editor(editor_factory.create());

	Editor::setInstance(*_editor);

	cout << _T("에디터를 초기화합니다.\n");
	cout << endl;
	Editor::getInstance().initialize();

	NEScriptHeader& header = Editor::getInstance().getScriptEditor().getScriptHeader();
	header.getName() = _T("unnamed");
	header.getDeveloper() = _T("kniz_test");
	header.setRevision(1);
	cout << _T("초기화를 완료했습니다.\n부팅 완료.\n");
	cout << _T("아무키나 누르세요 : ");
	_fetchCommandLine();

	_commander.getInterface("clear").run(NETStringSet());	
}

Runnable::~Runnable()
{
	if(_editor)
		delete _editor;
	if(_kernal)	
		delete _kernal;
}

NEObject& Runnable::getObjectFrom(const NETString& work_position) const
{
	NEObject* null_pointer = NE_NULL;	
	Editor& editor = Editor::getInstance();
	if( ! &editor)
	{
		printError(_T("Editor가 없습니다."));
		return *null_pointer;
	}
	NEScriptEditor& scripted = editor.getScriptEditor();
	if( ! &scripted)
	{
		printError(_T("ScriptEditor가 없습니다."));
		return *null_pointer;
	}
	NERootNodeCodeSet& nodelist = scripted.getScriptNodes();	
	if( ! &work_position)
	{
		return nodelist;
	}
	
	NETStringSet splited;
	work_position.split("/", splited);
	if(splited.getLength() < 1)
		return nodelist;
	
	
	NETString& position = splited[0];
	int type = 0; // 0은 unknown을 의미t한다.
	int index = _getPositionTypeAndIndex(position, type);
	
	if(type != 0 && type != 1)
	{
		NETString message = "잘못된 위치 문자열입니다 : " + position + "타입이 Node가 아닙니다.";
		printError(message);
		return *null_pointer;
	}
	
	
	if(index < 0 || index > nodelist.getLengthLastIndex())
	{
		printError(_T("잘못된 인덱스. 0보다 작거나, root의 크기를 넘습니다."));
		return *null_pointer;
	}
	
	NENode& node = nodelist[index];
	
	splited.popFront();
	if(splited.getLength() > 0)
	{
		NEModuleCodeSet& moduleset = node.getModuleSet();
		
		return _searchModuleSet(moduleset, splited);
	}
	else
		return node;
}

type_index Runnable::_getPositionTypeAndIndex(const NETString& position, NE_OUT int& type) const
{
	type = 0; // unknown

	if(position.extract(0,3) == _T("node")) 
	{
		type = 1; // 1은 node를 의미한다.
		return position.extract(4, position.getLengthLastIndex()).toInt();
	} 
	else if(position.extract(0, 5) == _T("module"))
	{
		type = 2; // 2는 module를 의미한다.
		return position.extract(6, position.getLengthLastIndex()).toInt();
	}
	else if(position.extract(0, 2) == _T("key"))
	{
		type = 3;
		return position.extract(3, position.getLengthLastIndex()).toInt();
	}
	else
	{
		type = 0;
		return position.toInt();
	}
}

NEObject& Runnable::_searchModuleSet(NEModuleCodeSet& moduleset, NETStringSet& work_position ) const
{
	//	pre:
	NEObject* null_pointer = NE_NULL;	
	if( ! &work_position)
	{
		printError(_T("작업위치 문자열이 없습니다."));
		return *null_pointer;
	}
	if(work_position.getLength() <= 0)
	{
		printError(_T("문자열이 잘못되었습니다. 제대로 /를 사용해서 위치를 구분했나요?"));
		return *null_pointer;
	}
	NETString& position = work_position[0];
	int type = 0; // 0은 unknown을 의미한다.
	int index = _getPositionTypeAndIndex(position, type);
	if(type != 0 && type != 2)
	{
		NETString message = "잘못된 위치 문자열입니다 : " + position + "타입이 모듈이 아닙니다.";
		printError(message);
		return *null_pointer;
	}
	if(index < 0 || index > moduleset.getLengthLastIndex())
	{
		printError(_T("잘못된 인덱스. 0보다 작거나, 모듈셋의 크기를 넘습니다."));
		return *null_pointer;
	}



	//	main:
	NEModule& module = moduleset[index];

	work_position.popFront();
	if(work_position.getLength() > 0)
	{
		NEKeyCodeSet& keyset = module.getKeySet();

		return _searchKeySet(keyset, work_position);
	}
	else
		return module;
}

NEObject& Runnable::_searchKeySet( NEKeyCodeSet& keyset, NETStringSet& work_position ) const
{
	//	pre:
	NEObject* null_pointer = NE_NULL;	
	if( ! &work_position)
	{
		printError(_T("작업위치 문자열이 없습니다."));
		return *null_pointer;
	}
	if(work_position.getLength() <= 0)
	{
		printError(_T("문자열이 잘못되었습니다. 제대로 /를 사용해서 위치를 구분했나요?"));
		return *null_pointer;
	}
	NETString& position = work_position[0];
	int type = 0; // 0은 unknown을 의미한다.
	int index = _getPositionTypeAndIndex(position, type);
	if(type != 0 && type != 3)
	{
		NETString message = "잘못된 위치 문자열입니다 : " + position + "타입이 키가 아닙니다.";
		printError(message);
		return *null_pointer;
	}
	if(index < 0 || index > keyset.getLengthLastIndex())
	{
		printError(_T("잘못된 인덱스. 0보다 작거나, 키셋의 크기를 넘습니다."));
		return *null_pointer;
	}



	//	main:
	NEKey& key = keyset[index];

	work_position.popFront();
	if(work_position.getLength() > 0)
	{
		if(key.isSubClassOf(NEType::NEMODULE_CODESET_KEY))
		{
			NEModuleCodeSetKey& modulesetkey = static_cast<NEModuleCodeSetKey&>(key);
			NEModuleCodeSet& moduleset = modulesetkey.getValue();
			
			return _searchModuleSet(moduleset, work_position);			
		} 
		else if(key.isSubClassOf(NEType::NENODE_CODESET_KEY))
		{
			NENodeCodeSetKey& managed_nodesetkey = static_cast<NENodeCodeSetKey&>(key);
			NENodeCodeSet& nodeset = managed_nodesetkey.getValue();
			
			return _searchNodeSet(nodeset, work_position);
		}
		else
		{
			printError(_T("잘못된 인덱스. 타입은 맞았지만, 키가 콜렉터가 아닙니다."));
			return *null_pointer;
		}
	}
	else
		return key;
}

NEObject& Runnable::_searchNodeSet( NENodeCodeSet& nodeset, NETStringSet& work_position ) const
{
	//	pre:
	NEObject* null_pointer = NE_NULL;	
	if( ! &work_position)
	{
		printError(_T("작업위치 문자열이 없습니다."));
		return *null_pointer;
	}
	if(work_position.getLength() <= 0)
	{
		printError(_T("문자열이 잘못되었습니다. 제대로 /를 사용해서 위치를 구분했나요?"));
		return *null_pointer;
	}
	NETString& position = work_position[0];
	int type = 0; // 0은 unknown을 의미한다.
	int index = _getPositionTypeAndIndex(position, type);
	if(type != 0 && type != 1)
	{
		NETString message = "잘못된 위치 문자열입니다 : " + position + "타입이 노드가 아닙니다.";
		printError(message);
		return *null_pointer;
	}
	if(index < 0 || index > nodeset.getLengthLastIndex())
	{
		printError(_T("잘못된 인덱스. 0보다 작거나, 노드셋의 크기를 넘습니다."));
		return *null_pointer;
	}



	//	main:
	NENode& node = nodeset[index];

	work_position.popFront();
	if(work_position.getLength() > 0)
	{
		NEModuleCodeSet& moduleset = node.getModuleSet();

		return _searchModuleSet(moduleset, work_position);
	}
	else
		return node;
}

void Runnable::printListFrom(const NEObject& object ) const
{
	if(! &object)
		return printError(_T("잘못된 객체입니다."));
	const Editor& editor = Editor::getInstance();
	if( ! &editor)
		return printError(_T("에디터가 없습니다."));
	const NEScriptEditor& scripted = editor.getScriptEditor();
	if( ! &scripted)
		return printError(_T("스크립트 에디터가 없습니다."));
	const NEScriptEditor::Banks& banks = scripted.getBanks();
	const NETStringList& scriptbank = banks.getBank(NECodeType::SCRIPT);
	
	
	
	if(object.isSubClassOf(NEType::NENODE_CODESET))
	{
		const NENodeCodeSet& collector = static_cast<const NENodeCodeSet&>(object);
		
		printNodeCollector(collector);
	}
	else if(object.isSubClassOf(NEType::NENODE_CODESET_KEY))
	{
		NENodeCodeSetKey& nsk = (NENodeCodeSetKey&) object;
		printNodeCollector(nsk.getValue());
	}
	else if(object.isSubClassOf(NEType::NEMODULE_CODESET_KEY))
	{
		const NEModuleCodeSetKey& msk = static_cast<const NEModuleCodeSetKey&>(object);
		const NEModuleCodeSet& moduleset = msk.getValue();

		cout	<< _T("moduleset\n")
				<< _T("n\tid\tnamecode\tmodule_name\n==================================================\n");
		for(int n=0; n < moduleset.getLength() ;n++)
		{
			const NEModule& module = moduleset[n];

			cout << n << _T("\t") << module.getScriptCode() << _T("\t") << module.getNameCode() << _T("\t") << module.getHeader().getName() << endl;
		}
	}
	else if(object.isSubClassOf(NEType::NEMODULE_CODESET))
	{
		const NEModuleCodeSet& moduleset = static_cast<const NEModuleCodeSet&>(object);

		cout	<< _T("moduleset\n")
				<< _T("n\tid\tnamecode\tdmodule_name\n==================================================\n");
		for(int n=0; n < moduleset.getLength() ;n++)
		{
			const NEModule& module = moduleset[n];

			cout << n << _T("\t") << module.getScriptCode() << _T("\t") << module.getNameCode() << _T("\t") << module.getHeader().getName() << endl;
		}
	}
	else if(object.isSubClassOf(NEType::NEKEY_CODESET))
	{
		const NEKeyCodeSet& keyset = static_cast<const NEKeyCodeSet&>(object);

		cout	<< _T("keyset\n")
				<< _T("n\tkeytype_string\tvalue\n===========================================\n");
		for(int n=0; n < keyset.getLength() ;n++)
		{
			const NEKey& key = keyset[n];

			cout << n << _T("\t") << key.getTypeName() << _T("\t");
			printKey(key);
			cout << endl;
		}
	}
	else
		return printError(_T("예상치 못한 타입입니다."));
}

void Runnable::printNode(const NENode& node) const
{
	Editor& editor = Editor::getInstance();
	if( ! &editor)
	{
		return printError(_T("에디터가 없습니다."));		
	}
	NEScriptEditor& scripted = editor.getScriptEditor();
	if( ! &scripted)
	{
		return printError(_T("스크립트 에디터가 없습니다."));		
	}
	NEScriptEditor::Banks& banks = scripted.getBanks();

	const TCHAR* is_enable = node.isEnable() ? _T("enable") : _T("disable");

	cout	<< _T("node(") << is_enable << _T(")\n")
			<< _T("\tid\t\t:\t") << node.getScriptCode() << _T("\t");			
	if(node.getScriptCode() >= 0 && node.getScriptCode() <= banks.getBank(NECodeType::SCRIPT).getLengthLastIndex())
		cout << banks.getBank(NECodeType::SCRIPT)[node.getScriptCode()] << endl;

	cout	<< _T("\tnamecode\t:\t") << node.getNameCode() << _T("\t");
	if(node.getNameCode() >= 0 && node.getNameCode() <= banks.getBank(NECodeType::NAME).getLengthLastIndex())
		cout << banks.getBank(NECodeType::NAME)[node.getNameCode()] << endl;

	cout	<< _T("\tpriority\t:\t") << node.getPriorityCode() << _T("\t");
	if(node.getPriorityCode() >= 0 && node.getPriorityCode() <= banks.getBank(NECodeType::PRIORITY).getLengthLastIndex())
		cout << banks.getBank(NECodeType::PRIORITY)[node.getPriorityCode()] << endl;

	cout	<< _T("\tgroupcode\t:\n");
	const NEIntSet& groupcodeset = node.getGroupCodes();
	for(int n=0; n < groupcodeset.getLength() ;n++)
	{
		type_code groupcode = groupcodeset[n];

		cout << _T("\t\t") << groupcode;

		if(groupcode >= 0 && groupcode <= banks.getBank(NECodeType::GROUP).getLengthLastIndex())
			cout << _T("\t") << banks.getBank(NECodeType::GROUP)[groupcode];

		cout << endl;
	}
	cout	<< _T("owner : ");
	if( ! &node.getOwner())
		cout << "null";
	else
	{
		cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!n";
		printListFrom(node.getOwner());
		cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
	}
}

void Runnable::printModule(const NEModule& module) const
{
	const TCHAR* is_enable = module.isEnable() ? _T("enable") : _T("disable");

	cout	<< _T("Enable\t:\t") << is_enable << "\n"
			<< _T("script\t:\t") << module.getScriptCode() << endl
			<< _T("name\t:\t") << module.getNameCode() << endl;
	printModuleHeader(module.getHeader());
}

void Runnable::printKey(const NEKey& key) const
{
	if(key.isSubClassOf(NEType::NENODE_CODESET_KEY))
	{
		const NENodeCodeSetKey& nodesetkey = static_cast<const NENodeCodeSetKey&>(key);
		printNodeSetKey(nodesetkey);
	}
	else if(key.isSubClassOf(NEType::NEMODULE_CODESET_KEY))
	{
		const NEModuleCodeSetKey& modulesetkey = static_cast<const NEModuleCodeSetKey&>(key);
		printModuleSetKey(modulesetkey);
	}
	else if(key.isSubClassOf(NEType::NEINT_KEY))
	{
		const NEIntKey& intkey = static_cast<const NEIntKey&>(key);
		cout << intkey.getValue();
	}
	else if(key.isSubClassOf(NEType::NEUINT_KEY))
	{
		const NEUIntKey& intkey = static_cast<const NEUIntKey&>(key);
		cout << intkey.getValue();
	}
	else if(key.isSubClassOf(NEType::NESHORT_KEY))
	{
		const NEShortKey& intkey = static_cast<const NEShortKey&>(key);
		cout << intkey.getValue();
	}
	else if(key.isSubClassOf(NEType::NEUSHORT_KEY))
	{
		const NEUShortKey& intkey = static_cast<const NEUShortKey&>(key);
		cout << intkey.getValue();
	}
	else if(key.isSubClassOf(NEType::NEBYTE_KEY))
	{
		const NEByteKey& bytekey = static_cast<const NEByteKey&>(key);
		cout << bytekey.getValue();
	}
	else if(key.isSubClassOf(NEType::NEUBYTE_KEY))
	{
		const NEUByteKey& bytekey = static_cast<const NEUByteKey&>(key);
		cout << bytekey.getValue();
	}
	else if(key.isSubClassOf(NEType::NECHAR_KEY))
	{
		const NECharKey& intkey = static_cast<const NECharKey&>(key);
		cout << intkey.getValue();
	}
	else if(key.isSubClassOf(NEType::NEWCHAR_KEY))
	{
		const NEWCharKey& intkey = static_cast<const NEWCharKey&>(key);
		cout << intkey.getValue();
	}
	else if(key.isSubClassOf(NEType::NEINT64_KEY))
	{
		const NEInt64Key& intkey = static_cast<const NEInt64Key&>(key);
		cout << intkey.getValue();
	}
	else if(key.isSubClassOf(NEType::NEFLOAT_KEY))
	{
		const NEFloatKey& intkey = static_cast<const NEFloatKey&>(key);
		cout << intkey.getValue();
	}
	else if(key.isSubClassOf(NEType::NEDOUBLE_KEY))
	{
		const NEDoubleKey& intkey = static_cast<const NEDoubleKey&>(key);
		cout << intkey.getValue();
	}
	else if(key.isSubClassOf(NEType::NEBOOLEAN_KEY))
	{
		const NEBooleanKey& intkey = static_cast<const NEBooleanKey&>(key);
		cout << intkey.getValue();
	}
	else if(key.isSubClassOf(NEType::NESTRING_KEY))
	{
		const NEStringKey& intkey = static_cast<const NEStringKey&>(key);
		cout << intkey.getValue();
	}
	else if(key.isSubClassOf(NEType::NEWSTRING_KEY))
	{
		const NEWStringKey& intkey = static_cast<const NEWStringKey&>(key);
		cout << intkey.getValue();
	}
	else if(key.isSubClassOf(NEType::NEBOOLEAN_SET_KEY))
	{
		const NEBooleanSetKey& intkey = static_cast<const NEBooleanSetKey&>(key);		
		const NEBooleanSet& set = intkey.getValue();

		for(int n=0; n < set.getLength() ;n++)
			cout << set[n] << _T(" ");
	}
	else if(key.isSubClassOf(NEType::NEBYTE_SET_KEY))
	{
		const NEByteSetKey& intkey = static_cast<const NEByteSetKey&>(key);		
		const NEByteSet& set = intkey.getValue();

		for(int n=0; n < set.getLength() ;n++)
			cout << set[n] << _T(" ");
	}
	else if(key.isSubClassOf(NEType::NEUBYTE_SET_KEY))
	{
		const NEUByteSetKey& intkey = static_cast<const NEUByteSetKey&>(key);		
		const NEUByteSet& set = intkey.getValue();

		for(int n=0; n < set.getLength() ;n++)
			cout << set[n] << _T(" ");
	}
	else if(key.isSubClassOf(NEType::NESHORT_SET_KEY))
	{
		const NEShortSetKey& intkey = static_cast<const NEShortSetKey&>(key);		
		const NEShortSet& set = intkey.getValue();		

		for(int n=0; n < set.getLength() ;n++)
			cout << set[n] << _T(" ");
	}
	else if(key.isSubClassOf(NEType::NEUSHORT_SET_KEY))
	{
		const NEUShortSetKey& intkey = static_cast<const NEUShortSetKey&>(key);
		const NEUShortSet& set = intkey.getValue();		

		for(int n=0; n < set.getLength() ;n++)
			cout << set[n] << _T(" ");
	}
	else if(key.isSubClassOf(NEType::NEINT_SET_KEY))
	{
		const NEIntSetKey& intkey = static_cast<const NEIntSetKey&>(key);		
		const NEIntSet& set = intkey.getValue();		

		for(int n=0; n < set.getLength() ;n++)
			cout << set[n] << _T(" ");		
	}
	else if(key.isSubClassOf(NEType::NEUINT_SET_KEY))
	{
		const NEUIntSetKey& intkey = static_cast<const NEUIntSetKey&>(key);		
		const NEUIntSet& set = intkey.getValue();		

		for(int n=0; n < set.getLength() ;n++)
			cout << set[n] << _T(" ");		
	}
	else if(key.isSubClassOf(NEType::NEINT64_SET_KEY))
	{
		const NEInt64SetKey& intkey = static_cast<const NEInt64SetKey&>(key);		
		const NEInt64Set& set = intkey.getValue();		

		for(int n=0; n < set.getLength() ;n++)
			cout << set[n] << _T(" ");
	}
	else if(key.isSubClassOf(NEType::NEFLOAT_SET_KEY))
	{
		const NEFloatSetKey& intkey = static_cast<const NEFloatSetKey&>(key);		
		const NEFloatSet& set = intkey.getValue();		

		for(int n=0; n < set.getLength() ;n++)
			cout << set[n] << _T(" ");
	}
	else if(key.isSubClassOf(NEType::NEDOUBLE_SET_KEY))
	{
		const NEDoubleSetKey& intkey = static_cast<const NEDoubleSetKey&>(key);		
		const NEDoubleSet& set = intkey.getValue();		

		for(int n=0; n < set.getLength() ;n++)
			cout << set[n] << _T(" ");


	}
	else if(key.isSubClassOf(NEType::NESTRING_SET_KEY))
	{
		const NEStringSetKey& intkey = static_cast<const NEStringSetKey&>(key);
		const NEStringSet& set = intkey.getValue();		

		for(int n=0; n < set.getLength() ;n++)
			cout << set[n] << _T(" ");


	}
	else if(key.isSubClassOf(NEType::NEWSTRING_SET_KEY))
	{
		const NEWStringSetKey& intkey = static_cast<const NEWStringSetKey&>(key);		
		const NEWStringSet& set = intkey.getValue();		

		for(int n=0; n < set.getLength() ;n++)
			cout << set[n] << _T(" ");


	}
	else if(key.isSubClassOf(NEType::NEKEY_SELECTOR))
	{
		const NEKeySelector& intkey = reinterpret_cast<const NEKeySelector&>(key);		
		printKeySelector(intkey);
	}
	else if(key.isSubClassOf(NEType::NEMODULE_SELECTOR))
	{
		const NEModuleSelector & intkey = reinterpret_cast<const NEModuleSelector&>(key);
		printModuleSelector(intkey);
	}
	else if(key.isSubClassOf(NEType::NENODE_SELECTOR))
	{
		const NENodeSelector& intkey = static_cast<const NENodeSelector&>(key);		
		printNodeSelector(intkey);
	}
	else
	{
		printError(NETString("알수없는 타입입니다 :") + key.getType());
	}	
}
void Runnable::printCodeType( NECodeType::CodeType type ) const
{
	switch(type)
	{
	case NECodeType::ALL:
		cout << _T("ALL");
		break;
	case NECodeType::SCRIPT:
		cout << _T("SCRIPT");
		break;
	case NECodeType::NAME:
		cout << _T("NAME");
		break;
	case NECodeType::GROUP:
		cout << _T("GROUP");
		break;
	case NECodeType::PRIORITY:
		cout << _T("PRIORITY");
		break;
	case NECodeType::RECENT:
		cout << _T("RECENT");
		break;
	case NECodeType::ME:
		cout << _T("ME");
		break;
	case NECodeType::UNDEFINED:
	default:
		cout << _T("UNDEFINED");	
	}
}

void Runnable::printNodeSelector(const NENodeSelector& selector) const
{
	cout << _T("node type\t:\t");
	printCodeType(selector.getNodeType());
	cout << endl;

	cout	<< _T("countlimit\t:\t") << selector.getCountLimit() << endl
			<< _T("codeset\t\t:\t");
	const NEIntSet& codeset = selector.getCodeSet();
	for(int n=0; n < codeset.getLength() ;n++)
		cout << codeset[n] << _T(" ");
	cout	<< _T("\nuse AND operation\t:\t") << selector.isUsingAndOperation() << endl
			<< _T("use Auto Binding\t:\t") << selector.isUsingAutoBinding() << endl;
}

void Runnable::printModuleSelector(const NEModuleSelector& selector) const
{
	printNodeSelector(selector);

	cout << _T("module type\t:\t");
	printCodeType(selector.getModuleType());
	cout << endl;

	const NECodeSet& codeset = selector.getModuleCodeSet();
	for(int n=0; n < codeset.getLength() ;n++)
			cout << codeset[n] << _T(" ");
	cout << "use auto binding for module\t:\t" << selector.isUsingAutoBinding() << endl;
}

void Runnable::printKeySelector(const NEKeySelector& selector ) const
{
	printModuleSelector(selector);

	cout << _T("key index\t:\t") << selector.getKeyIndex() << endl;
}

void Runnable::printObject(const NETString& path) const
{
	Runnable& runnable = Runnable::get();
	NEObject& object = runnable.getObjectFrom(path);
	if( ! &object)
		return printError(_T("잘못된 경로입니다."));

	cout	<< _T("position\t:\t") << runnable.getCurrentWorkPosition() << endl 
			<< _T("typename\t:\t") << object.getTypeName() << endl;

	if(object.isSubClassOf(NEType::NENODE))
	{
		const NENode& node = static_cast<NENode&>(object);
		runnable.printNode(node);
		runnable.printListFrom(node.getModuleSet());
	}
	else if(object.isSubClassOf(NEType::NENODE_CODESET))
	{
		runnable.printListFrom(object);
	}
	else if(object.isSubClassOf(NEType::NEMODULE_CODESET_KEY)	||
			object.isSubClassOf(NEType::NENODE_CODESET_KEY)		)
	{
		runnable.printListFrom(object);
	}
	else if(object.isSubClassOf(NEType::NEKEY))
	{
		NEKey& key = static_cast<NEKey&>(object);
		cout << _T("value\t\t:\n\t");
		runnable.printKey(key);
	}
	else if(object.isSubClassOf(NEType::NEMODULE))
	{
		const NEModule& module = static_cast<NEModule&>(object);
		runnable.printModule(module);
		runnable.printListFrom(module.getKeySet());
	}
	else
	{
		runnable.printError(NETString("현재 위치의 타입이 이상합니다 : ") + object.getTypeName());
	}
}

void Runnable::modifyNode(NENode& node) const
{
	if(! &node)
	{
		printError(_T("node 가 널객체입니다."));
	}
	
	bool node_running = true;

	while(node_running)
	{
		printNode(node);
		cout	<< _T("==========================================\n")
				<< _T("1. modify namecode\n")
				<< _T("2. modify groupcode\n")
				<< _T("3. modify priority\n")
				<< _T("4. modify enable\n")
				<< _T("5. quit\n")
				<< _T("==========================================\n")
				<< _T(" > ");

		int command = -1;
		TIN >> command;
		if(command < 1 || command > 5)
		{
			cout << _T("숫자를 다시 입력하세요.");
			continue;
		}
		
		flushStdIo();

		Editor& editor = Editor::getInstance();
		if( ! &editor)
		{
			return printError(_T("에디터가 없습니다."));
		}
		NEScriptEditor& scripted = editor.getScriptEditor();
		if( ! &scripted)
		{
			return printError(_T("스크립트 에디터가 없습니다."));
		}
		NEEventHandler& handler = editor.getEventHandler();
		if( ! &handler)
		{
			return printError(_T("핸들러가 없습니다"));
		}


		switch(command)
		{
		case 1:
			{
				type_index max_namecode = scripted.getScriptHeader().getMaxNameCodeIndex();
				bool running = true;
				while(running)
				{
					cout << _T("변경할 이름 코드 입력(0 ~ ") << max_namecode << _T(") : ");
					type_index new_namecode = -1;
					TIN >> new_namecode;
					flushStdIo();
					if(new_namecode >= 0 && new_namecode <= max_namecode)
					{
						node.setNameCode(new_namecode);
						break;
					}
					else
						cout << _T("잘못된 코드값. 다시 입력하세요\n");
				}
			}
			break;
		case 2:
			{
				type_index max_namecode = scripted.getScriptHeader().getMaxGroupCodeIndex();

				bool running = true;
				while(running)
				{
					const NEIntSet& node_codeset = node.getGroupCodes();

					cout	<< _T("==============================================\n");
						for(int n=0; n <  node_codeset.getLength() ;n++)
							cout << node_codeset[n] << _T(" ");
					cout	<< _T("==============================================\n")
							<< _T("스페이스로 구분해서 그룹코드(0 ~ ") << max_namecode << _T("set 입력 > ");

					TCHAR buffer[256];
					TIN.getline(buffer, 256);
					NETString groupcode_string = buffer;

					if(groupcode_string == _T("") || ! groupcode_string.getLength())
					{
						printError(_T("잘못된 그룹코드. 재입력"));
						continue;
					}

					NETStringSet splited;
					groupcode_string.split(_T(" "), splited);
					NEIntSet codeset(splited.getLength());
					bool is_valid_codeset = true;
					for(int n=0; n < splited.getLength() ;n++)
					{
						type_index groupcode = splited[n].toInt();
						if(groupcode < 0 || groupcode > max_namecode)
						{
							printError( NETString(_T("주어진 그룹코드값 : ") + groupcode + NETString(_T("는 범위에 어긋납니다.")) ) );
							is_valid_codeset = false;
							break;
						}
						else
							codeset.push(groupcode);
					}

					if(is_valid_codeset)
					{
						node.setGroupCodes(codeset);
						break;
					}
				}
			}
			break;
		case 3:
			{
				type_index max_namecode = scripted.getScriptHeader().getMaxPriorityCodeIndex();
				bool running = true;
				while(running)
				{
					cout << _T("변경할 우선권 코드 입력(0 ~ ") << max_namecode << _T(") : ");
					type_index new_namecode = -1;
					TIN >> new_namecode;
					flushStdIo();
					if(new_namecode >= 0 && new_namecode <= max_namecode)
					{
						node.setPriorityCode(new_namecode);
						break;
					}
					else
						cout << _T("잘못된 코드값. 다시 입력하세요\n");
				}
			}
			break;	
		case 4:
			{
				bool running = true;
				while(running)
				{
					cout << _T("Enable로 합니까?(Y/N) : ");
					TCHAR answer;
					TIN >> answer;
					cout << "\n";
					flushStdIo();
					switch(answer)
					{
					case _T('Y'): case _T('y'):
						node.setEnable(true);
						running = false;
						break;
					case _T('N'): case _T('n'):
						node.setEnable(false);
						running = false;
						break;
					default:
						cout << _T("다시 입력하세요\n");
					}
				}
			}
		case 5:
			node_running = false;
			break;
		}
	}

}

void Runnable::modifyModule(NEModule& module) const
{
	//	pre:
	if(! &module)
	{
		printError(_T("node 가 널객체입니다."));
	}
	//		타겟팅:
	Editor& editor = Editor::getInstance();
	if( ! &editor) return printError(_T("에디터가 없습니다"));
	NEPanelManager& paneler = editor.getPanelManager();
	if( ! &paneler) return printError(_T("패널 매니져가 없습니다"));
	//		모듈과 맞는 패널 찾기:
	NEEventHandler& handler = editor.getEventHandler();
	if( ! &handler)
	{
		return printError(_T("핸들러가 없습니다"));
	}
	

	bool node_running = true;
	while(node_running)
	{
		cout	<< _T("==========================================\n")
			<< _T("1. modify namecode\n")
			<< _T("2. call panel(or call default module editor)\n")
			<< _T("3. modify enable\n")
			<< _T("4. quit\n")
			<< _T("==========================================\n")
			<< _T(" > ");

		int command = -1;
		TIN >> command;
		if(command < 1 || command > 4)
		{
			cout << _T("숫자를 다시 입력하세요.");
			continue;
		}

		flushStdIo();

		switch(command)
		{
		case 1:
			{
				int max_namecode = 200000000;
				bool running = true;
				while(running)
				{
					cout << _T("변경할 이름 코드 입력(0 ~ ") << max_namecode << _T(") : ");
					type_index new_namecode = -1;
					TIN >> new_namecode;
					flushStdIo();
					if(new_namecode >= 0 && new_namecode <= max_namecode)
					{
						module.setNameCode(new_namecode);
						break;
					}
					else
						cout << _T("잘못된 코드값. 다시 입력하세요\n");
				}
			}
			break;
		case 2:
			{
				NEPanel& panel = paneler.getPanel(module.getHeader());
				if( ! &panel) return printError(_T("지정한 모듈에 맞는 패널이 없습니다."));
				if( &(panel.getModule()) ) // 이미 다른 모듈이 open 중이라면
				{
					return printError(_T("다른 모듈에서 패널을 사용중입니다."));		
				}
				//		패널과 모듈 연결:
				handler.onCallingPanel(panel, module);
			}
			
			break;
		case 3:
			{
				bool running = true;
				while(running)
				{
					cout << _T("Enable로 합니까?(Y/N) : ");
					TCHAR answer;
					TIN >> answer;
					cout << "\n";
					flushStdIo();
					switch(answer)
					{
					case _T('Y'): case _T('y'):
						module.setEnable(true);
						running = false;
						break;
					case _T('N'): case _T('n'):
						module.setEnable(false);
						running = false;
						break;
					default:
						cout << _T("다시 입력하세요\n");
					}
				}
			}
		case 4:
			node_running = false;
			break;
		}
	}
}

void Runnable::modifyCode(NECodeType::CodeType type, type_code max_code, type_result(NEScriptEditor::*insert_method)(type_code), type_result(NEScriptEditor::*remove_method)(type_code), NETStringList& bank)
{
	bool is_script_code = (!insert_method && !remove_method);
	
	//	pre:
	Editor& editor = Editor::getInstance();
	if( ! &editor)
		return printError(_T("에디터가 없습니다."));
	NEEventHandler& handler = editor.getEventHandler();
	if( ! &handler)
		return printError(_T("이벤트 핸들러"));
	NEScriptEditor& scripted = editor.getScriptEditor();
	if( ! &scripted)
		return printError(_T("스트립트 에디터가 없습니다"));
	NEScriptEditor::Banks& banks = scripted.getBanks();
	
	
	
	bool running = true;
	while(running)
	{
		cout	<< _T("=========================================\n");

		switch(type)
		{
		case NECodeType::SCRIPT: cout << _T("scriptcode"); break;
		case NECodeType::NAME: cout << _T("namecode"); break;
		case NECodeType::GROUP: cout << _T("groupcode"); break;
		case NECodeType::PRIORITY: cout << _T("prioritycode"); break;
		default:
			cout << _T("undefined");
		}

		if(!is_script_code)
		{
			cout	<< _T(" : 0 ~ ") << max_code << endl << endl
					<< _T("1. insert namecode\n")
					<< _T("2. remove namecode\n")
					<< _T("3. modify bank\n")
					<< _T("4. back\n")
					<< _T("=========================================\n")
					<< _T(" > ");
		}
		else
		{
			cout	<< _T(" : 0 ~ ") << max_code << endl << endl
					<< _T("3. modify bank\n")
					<< _T("4. back\n")
					<< _T("=========================================\n")
					<< _T(" > ");
		}

		int command2 = -1;
		TIN >> command2;
		if(!is_script_code)
		{
			if(command2 < 1 || command2 > 4)
			{
				printError(_T("잘못된 명령번호."));
				continue;
			}
		}
		else
		{
			if(command2 < 3 || command2 > 4)
			{
				printError(_T("잘못된 명령번호."));
				continue;
			}
		}	
		
		flushStdIo();

		switch(command2)
		{
		case 1:
			{
				cout << _T("\n삽입할 인덱스 입력 0 ~ ") << max_code + 1 << _T(" > ");
				type_code new_namecode = max_code;
				while(1)
				{
					TIN >> new_namecode;
					flushStdIo();

					if(new_namecode < 0 || new_namecode > max_code + 1) // push는 가능하다
						printError(_T("범위가 잘못되었습니다."));
					else
						break;
				}

				(scripted.*insert_method)(new_namecode);
				
				running = false;
			}
			break;
		case 2:
			{
				cout << _T("\n삭제할 인덱스 입력 0 ~ ") << max_code << _T(" > ");
				type_code new_namecode = max_code;
				while(1)
				{
					TIN >> new_namecode;
					flushStdIo();

					if(new_namecode < 0 || new_namecode > max_code)
						printError(_T("범위가 잘못되었습니다."));
					else
						break;
				}

				(scripted.*remove_method)(new_namecode);
				running = false;
			}
			break;
		case 3:
			{
				cout << _T("\n변경할 네임의 인덱스 입력 0 ~ ") << max_code << _T(" > ");
				type_code new_namecode = max_code;
				while(1)
				{
					TIN >> new_namecode;
					flushStdIo();

					if(new_namecode < 0 || new_namecode > max_code)
						printError(_T("범위가 잘못되었습니다."));
					else
						break;
				}

				

				//	인덱스를 입력했다면:
				cout	<< _T("선택한 인덱스") << new_namecode << _T("의 이름은 현재, \n\t");

				NETStringList* bank = NE_NULL;
				switch(type)
				{
				case NECodeType::SCRIPT: bank = &(banks.getBank(NECodeType::SCRIPT)); break;
				case NECodeType::NAME: bank = &(banks.getBank(NECodeType::NAME)); break;
				case NECodeType::GROUP: bank = &(banks.getBank(NECodeType::GROUP)); break;
				case NECodeType::PRIORITY: bank = &(banks.getBank(NECodeType::PRIORITY)); break;
				default:
					return printError(_T("이상한 타입. undefined."));
				}

				NETString& target_name = bank->getElement(new_namecode);
				
				cout	<< target_name << endl
						<< _T("새로 바꿀 이름 입력 > ");

				TCHAR buffer[256] = {0, };
				TIN.getline(buffer, 256);

				target_name = buffer;

				running = false;
			}
			break;
		case 4:
			running = false;
			break;
		default:
			printError(_T("뭔가 잘못되었습니다."));
		}
	}
}

void Runnable::printNEScriptHeader(const NEScriptEditor& editor) const
{
	_printHeader(editor.getScriptHeader());
	cout	<< _T("\t최대 NameCode : ") << editor.getScriptHeader().getMaxNameCodeIndex() << endl
			<< _T("\t최대 ScriptCode : ") << editor.getScriptHeader().getMaxScriptCodeIndex() << endl
			<< _T("\t최대 GroupCode : ") << editor.getScriptHeader().getMaxGroupCodeIndex() << endl
			<< _T("\t최대 PriorityCode : ") << editor.getScriptHeader().getMaxPriorityCodeIndex() << endl;
}

void Runnable::printNEScriptHeader(const NEScriptHeader& header) const
{
	_printHeader(header);
	cout	<< _T("\t최대 NameCode : ") << header.getMaxNameCodeIndex() << endl
			<< _T("\t최대 ScriptCode : ") << header.getMaxScriptCodeIndex() << endl
			<< _T("\t최대 GroupCode : ") << header.getMaxGroupCodeIndex() << endl
			<< _T("\t최대 PriorityCode : ") << header.getMaxPriorityCodeIndex() << endl;
}

void Runnable::_printHeader( const NEExportable::Header& header ) const
{
	cout << header.getName() << endl;
	cout << _T("\t개발자\t\t:\t") << header.getDeveloper() <<  endl;
	
	cout << _T("\t개정횟수\t:\t") << header.getRevision() << endl;
	
	cout << _T("\t버전\t\t:\t") << header.getVersion() << endl;
	cout << _T("\t배포일자\t:\t") << header.getReleaseDate() << endl;
	cout << _T("\t설명\t\t:\t") << header.getComment() << endl;	
}

type_index Runnable::_fetchIndex( const NETString& message, type_index max_index, type_index min_index) const
{
	int command = -1;
		
	while(1)
	{	
		cout << message;
		TIN >> command;
		flushStdIo();
		if(TIN.fail())
		{
			cout << _T("잘못된 입력입니다. 재입력하세요\n");
			TIN.clear();
			flushStdIo();
			continue;
		}
		
		if(command < min_index || command > max_index)
			cout << _T("\n범위를 벗어난 입력\n");
		else
			break;
	}
	
	cout << endl;
	
	return command;
}

void Runnable::flushStdIo() const
{
	TIN.ignore(256, L'\n');
}

void Runnable::printNodeSetKey( const NENodeCodeSetKey& key ) const
{
	Editor& editor = Editor::getInstance();
	if( ! &editor)
	{
		printError(_T("에디터가 없습니다"));
		return;
	}
	NEScriptEditor& scripted = editor.getScriptEditor();
	if( ! &scripted)
	{
		printError(_T("스크립트 에디터가 없습니다"));
		return;
	}
	NERootNodeCodeSet& list = scripted.getScriptNodes();

	const NENodeCodeSet& ns = key.getValue();
	const NENode& first_node = ns[0];
	if(&first_node)
	{
		cout << _T("노드 ");

		if(first_node.getScriptCode() > NE_HAS_NO_ID)
			cout << scripted.getBanks().getBank(NECodeType::SCRIPT)[ first_node.getScriptCode() ];
		else
			cout << _T("아이디없음(-1)");

		cout << _T(" 등 ") << ns.getLength() << _T("개 존재");
	}
	else
		cout << _T("노드 없음");
}

void Runnable::printModuleSetKey( const NEModuleCodeSetKey& key ) const
{
	const NEModuleCodeSetKey& msk = static_cast<const NEModuleCodeSetKey&>(key);
	const NEModuleCodeSet& ms = msk.getValue();
	
	cout << _T("모듈 ") << ms.getLength() << _T("개 존재");
}

void Runnable::printNodeCollector( const NENodeCollector& collector ) const
{
	Editor& editor = Editor::getInstance();
	if( ! &editor)
	{
		printError(_T("에디터가 없습니다"));
		return;
	}
	NEScriptEditor& scripted = editor.getScriptEditor();
	if( ! &scripted)
	{
		printError(_T("스크립트 에디터가 없습니다"));
		return;
	}
	NETStringList& scriptbank = scripted.getBanks().getBank(NECodeType::SCRIPT);
	

	cout << _T("n\tid\tnamecod\tprior.\tmodcnt.\tgrpcod.\tname\n==================================================\n");
	for(int n=0; n < collector.getLength() ;n++)
	{
		const NENode& node = collector[n];

		cout << n << _T("\t") << node.getScriptCode() << _T("\t") << node.getNameCode() << _T("\t") << node.getPriorityCode() << _T("\t") << node.getModuleSet().getLength() << _T("\t");
		const NEIntSet& codeset = node.getGroupCodes();
		for(int code_n=0; code_n < codeset.getLength() ; code_n++)
			cout << codeset[code_n] << " ";

		type_code scriptcode = node.getScriptCode();

		if(scriptcode >= 0 && scriptcode <= scriptbank.getLengthLastIndex())
			cout << "\t" << scriptbank[scriptcode];

		cout << endl;
	}

}

void Runnable::print(NEEnlistableManager& manager)
{
	cout << "ShortcutSets:\n";
	cout << "\tName:" << manager.getShortCutSet(NECodeType::NAME).getLength() << " / " << manager.getShortCutSet(NECodeType::NAME).getSize() << "\n";
	::print(manager.getShortCutSet(NECodeType::NAME));
	cout << "\tScript:" << manager.getShortCutSet(NECodeType::SCRIPT).getLength() << " / " << manager.getShortCutSet(NECodeType::SCRIPT).getSize() << "\n";
	::print(manager.getShortCutSet(NECodeType::SCRIPT));
	cout << "\tGroup:" << manager.getShortCutSet(NECodeType::GROUP).getLength() << " / " << manager.getShortCutSet(NECodeType::GROUP).getSize() << "\n";
	::print(manager.getShortCutSet(NECodeType::GROUP));
	cout << "\tPriority:" << manager.getShortCutSet(NECodeType::PRIORITY).getLength() << " / " << manager.getShortCutSet(NECodeType::PRIORITY).getSize() << "\n";
	::print(manager.getShortCutSet(NECodeType::PRIORITY));
}

namespace 
{
	void print(const NEShortCutSet& set)
	{
		for(int n=0; n < set.getLength() ;n++)
		{
			cout << n << ":";
			const NEShortCut& cut = set[n];
			for(int n2=0; n2 < cut.getLength() ;n2++)
				cout << cut[n2] << " ";
			cout << endl;
		}		
	}
}