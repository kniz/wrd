//	UNDEFINED:
//		코드의 타입이 정의되지 않았다. 따라서 Target의 CodeType을 따라간다.
//		void* 혹은 var 와도 같은 거라 보면 된다. 어떤 CodeType으로도 취급된다.
//
//	setCode(getCode() + rhs.getCode())와 operator+(rhs)의 차이:
//		-	setCode는 Code만 놓고 적합성을 따지며(NEScriptHeader에 명시된 max_code로
//			적합성 여부를 검증한다)
//		-	operator+는 CodeType을 통한 적합성 여부를 검증하고, setCode를 호출
//			한다.
//
//		다시말해서, operator+(rhs)를 하게되면, 주어진 rhs가 적합한 CodeType인지
//		검사하며, 아닐 경우 작업을 취소한다.
//		반면, setCode를 하는 경우는 rhs가 어떤 타입인지는 고려하지 않겠다는 걸 
//		의미한다. 즉, operator+(NECode(NEcodeType::UNDEFINED)) 와 같은 것.
//
//	FixedType간 연산 성립 테이블:
//		This = Source의 연산의 경우,
//		+===================================================+
//		|	This \ Source	|	Flexible	|	Fixed		|
//		|====================================================
//		|	Flexible		|	Source's	|	Source's	|
//		|---------------------------------------------------|
//		|	Fixed			|	Mine		|	Mine		|
//		|====================================================
#pragma once

#include "../NECodeType/NECodeType.hpp"
#include "../../Interfaces/NEExportable/NEExportable.hpp"

namespace NE
{
	class NE_DLL NECode : public NECodeType
	{
		NE_DECLARE_CLASS(NECode, NECodeType)

	public:
		typedef NECode ThisClass;
		typedef NECodeType SuperClass;

	public:
		NECode();
		NECode(const NEExportable::Identifier& identifier);
		NECode(type_code new_code, const NECodeType& codetype = NECodeType());
		NECode(const ThisClass& source);

	public:
		virtual ~NECode();

	public:
		ThisClass& operator=(const ThisClass& source);
		NECode& operator++();
		NECode operator++(int);
		NECode& operator--();
		NECode operator--(int);
		type_bool operator==(const ThisClass& source) const;
		type_bool operator==(const type_code source) const;
		type_bool operator!=(const type_code source) const;
		type_bool operator!=(const ThisClass& source) const;
		type_bool operator<(const ThisClass& source) const;
		type_bool operator<=(const ThisClass& source) const;
		type_bool operator>(const ThisClass& source) const;
		type_bool operator>=(const ThisClass& source) const;
		ThisClass operator+(const ThisClass& source) const;
		ThisClass operator-(const ThisClass& source) const;
		ThisClass operator*(const ThisClass& source) const;
		ThisClass operator/(const ThisClass& source) const;
		ThisClass operator%(const ThisClass& source) const;
		ThisClass& operator+=(const ThisClass& source);
		ThisClass& operator-=(const ThisClass& source);
		ThisClass& operator*=(const ThisClass& source);
		ThisClass& operator/=(const ThisClass& source);		
		ThisClass& operator%=(const ThisClass& source);
		operator type_code();

	public:
		type_code getCode() const;
		type_result setCode(type_code new_code);
		type_result setCode(const NEExportable::Identifier& identifier);

	public:
		virtual void release();
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual type_result isValid() const;

	private:
		ThisClass& _assign(const ThisClass& source);
		NEBinaryFileSaver& _serializeAsScript(NEBinaryFileSaver& saver) const;
		NEBinaryFileLoader& _serializeAsScript(NEBinaryFileLoader& loader);
		void _release();
		ThisClass _createCode(const NECode& source, type_code new_code) const;

	private:
		type_code _code;
	};
}