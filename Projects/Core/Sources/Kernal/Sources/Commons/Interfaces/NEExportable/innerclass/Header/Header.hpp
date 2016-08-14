//	---------------------------------------------------------------------------------
//	클래스명:	NEExportable::Header
//	설명	:	모듈의 헤더를 나타낸다.
//	관계	:	기반클래스.			Identifier
//				소유되는 클래스.	NEModule
//	특성	:	중첩클래스.
//	알고리즘:	
//	사용방법:	
//	메모	:	
//	히스토리:	2011-07-07	이태훈	개발 완료	
//				2011-07-09	이태훈	추가		버전 호환성 추가
//												version_compatibilities에 들어가 있는 
//												버전들은 하위 버전으로써, 이 모듈로도
//												실행이 가능하다는 뜻이다.
//	---------------------------------------------------------------------------------
class NE_DLL Header : public Identifier
{
	NE_DECLARE_CLASS(Header, Identifier)

	//	생성자:
public:
	Header();
	Header(const Header& source);

	//	소멸자:
public:
	virtual ~Header();

	//	연산자 중첩:
public:
	const Header& operator=(const Header& source);
	type_bool operator==(const Header& source) const;
	type_bool operator!=(const Header& source) const;
	
public:
	NETString& getReleaseDate();	
	const NETString& getReleaseDate() const;
	NETString& getVersion();
	const NETString& getVersion() const;
	NETString& getComment();
	const NETString& getComment() const;
	NETString& getContact();
	const NETString& getContact() const;

	//	인터페이스:
	//		상속:
	//			NEObject:
public:
	virtual void release();
	virtual type_result isValid() const;
	virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
	virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);

	//	내부함수:
private:	
	const Header& _assign(const Header& source);

	//	멤버변수:
protected:
	NETString _release_date;
	NETString _version;
	NETString _comment; // 모듈 설명
	NETString _contact;
};
