class NE_DLL Factory : public NEObject, public NEInitializable
{
	NE_DECLARE_CLASS(Factory, NEObject)

public:
	Factory();
	Factory(const Settings& base_settings);
	Factory(const This& source);

public:
	virtual ~Factory();

public:
	const This& operator=(const This& source);
	type_bool operator==(const This& source) const;
	type_bool operator!=(const This& source) const;

public:
	NEExportable::Identifier& getNodeManagerIdentifier();
	const NEExportable::Identifier& getNodeManagerIdentifier() const;
	NEExportable::Identifier& getScriptManagerIdentifier();
	const NEExportable::Identifier& getScriptManagerIdentifier() const;
	Kernal::ModifiableSettings& getDefaultSettings();
	const Kernal::ModifiableSettings& getDefaultSettings() const;
	
public:
	virtual type_result initialize();

public:
	virtual void release();
	virtual type_result isValid() const;
	virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
	virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;

public:
	Kernal create();
	
protected:
	const This& _assign(const This& source);

protected:
	NEExportable::Identifier _node_manager_identifier;
	NEExportable::Identifier _script_manager_identifier;
	Kernal::ModifiableSettings _settings;
};
