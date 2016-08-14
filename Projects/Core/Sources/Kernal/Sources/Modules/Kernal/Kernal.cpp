#include "Kernal.hpp"
#include "define/define.hpp" // 노드엔진의 헤더

namespace NE
{
	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	메모	:	왜 여기서만 멤버변수를 초기화 구문에 넣는가:
	//					release에서는 매니져클래스의 포인터를 NE_NULL로 초기화 할수 없기 때문이다.
	//	---------------------------------------------------------------------------------
	NE_DLL Kernal::Kernal()
		: Super(), _script_manager(NE_NULL), _node_manager(NE_NULL)
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NE_DLL Kernal::Kernal(const This& source)
		: Super(source), _script_manager(NE_NULL), _node_manager(NE_NULL)
	{
		_assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NE_DLL Kernal::~Kernal()
	{
		if(_is_setting_initialized)
		{
			//	커널 스위칭을 해서 saveSetting을 실시:
			This& original = This::getInstance();
			This::setInstance(*this);
			This::saveSettings();				
			This::setInstance(original);
		}

		release();
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	const Kernal NE_DLL &Kernal::operator=(const This& source)
	{
		Super::operator=(source);

		return _assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	type_bool NE_DLL Kernal::operator==(const This& source) const
	{
		if(this == &source) return true;
		if(Super::operator==(source) == false) return false;
		//	인스턴스 체크:
		if(_ini_manager != source._ini_manager) return false;
		if(_debug_manager != source._debug_manager) return false;
		if(_module_manager != source._module_manager) return false;
		//		NENodeManager:		
		if(_node_manager)
		{
			if(source._node_manager)
			{
				if(*_node_manager != *(source._node_manager)) return false;
			}
			else
				return false;
		}
		else if(source._node_manager)
			return false;
		//		NEScriptManager:
		if(_script_manager)
		{
			if(source._script_manager)
			{
				if(*_script_manager != *(source._script_manager)) return false;
			}
			else
				return false;
		}
		else if(source._script_manager)
			return false;
		if(_node_manager_identifier != source._node_manager_identifier) return false;
		if(_script_manager_identifier != source._script_manager_identifier) return false;
		if(_settings != source._settings) return false;
		if(_default_settings != source._default_settings) return false;
		if(_is_setting_initialized != source._is_setting_initialized) return false;
		
		return true;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	type_bool NE_DLL Kernal::operator!=(const This& source) const
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	type_result NE_DLL Kernal::execute()
	{
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	설명	:	초기화 한다.
	//				포인터를 릴리즈하는, Kernal::release() 함수처럼 완전히 초기화 하는것이
	//				아니기 때문에 매니져클래스의 포인터들과 헤더정보는 유지가 된다.
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	void NE_DLL Kernal::release() 
	{	
		Super::release();	

		_release();
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NEBinaryFileSaver NE_DLL &Kernal::serialize(NEBinaryFileSaver& saver) const
	{
		Super::serialize(saver);
		
		saver	<< _ini_manager << _debug_manager << _module_manager << _settings << _default_settings
				<< _node_manager_identifier << _script_manager_identifier << _is_setting_initialized;
		
		if(_node_manager)
			saver << *_node_manager;
			
		if(_script_manager)
			saver << *_script_manager;
		
		return saver;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	NEBinaryFileLoader NE_DLL &Kernal::serialize(NEBinaryFileLoader& loader)
	{
		Super::serialize(loader);

		loader	>> _ini_manager >> _debug_manager >> _module_manager >> _settings >> _default_settings 
				>> _node_manager_identifier >> _script_manager_identifier >> _is_setting_initialized;

		if(_node_manager)
			loader >> *_node_manager;

		if(_script_manager)
			loader >> *_script_manager;

		return loader;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	type_result NE_DLL Kernal::isValid() const
	{
		type_result result = Super::isValid();
		if(NEResult::hasError(result)) return result;
		result = _ini_manager.isValid();
		if(NEResult::hasError(result)) return result;
		result = _debug_manager.isValid();
		if(NEResult::hasError(result)) return result;
		result = _module_manager.isValid();
		if(NEResult::hasError(result)) return result;
		result = _node_manager_identifier.isValid();
		if(NEResult::hasError(result)) return result;
		result = _script_manager_identifier.isValid();
		if(NEResult::hasError(result)) return result;
		result = _settings.isValid();
		if(NEResult::hasError(result)) return result;
		result = _default_settings.isValid();
		if(NEResult::hasError(result)) return result;
		if(_script_manager)
		{
			result = _script_manager->isValid();
			if(NEResult::hasError(result)) return result;
		}
		if(_node_manager)
		{
			result = _node_manager->isValid();
			if(NEResult::hasError(result)) return result;
		}

		return RESULT_SUCCESS;
	}

	//	헤더의 valid 체크는 Factory에서 검사한다
	//	여기서는 모두 valid 하다는 전제하에 진행한다
	type_result NE_DLL Kernal::initialize()
	{	
		//	pre:		
		//		세팅 로드:
		_ini_manager.initialize();
		_initializeSettings();
		
		
		
		//	main:
		//		모듈매니져 초기화:	모듈을 가져온다	
		type_result result = _key_manager.initialize();
		result |= _module_manager.initialize();
		result |= _debug_manager.initialize(); // 디버그매니져는 모듈매니져가 초기화되고 나서 초기화 해야한다. _flagset 초기화가 moduleset을 이용하기 때문이다
		//	main:		
		//		manager 포인터 할당:
		result |= _pointManagerInstance();		
		//		매니져 클래스 초기화:
		//			settings:	
		//				모듈매니져에서 settings 값을 이용하므로, Setting의 초기화에는 NEINIManager
		//				가 필요하므로 모듈매니져를 초기화 하고 나서 pointManager를 한 뒤에 
		//				읽어야 한다.
		if(_node_manager)
			result |= _node_manager->initialize();
		if(_script_manager)
			result |= _script_manager->initialize();


		
		//	post:
		return result;
	}

	//	---------------------------------------------------------------------------------
	//	설명	:	모듈의 헤더를 내보낸다. 헤더는 클래스에 이미 정의되어서 컴파일된다.
	//	동작조건:
	//	메모	:	헤더는 Static으로 되어있다.
	//	히스토리:	2011-07-07	이태훈	개발 완료
	//				2011-07-09	이태훈	변경		
	//					:	노드엔진의 헤더를 추가했고, 데이터를 노드엔진 헤더에 통합했다.
	//						처음에는 컴파일시 정한 디폴트값이 들어가고, 이 후 INI로부터 
	//						데이터를 가져온다.
	//	---------------------------------------------------------------------------------
	const NEExportable::ModuleHeader NE_DLL &Kernal::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			//	디폴트 값으로 할당:
			//		식별자 할당:
			_header.getName() = _T(_NAME);
			_header.getDeveloper() = _T(_DEVELOPER);
			_header.setRevision(_REVISION);
			_header.getReleaseDate() = _T(_DATE);
			_header.getVersion()  = _T(_VERSION);
			_header.getComment() = _T(_COMMENT);
			_header.setMaxErrorCodeCount(0);
			//_header.getModuleDependencies() = _T(_DEPENDENCIES );
			//_header.version_compatibilities = _T(_VERSION_COMPATIBILITIES);
		}

		return _header;
	}

	const Kernal::ModifiableSettings NE_DLL &Kernal::getSettings()
	{
		return _settings;
	}

	const Kernal::ModifiableSettings NE_DLL &Kernal::getDefaultSettings() const
	{
		return _default_settings;
	}



	//	---------------------------------------------------------------------------------
	//	히스토리:	2011-08-08	이태훈	개발 완료
	//	---------------------------------------------------------------------------------
	const Kernal& Kernal::_assign(const This& source)
	{
		//	pre:
		if(this == &source) return *this;
		
		
		
		//	main:
		//		초기화:	
		//			어짜피 할당을 수행하면 초기화가 일어나는 것과 마찬가지다. 포인터 유무로 판단하기가
		//			까다로우니, 애초에 초기화를 하고 들어가자.
		release();
		_settings = source._settings;
		_default_settings = source._default_settings;
		_is_setting_initialized = source._is_setting_initialized;
		//		Identifier:
		_script_manager_identifier = source._script_manager_identifier;
		_node_manager_identifier = source._node_manager_identifier;
		//		ModuleManager:
		_ini_manager = source._ini_manager;
		_debug_manager = source._debug_manager;
		_module_manager = source._module_manager; //	오류가 있을 수 있다
		//		Manager Pointing:
		//			인스턴스를 무작정 모듈매니져로부터 가져오기보다는, source가 모듈들을
		//			fetch 여부를 판단해야 한다. 왜냐하면 
		//				1. source의 상황과 일치시켜야 하므로.
		//				2. 쓸데없는 부하를 줄여야 하므로.
		//			fetch 여부 판단:
		if(source._node_manager || source._script_manager) // 하나라도 포인터가 할당되어있다면, 로드를 했단 얘기다.
		{
			//		할당:
			//			모듈 fetch:	모듈매니져가 모듈을 fetch 한다.
			_module_manager.initialize(); 
			//			포인터 할당:	fetch한 모듈중에서 매니져클래스를 찾아서 포인터에 넣는다
			_pointManagerInstance();
			//			매니져클래스의 대입연산:
			if(source._script_manager)
			{
				if(_script_manager)
					*_script_manager = *source._script_manager;
				else
				{
					KERNAL_ERROR(" : ")
					return *this;
				}
			}
			if(source._node_manager)
			{
				if(_node_manager)
					*_node_manager = *source._node_manager;
				else
				{
					KERNAL_ERROR(" : ")
					return *this;
				}
			}
		}



		//	post:
		return *this;
	}

	LPCTSTR Kernal::getErrorMessage(type_errorcode errorcode) const
	{
		switch(errorcode)
		{
		case NEMODULE_IS_NOT_VALID:
			return _T("잘못된 Module");
		case INI_MANAGER_IS_NOT_VALID:
			return _T("잘못된 INI 매니져");
		case DEBUG_MANAGER_IS_NOT_VALID:
			return _T("잘못된 디버그 매니져");
		case MODULE_MANAGER_IS_NOT_VALID:
			return _T("잘못된 모듈 매니져");
		case SCRIPT_MANAGER_IS_NOT_VALID:
			return _T("잘못된 스크립트 매니져");
		case NODE_MANAGER_IS_NOT_VALID:
			return _T("잘못된 노드 매니져");
		default:
			return _T("알수 없는 에러");
		}
	}
	
	NEINIManager NE_DLL &Kernal::getINIManager()
	{
		return _ini_manager;
	}

	const NEINIManager& Kernal::getINIManager() const
	{
		return _ini_manager;
	}
	NEDebugManager NE_DLL &Kernal::getDebugManager()
	{
		return _debug_manager;
	}

	const NEDebugManager& Kernal::getDebugManager() const
	{
		return _debug_manager;
	}
	NEKeyManager& Kernal::getKeyManager()
	{
		return _key_manager;
	}
	const NEKeyManager& Kernal::getKeyManager() const
	{
		return _key_manager;
	}
	NEPackageManager NE_DLL &Kernal::getModuleManager()
	{
		return _module_manager;
	}

	const NEPackageManager& Kernal::getModuleManager() const
	{
		return _module_manager;
	}
	NENodeManager NE_DLL &Kernal::getNodeManager()
	{
		if(!_node_manager)
			KERNAL_ERROR(" : ")

		return *_node_manager;
	}

	const NENodeManager& Kernal::getNodeManager() const
	{
		if(!_node_manager)
			KERNAL_ERROR(" : ")

		return *_node_manager;
	}
	NEScriptManager NE_DLL &Kernal::getScriptManager()
	{
		if(!_script_manager)
			KERNAL_ERROR(" : ")

		return *_script_manager;
	}

	const NEScriptManager& Kernal::getScriptManager() const
	{
		if(!_script_manager)
			KERNAL_ERROR(" : ")

		return *_script_manager;
	}
	type_result Kernal::_pointManagerInstance()
	{
		//		INI매니져:	디버그매니져에서 INI가 필요하므로 INI매니져를 먼저 추가한다.
		//		스크립트 매니져:
		if( !NEResult::hasError(_script_manager_identifier.isValid()) )
		{	
			const NEScriptManager* script_manager_pointer = static_cast<const NEScriptManager*>( &(_module_manager.getModule(_script_manager_identifier)));
			if( ! script_manager_pointer)
				KERNAL_ERROR(" : ")
			else
				_script_manager = static_cast<NEScriptManager*>( & script_manager_pointer->clone());
		}
		else
			KERNAL_ERROR(" : ")
		//		노드매니져:
		if( !NEResult::hasError(_node_manager_identifier.isValid()) )
		{
			const NENodeManager* node_manager_pointer = static_cast<const NENodeManager*>( &(_module_manager.getModule(_node_manager_identifier)));
			if( ! node_manager_pointer)
				KERNAL_ERROR(" : ")
			else
				_node_manager = static_cast<NENodeManager*>( & node_manager_pointer->clone());			
		}
		else
			KERNAL_ERROR(" : ")

		return RESULT_SUCCESS;
	}

	void Kernal::_releaseManagerPointer()
	{
		if(_node_manager)
		{
			delete _node_manager;
			_node_manager = NE_NULL;
		}
		if(_script_manager)
		{
			delete _script_manager;
			_script_manager = NE_NULL;
		}
	}

	type_result NE_DLL Kernal::loadSettings()
	{
		type_result result = RESULT_SUCCESS;
		NETString buffer;

		NEResult::addErrorFlag(result, _ini_manager.readModuleIniString(getHeader(), _LOCALE_INI_KEY, _settings.getLocale()));
		NEResult::addErrorFlag(result, _ini_manager.readModuleIniString(getHeader(), _LOG_DIRECTORY_INI_KEY, buffer));
		_settings.setLogDirectory(buffer);
		NEResult::addErrorFlag(result, _ini_manager.readModuleIniString(getHeader(), _SCRIPT_DIRECTORY_INI_KEY, buffer));
		_settings.setScriptDirectory(buffer);
		NEResult::addErrorFlag(result, _ini_manager.readModuleIniString(getHeader(), _NODE_DIRECTORY_INI_KEY, buffer));
		_settings.setNodeDirectory(buffer);
		NEResult::addErrorFlag(result, _ini_manager.readModuleIniString(getHeader(), _DEBUG_FLAG_INI_KEY, buffer));
		_settings.getDebugFlag() = NEDebugManager::Flag( buffer.toInt() );
		
		return result;
	}

	type_result NE_DLL Kernal::saveSettings()
	{
		//	pre:
		//		타겟팅:
		This& This = This::getInstance();
		if( ! &This)
		{
			KERNAL_ERROR(" : ")
			return RESULT_TYPE_ERROR;
		}
		NEINIManager& ini = This.getINIManager();
		const This::ModifiableSettings& settings = This.getSettings();
		const NEExportable::Identifier& identifier = This.getHeader();
	
		type_result result = RESULT_SUCCESS;

		NEResult::addErrorFlag(result, ini.writeModuleIniString(identifier, _LOCALE_INI_KEY, settings.getLocale()));
		NEResult::addErrorFlag(result, ini.writeModuleIniString(identifier, _LOG_DIRECTORY_INI_KEY, settings.getLogDirectory()));
		NEResult::addErrorFlag(result, ini.writeModuleIniString(identifier, _SCRIPT_DIRECTORY_INI_KEY, settings.getScriptDirectory()));
		NEResult::addErrorFlag(result, ini.writeModuleIniString(identifier, _NODE_DIRECTORY_INI_KEY, settings.getNodeDirectory()));
		//	왜 flag를 int형으로 형변환 하는가?:
		//		flag를 그대로 NETString으로 형변환 할 수 있지만, 
		//		그리하면 INI에서 유저가 수정불가능한 문자가 되어 나온다.
		//		int형으로 형변환 하면 숫자의 형태로 나오므로 수정이 가능해진다.
		int data = static_cast<int>(settings.getDebugFlag().getFlag());
		NEResult::addErrorFlag(result, ini.writeModuleIniString(identifier, _DEBUG_FLAG_INI_KEY, data));
		
		This.getDebugManager().saveToINI();

		return result;
	}

	type_result NE_DLL Kernal::setInstance(This& instance)
	{
		//	pre:
		if( ! &instance)
		{
			KERNAL_ERROR(" : ")
			return RESULT_TYPE_ERROR;
		}
		
		
		
		//	main:
		//		인스턴스 복사:		
		_instance = &instance;
		


		//	post:
		return RESULT_SUCCESS;
	}

	Kernal NE_DLL &Kernal::getInstance()
	{
		return *_instance;
	}



	void Kernal::_initializeSettings()
	{
		//	INI로부터 읽어야 하는가:
		if(_settings.isLoadedFromINI())
		{	//	INI로부터 로드:
			if(NEResult::hasError(loadSettings()))
			{	//	INI로드 중 에러가 발생하면:	디폴트로부터 가져온다
				KERNAL_WARNING(" : 세팅을 로드하는 중 에러")
			}
		}
		else
		{
			KERNAL_INFORMATION(" : INI를 사용하지 않음")
		}
			
				
		
		if(NEResult::hasError(_settings.isValid()))
		{
			KERNAL_WARNING(" : 가지고 있는 세팅이 잘못되서 기본(디폴트) 세팅으로 적용 시도")
			_initializeSettingsToDefaultSettingsOrReleaseBoth();
		}
	
		_is_setting_initialized = true;
	}

	void Kernal::_initializeSettingsToDefaultSettingsOrReleaseBoth()
	{
		//			근데 디폴트값도 invalid 하다면:
		if(NEResult::hasError(_default_settings.isValid()))
		{	//			둘다 release 한다:
			KERNAL_ERROR(" : ")
			_default_settings.release();
			_settings.release();					
		}
		else
		{	//	디폴트는 valid하다면:
			KERNAL_WARNING(" : 로드에 실패해서 기본 세팅으로 읽어들임")
			_settings = _default_settings;
		}
	}

	void NE_DLL Kernal::_release()
	{	
		/*
			release를 호출하는 순서에 주의:
				생성한 순서에 역순으로 해제해야하는 것이 기본이다. 
		*/
		_releaseManagerPointer();
		_debug_manager.release();
		_module_manager.release();
		_ini_manager.release();
		_settings.release();
		_default_settings.release();
		_node_manager_identifier.release();
		_script_manager_identifier.release();
		_is_setting_initialized = false;
	}
}

NE::Kernal* NE::Kernal::_instance = NE_NULL;
