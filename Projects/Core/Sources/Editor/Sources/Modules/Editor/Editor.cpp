#include "Editor.hpp"
#include "define/define.hpp"

namespace NE
{
	//	NENodeSelector의 getManagerInterface 교체:
	//		사전 지식:	getManager란?			
	//			NENodeSelector에는 getManager라는 메소드가 있다.
	//			이 메소드는 NodeSelector에서 검색을 실시할 Manager를 정하는 것으로,
	//			NodeManager 뿐만아니라 ScriptManager, ScriptEditor에서도 Selector
	//			가 검색할 수 있도록 하는 기능이다.
	//			
	//		문제 정의:	ScriptEditor가 문제다.
	//			ScriptEditor는 Editor 패키지에 존재하기 때문에 노드를 실행만하는 
	//			경우는 Kernal에 없는 ScriptEditor는 탐색이 되선 안된다.
	//			반대로 Editing시에는 ScriptEditor도 탐색을 해야 하는 상황이 생긴다.
	//			(대표적인 예가 GUI기반 Script 에디터를 만들때)
	//			따라서, 현재 프로세스에 Editor.dll까지 로드가 되어있을때만 
	//			ScriptEditor를 getManager에서 참조할 수 있도록 해야 한다는
	//			이야기가 되어버린다.
	//			문제를 해결하기 위한 조건을 서술하면 다음과 같을 것이다.
	//			
	//				1. Editor와 Kernal 패키지는 각각 분리되어 있어야 한다.
	//				Kernal안에 ScriptEditor의 기능을 넣는 형태는 안된다.
	//				그러면 Editor에 커널 코드가 들어가게 되면서 사실상 두 
	//				패키지를 나눈 이유가 없어지기 때문이다.
	//			
	//				2. Editor가 시스템에 로드 되었을때만 ScriptEditor가 getManager
	//				에서 접근 할 수 있어야 한다.
	//			
	//		어떻게 해결했는가:	임시땜방
	//			NodeSelector에 getManager가 호출하는 인터페이스를 전역으로 만들고,
	//			Editor가 시스템에 붙을때(정확히는 execute로 실행될때) 이 인터페이스
	//			를 Editor용 getManager로 교체하는 방법을 사용했다.(즉, 함수포인터 사용)
	//			1번과 2번의 내용을 모두 해결하면서 코드 수정은 극단적으로 수정한
	//			훌륭한 해결방법 중의 하나라고 생각하지만, 역시 땜방의 느낌이 드는
	//			건 어쩔 수가 없다.	이유는 다음과 같은데,
	//
	//				1. 다형성이나 여러가지 좋은 C++ 기능을 나두고 함수포인터를 사용
	//				한다는 C 적인 관점에서 해결한 점.
	//
	//				2. Editor에서 NodeSelector(Kernal)의 private에 접근해야 하므로
	//				friend 선언이 하나 늘어났다는 점.
	NEEnlistableManager& _newManagerInterface(NEType::Type type)
	{
		//	main:
		switch(type)
		{
		case NEType::NENODE_MANAGER:	return Kernal::getInstance().getNodeManager();
		case NEType::NESCRIPT_MANAGER:	return Kernal::getInstance().getScriptManager();
		case NEType::NESCRIPT_EDITOR:	
			if(&Editor::getInstance())
				return Editor::getInstance().getScriptEditor();

		default:
			NEEnlistableManager* nullpointer = 0;
			return *nullpointer;
		};
	}

	NE_DLL Editor::Editor()
		: NEModule(), _panel_manager(NE_NULL), _script_editor(NE_NULL), _event_handler(NE_NULL)
	{
		_release();
	}
	
	
	
	NE_DLL Editor::Editor(const Editor& source)
		: NEModule(source), _panel_manager(NE_NULL), _script_editor(NE_NULL), _event_handler(NE_NULL)
	{
		_assign(source);
	}
	
	
	
