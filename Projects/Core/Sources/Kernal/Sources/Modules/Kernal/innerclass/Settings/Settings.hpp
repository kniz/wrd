//	값은 커널 생성시에만 기본적으로 주어지며, 커널내의 주요 변수들은 이 값들로
//	초기화된다.
//	디폴트 세팅은 팩토리로부터만 생성되며, 일단 커널이 생성되면 변경 할 수 없다.
//	이건 팩토리로부터만 변경되어지는 변수가 들어있다.

//	프렌드 선언을 위한 전방선언:
class Factory;

class NE_DLL Settings : public NEObject
{
	NE_DECLARE_CLASS(Settings, NEObject)

public:
	Settings();
	Settings(const This& source);

public:
	virtual ~Settings();

public:
	const This& operator=(const This& source);
	type_bool operator==(const This& source) const;
	type_bool operator!=(const This& source) const;

public:
	virtual void release();
	virtual type_result isValid() const;
	NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
	NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);

public:
	const NETString& getTitle() const;
	const NETStringSet& getLocaleCompatibilities() const;
	const NETStringSet& getExecutablePlatfroms() const;
	const NETString& getINIFullPath() const;
	const NETString& getINIForms() const;
	const NETString& getLogExtractor() const;
	const NETString& getModuleDirectory() const;
	const NETString& getScriptExtractor() const;
	const NETString& getNodeExtractor() const;
	NETString& getTitle();
	NETStringSet& getLocaleCompatibilities();
	NETStringSet& getExecutablePlatfroms();
	NETString& getINIFullPath();
	NETString& getINIForms();
	NETString& getLogExtractor();
	type_result setModuleDirectory(const NETString& module_directory);
	NETString& getScriptExtractor();
	NETString& getNodeExtractor();
	type_bool isKernalUsingUnicode() const;
	void setKernalUsingUnicode(type_bool use_unicode);	
	int getMainScriptIndex() const;
	void setMainScriptIndex(type_index index);
	type_bool isLoadedFromINI() const;
	void setLoadFromINI(type_bool enable);


protected:
	const This& _assign(const This& source);
	type_result _fixWhenDirectoryHasNotSlashAtTheEnd(NETString& directory_string_to_be_validate) const;

protected:
	NETString _title;
	NETStringSet _locale_compatibilities;
	NETStringSet _executable_platforms;
	NETString _ini_fullpath;
	NETString _ini_forms;
	NETString _log_extractor;
	NETString _module_directory;
	NETString _script_extractor;
	NETString _node_extractor;	
	int _main_script_index;
	type_bool _is_kernal_using_unicode;
	type_bool _is_loading_from_ini;
};