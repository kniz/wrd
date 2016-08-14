class NE_DLL Flag : public NEObject
{
	NE_DECLARE_CLASS(Flag, NEObject)

public:
	Flag();
	Flag(const TCHAR& source);
	Flag(const This& source);

public:
	virtual ~Flag();
	
public:
	const This& operator=(const This& source);
	type_bool operator==(const This& source) const;
	type_bool operator!=(const This& source) const;

public:
	virtual TCHAR getFlag() const;
	virtual void setFlag(TCHAR flag);
	virtual type_bool isLogShown() const;
	virtual void setLogShown(type_bool enable);
	virtual type_bool isConsoleShown() const;
	virtual void setConsoleShown(type_bool enable);
	virtual type_bool isMessageBoxShown() const;
	virtual void setMessageBoxShown(type_bool enable);
	virtual type_bool isDebugWindowShown() const;
	virtual void setDebugWindowShown(type_bool enable);

public:
	virtual void release();
	virtual type_result isValid() const;
	virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
	virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
	
private:
	const This& _assign(const This& source);
	void _release();

protected:
	//	왜 TCHAR가 아니라 TCHAR여야 하는가?:
	//		UNICODE컴파일시, INI를 유니코드로 저장하기 때문에 INI가 WBCS인지 MBCS인지 알수
	//		없다. _flag는 INI로부터 데이터를 가져오므로 그에 맞춰서 데이터를 정할 필요가
	//		있다.
	TCHAR _flag;
};