	NE_DLL Editor::~Editor()
	{
		if(_is_setting_initialized)
		{
			//	커널 스위칭을 해서 saveSetting을 실시:
			Editor& original = Editor::getInstance();
			Editor::setInstance(*this);
			Editor::saveSettings();				
			Editor::setInstance(original);
		}

		_release();
	}
	
	
	
	const Editor NE_DLL &Editor::operator=(const Editor& source)
	{
		NEModule::operator=(source);

		return _assign(source);
	}
	
	
	
	bool NE_DLL Editor::operator==(const Editor& source) const
	{
		if(this == &source) return true;
		if(NEModule::operator==(source) == false) return false;
		//	인스턴스 체크:
		//		NEPanelManager:
		if(_panel_manager)
		{
			if(source._panel_manager)
			{
				if(*_panel_manager != *(source._panel_manager)) return false;
			}
			else
				return false;
		}
		else if(source._panel_manager)
			return false;
		//		NEScriptEditor:
		if(_script_editor)
		{
			if(source._script_editor)
			{
				if(*_script_editor != *(source._script_editor)) return false;
			}
			else
				return false;
		}
		else if(source._script_editor)
			return false;
		//		NEEventHandler:
		if(_event_handler)
		{
			if(source._event_handler)
			{
				if(*_event_handler != *(source._event_handler)) return false;
			}
			else
				return false;
		}
		else if(source._event_handler)
			return false;
		//	Identifier:
		if(_panel_manager_identifier != source._panel_manager_identifier) return false;
		if(_script_editor_identifier != source._script_editor_identifier) return false;
		if(_event_handler_identifier != source._event_handler_identifier) return false;
		if(_settings != source._settings) return false;
		if(_default_settings != source._default_settings) return false;
		if(_is_setting_initialized != source._is_setting_initialized) return false;
		
		return true;
	}
	
	
	
	bool NE_DLL Editor::operator!=(const Editor& source) const
	{
		return !(operator==(source));
	}
	
	
	
	NEEventHandler NE_DLL &Editor::getEventHandler()
	{
		return *_event_handler;
	}
	const NEEventHandler NE_DLL &Editor::getEventHandler() const
	{
		return *_event_handler;
	}
	NEScriptEditor NE_DLL &Editor::getScriptEditor()
	{
		return *_script_editor;
	}
	const NEScriptEditor NE_DLL &Editor::getScriptEditor() const
	{
		return *_script_editor;
	}
	NEPanelManager NE_DLL &Editor::getPanelManager()
	{
		return *_panel_manager;
	}
	const NEPanelManager NE_DLL &Editor::getPanelManager() const
	{
		return *_panel_manager;
	}
	const Editor::ModifiableSettings NE_DLL &Editor::getSettings() const
	{
		return _settings;
	}
	const Editor::ModifiableSettings NE_DLL &Editor::getDefaultSettings() const
	{
		return _default_settings;
	}	
	
