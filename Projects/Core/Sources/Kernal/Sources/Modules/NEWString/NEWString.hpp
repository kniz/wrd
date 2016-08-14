//	---------------------------------------------------------------------------------
//	클래스명:	NEWString
//	설명	:	NETArray를 통해서 구현된, 문자열 클래스.
//	관계	:	기반클래스.		NEWCharSet
//				외부라이브러리.	tchar.h
//	특성	:	형변환에 CRT를 사용하고 있다.
//	알고리즘:	NETArray를 참고하라.
//	사용방법:	기본적인 인터페이스는 NETArray를 참고하라.
//				추가 인터페이스
//					1. toInt()			:	문자열을 int형으로 변환한다. (_tstoi 사용)
//					2. toDouble()		:	문자열을 double로 변환한다.	(_tstof 사용)
//					3. toCHARPointer()	:	문자열을 LPTSTR로 변환한다.
//	메모	:	형변환이 많으면 모호성의 오류가 발생하기 때문에 대부분의 형변환은 
//				NEWString에 위치하고 있다.
//	히스토리:	2011-07-07	이태훈	개발 완료	
//	---------------------------------------------------------------------------------
#pragma once

#include "../NEWCharSet/NEWCharSet.hpp"
#include "../NECharSet/NECharSet.hpp"
#include "../NEIntSet/NEIntSet.hpp"

namespace NE
{
	class NEString;
	class NEWString;
	class NEWStringSet;

	class NE_DLL NEWString : public NEWCharSet
	{
	public:
		NE_DECLARE_CLASS_ONLY(NEWString, NEWCharSet)

		//	생성자:
	public:
		NEWString();		
		NEWString(LPCWSTR source);
		NEWString(LPCSTR source);
		NEWString(const NEWString& source);
		NEWString(const NEWCharSet& source);
		NEWString(const NEString& source);
		NEWString(const NECharSet& source);
		NEWString(const type_int data);
		NEWString(const type_uint data);
		NEWString(const type_double data);
		NEWString(const type_char data);
		NEWString(const type_uchar data);
		NEWString(const type_wchar data);
		NEWString(const type_bool data);
		NEWString(const type_int64 data);
		NEWString(const type_uint64 data);

		//	소멸자:
	public:
		virtual ~NEWString();

		//	연산자 중첩:
	public:
		const NEWString& operator=(const NEWString& source);
		type_bool operator==(const NEWString& source) const;
		type_bool operator!=(const NEWString& source) const;
		NEWString operator+(const NEWString& source) const;
		const NEWString& operator+=(const NEWString& source);

		//	인터페이스: 
		//		상속:
		//			NEObject:
	public:
		virtual type_result isValid() const; 
		virtual const NEClassBase& getClass() const;

		//		고유 인터페이스:
	public:		
		using NEWCharSet::find;
		NEIntSet find(const NEWString& sample) const;
		type_result split(const NEWString& delimiter, NE_OUT NEWStringSet& buffer_from_outside) const;
		NEWString extract(type_index start_index, type_index end_index) const;
		int toInt() const;
		double toDouble() const;
		type_bool toBoolean() const;
		wchar_t* toCharPointer();		
		const wchar_t* const toCharPointer() const;
		type_result setLowerCase();
		NEWString toLowerCase() const;
		type_result setUpperCase();
		NEWString  toUpperCase() const;
		void from(const type_int data);
		void from(LPCSTR data);
		void from(LPCWSTR data);
		void from(const type_double data);
		void from(const type_bool data);
		void from(const type_uint data);
		void from(const type_uchar data);
		void from(const type_wchar data);
		void from(const type_char data);		
		void from(const type_int64 data);
		void from(const type_uint64 data);

	private:
		void _convertString(LPCSTR source);
		const NEWString& _assign(LPCWSTR source);

		//	더미함수:
	public:													
		static const NEClassBase& getClassStatically();

	public:
		friend NEWString NE_DLL operator+(LPCWSTR source1, const NEWString& source2);
	};
}