	const NEExportable::ModuleHeader NE_DLL &Editor::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			//	아직 초기화가 안되었음
			_header.getName() = _T(_NAME);
			_header.getDeveloper() = _T(_DEVELOPER);
			_header.setRevision(_REVISION);
			_header.getReleaseDate() = _T(_DATE);
			_header.getVersion()  = _T(_VERSION);
			_header.getComment() = _T(_COMMENT);
			_header.setMaxErrorCodeCount(Editor::END_OF_ERROR_CODE - 1);
		}

		return _header;
	}
	
	
	
	LPCTSTR NE_DLL Editor::getErrorMessage(type_errorcode errorcode)
	{
		switch(errorcode)
		{
		case NEMODULE_IS_NOT_VALID:
			return _T("잘못된 모듈");
		case PANEL_MANAGER_IS_NOT_VALID:
			return _T("잘못된 패널 매니져");
		case SCRIPT_EDITOR_IS_NOT_VALID:
			return _T("잘못된 스크립트 에디터");
		case EVENT_HANDLER_IS_NOT_VALID:
			return _T("잘못된 이벤트 핸들러");
		default:			
			return _T("알 수 없는 오류");
		}
	}
	
	
	
	type_result NE_DLL Editor::execute()
	{
		return RESULT_SUCCESS;
	}
	
	
	
	NEType::Type NE_DLL Editor::getType() const
	{
		return NEType::EDITOR;
	}
	
	
	
	NEBinaryFileSaver NE_DLL &Editor::serialize(NEBinaryFileSaver& saver) const
	{
		NEModule::serialize(saver);

		saver	<< _settings << _default_settings << _panel_manager_identifier << _script_editor_identifier
				<< _event_handler_identifier << _is_setting_initialized;
		
		if(_panel_manager)
			saver << *_panel_manager;
		if(_script_editor)
			saver << *_script_editor;
		if(_event_handler)
			saver << *_event_handler;
			
		return saver;
	}
	
	
	
	NEBinaryFileLoader NE_DLL &Editor::serialize(NEBinaryFileLoader& loader)
	{
		NEModule::serialize(loader);

		loader	>> _settings >> _default_settings >> _panel_manager_identifier >> _script_editor_identifier
				>> _event_handler_identifier >> _is_setting_initialized;

		if(_panel_manager)
			loader >> *_panel_manager;
		if(_script_editor)
			loader >> *_script_editor;
		if(_event_handler)
			loader >> *_event_handler;

		return loader;
	}
	
	
	
	type_result NE_DLL Editor::isValid() const
	{
		type_result result = NEModule::isValid();
		if(NEResult::hasError(result)) return result;
		result = _panel_manager_identifier.isValid();
		if(NEResult::hasError(result)) return result;
		result = _script_editor_identifier.isValid();
		if(NEResult::hasError(result)) return result;
		result = _event_handler_identifier.isValid();
		if(NEResult::hasError(result)) return result;
		result = _settings.isValid();
		if(NEResult::hasError(result)) return result;
		result = _default_settings.isValid();
		if(NEResult::hasError(result)) return result;
		if(_panel_manager)
		{
			result = _panel_manager->isValid();
			if(NEResult::hasError(result)) return result;
		}
		if(_script_editor)
		{
			result = _script_editor->isValid();
			if(NEResult::hasError(result)) return result;
		}
		if(_event_handler)
		{
			result = _event_handler->isValid();
			if(NEResult::hasError(result)) return result;
		}

		return RESULT_SUCCESS;
	}
	
	
	
	NEObject NE_DLL &Editor::clone() const
	{
		return *(new Editor(*this));
	}
	void NE_DLL Editor::release()
	{
		NEModule::release();

		return _release();
	}

	type_result NE_DLL Editor::loadSettings()
	{
		//	pre:
		Kernal& kernal = Kernal::getInstance();
		if( ! &kernal) return RESULT_TYPE_ERROR;
		NEINIManager& ini = kernal.getINIManager();



		//	main:
		//		변수 정의:
		NETString buffer;
		//		INI로부터 값 읽기:
		type_result result = ini.readModuleIniString(getHeader(), _PANEL_DIRECTORY_KEY, buffer);
		_settings.setPanelDirectory(buffer);

		return result;
	}

	const Editor& Editor::_assign(const Editor& source)
	{
		if(this == &source) return *this;

		//	Identifier:
		_panel_manager_identifier = source._panel_manager_identifier;
		_script_editor_identifier = source._script_editor_identifier;
		_event_handler_identifier = source._event_handler_identifier;
		//	Settings:
		_settings = source._settings;
		_default_settings = source._default_settings;
		_is_setting_initialized = source._is_setting_initialized;
		//	Manager Pointing:
		//			인스턴스를 무작정 모듈매니져로부터 가져오기보다는, source가 모듈들을
		//			fetch 여부를 판단해야 한다. 왜냐하면 
		//				1. source의 상황과 일치시켜야 하므로.
		//				2. 쓸데없는 부하를 줄여야 하므로.
		//			fetch 여부 판단:
		if(source._panel_manager || source._script_editor || source._event_handler) // 하나라도 포인터가 할당되어있다면, 로드를 했단 얘기다.
		{
			//		할당:
			//			포인터 할당:	fetch한 모듈중에서 매니져클래스를 찾아서 포인터에 넣는다
			_pointManagerInstance();
			//			매니져클래스의 대입연산:
			if(source._panel_manager)
			{
				if(_panel_manager)
					*_panel_manager = *source._panel_manager;
				else
					EDITOR_ERROR(" : ")
			}
			if(source._script_editor)
			{
				if(_script_editor)
					*_script_editor = *source._script_editor;
				else
					EDITOR_ERROR(" : ")
			}
			if(source._event_handler)
			{
				if(_event_handler)
					*_event_handler = *source._event_handler;
				else
					EDITOR_ERROR(" : ")
			}
		}

		return *this;
	}

	Editor NE_DLL &Editor::getInstance()
	{
		return *_instance;
	}
	type_result NE_DLL Editor::initialize()
	{
		//	post:
		_initializeSettings();



		//	main:
		_pointManagerInstance();		
		//		매니져 클래스 초기화:
		//			settings:	
		//				모듈매니져에서 settings 값을 이용하므로, Setting의 초기화에는 NEINIManager
		//				가 필요하므로 모듈매니져를 초기화 하고 나서 pointManager를 한 뒤에 
		//				읽어야 한다.
		if(_panel_manager)
			_panel_manager->initialize();
		if(_script_editor)
			_script_editor->initialize();
		if(_event_handler)
			_event_handler->initialize();



		//	post:
		_attachNewManagerInterfaceToNodeSelector();
		return RESULT_SUCCESS;
	}
	
	void Editor::_release()
	{
		_releaseManagerPointer();
		_panel_manager_identifier.release();
		_script_editor_identifier.release();
		_event_handler_identifier.release();
		_settings.release();
		_default_settings.release();
		_is_setting_initialized = false;
	}
	
	void Editor::_releaseManagerPointer()
	{
		if(_panel_manager)
		{
			delete _panel_manager;
			_panel_manager = NE_NULL;
		}
		if(_script_editor)
		{
			delete _script_editor;
			_script_editor = NE_NULL;
		}
		if(_event_handler)
		{
			delete _event_handler;
			_event_handler = NE_NULL;
		}
	}
	type_result Editor::_pointManagerInstance()
	{
		//	pre:
		//		타겟팅:
		Kernal& kernal = Kernal::getInstance();
		if( ! &kernal)
		{
			EDITOR_ERROR(" : ")
			return RESULT_TYPE_ERROR;
		}
		NEModuleManager& moduler = kernal.getModuleManager();
		if( ! &moduler)
		{
			EDITOR_ERROR(" : ")
			return RESULT_TYPE_ERROR;
		}
		
		
		
		//		INI매니져:	디버그매니져에서 INI가 필요하므로 INI매니져를 먼저 추가한다.
		//		패널 매니져:
		if( ! NEResult::hasError(_panel_manager_identifier.isValid()) )
		{	
			const NEPanelManager* panel_manager_pointer = static_cast<const NEPanelManager*>( &(moduler.getModule(_panel_manager_identifier)));
			if( ! panel_manager_pointer)
				EDITOR_ERROR(" : ")
			else
				_panel_manager = const_cast<NEPanelManager*>(panel_manager_pointer);
			
		}
		else
		{
			EDITOR_ERROR(" : ")
			return RESULT_TYPE_ERROR;
		}
		//		스크립트 에디터:
		if( ! NEResult::hasError(_script_editor_identifier.isValid()) )
		{	
			const NEScriptEditor* script_editor_pointer = static_cast<const NEScriptEditor*>( &(moduler.getModule(_script_editor_identifier)));
			if( ! script_editor_pointer)
				EDITOR_ERROR(" : ")
			else
				_script_editor = const_cast<NEScriptEditor*>(script_editor_pointer);			
		}
		else
		{
			EDITOR_ERROR(" : ")
			return RESULT_TYPE_ERROR;
		}
		//		이벤트 핸들러:
		if( ! NEResult::hasError(_event_handler_identifier.isValid()) )
		{	
			const NEEventHandler* event_handler_pointer = static_cast<const NEEventHandler*>( &(moduler.getModule(_event_handler_identifier)));
			if( ! event_handler_pointer)
				EDITOR_ERROR(" : ")
			else
				_event_handler = const_cast<NEEventHandler*>(event_handler_pointer);			
		}
		else
		{
			EDITOR_ERROR(" : ")
			return RESULT_TYPE_ERROR;
		}
		
		
		//	pre:
		return RESULT_SUCCESS;
	}

	type_result NE_DLL Editor::setInstance(Editor& new_instance_to_set)
	{
		//	pre:		
		if( ! &new_instance_to_set)
		{
			EDITOR_ERROR(" : ")
			return RESULT_TYPE_ERROR;
		}
		



		//	main:
		//		인스턴스 복사:		
		_instance = &new_instance_to_set;



		//	post:
		return RESULT_SUCCESS;
	}

	void Editor::_initializeSettings()
	{
		//	INI로부터 읽어야 하는가:
		if(_settings.isLoadedFromINI())
		{	//	INI로부터 로드:
			if(NEResult::hasError(loadSettings()))
				//	INI로드 중 에러가 발생하면:	디폴트로부터 가져온다
				EDITOR_WARNING(" : 세팅을 로드하는 중 에러")
		}
		else
			EDITOR_INFORMATION(" : INI를 사용하지 않음")



		if(NEResult::hasError(_settings.isValid()))
		{
			EDITOR_WARNING(" : 가지고 있는 세팅이 잘못되서 기본(디폴트) 세팅으로 적용 시도")
			_initializeSettingsToDefaultSettingsOrReleaseBoth();
		}

		_is_setting_initialized = true;
	}

	void Editor::_initializeSettingsToDefaultSettingsOrReleaseBoth()
	{
		//			근데 디폴트값도 invalid 하다면:
		if(NEResult::hasError(_default_settings.isValid()))
		{	//			둘다 release 한다:
			EDITOR_ERROR(" : ")
			_default_settings.release();
			_settings.release();					
		}
		else
		{	//	디폴트는 valid하다면:
			EDITOR_WARNING(" : 로드에 실패해서 기본 세팅으로 읽어들임")
			_settings = _default_settings;
		}
	}

	type_result Editor::saveSettings()
	{
		//	pre:
		//		타겟팅:
		Kernal& kernal = Kernal::getInstance();
		if( ! &kernal)
		{
			EDITOR_ERROR(" : ")
			return RESULT_TYPE_ERROR;
		}
		Editor& editor = Editor::getInstance();
		if( ! &editor)
		{
			EDITOR_ERROR(" : ")
			return RESULT_TYPE_ERROR;
		}
		NEINIManager& ini = kernal.getINIManager();
		const Editor::ModifiableSettings& settings = editor.getSettings();
		const NEExportable::Identifier& identifier = editor.getHeader();

		type_result result = RESULT_SUCCESS;

		NEResult::addErrorFlag(result, ini.writeModuleIniString(identifier, _PANEL_DIRECTORY_KEY, settings.getPanelDirectory()));

		return result;
	}

	void Editor::_attachNewManagerInterfaceToNodeSelector()
	{
		NENodeSelector::_interface = _newManagerInterface;
	}

}

NE::Editor* NE::Editor::_instance = NE_NULL;