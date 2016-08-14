//	isFixedType:
//		FixedType은 이 인스턴스는 반드시 이 CodeType이 유지가 되어야 한다는 걸
//		의미한다.
//		Script Type이라고 하더라도, Fixed 선언이 되어있지 않으면 Target의 Type
//		으로 처리될 가능성이 존재한다.
//		하지만, 양쪽 모두 Fixed이며, 서로 다른 타입일 경우에는 연산이 성립되지
//		않는다.
//		Fixed는 Unique 속성으로써 다른 인스턴스에 할당하거나 할당될 수 없다.
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
//		*1)	단, Type이 같다면 성립한다.
//
//	operator+, +=, -=, -, *, *=, /=의 연산은 적용이 되면 안된다:
//
#pragma once

#include "../../../Modules/NEObject/NEObject.hpp"

namespace NE
{
	class NE_DLL NECodeType : public NEObject
	{
		NE_DECLARE_CLASS(NECodeType, NEObject)

	public:
		typedef NEObject SuperClass;
		typedef NECodeType ThisClass;

	public:
		enum CodeType
		{
			CODETYPE_START = -1,
			UNDEFINED = 0,
			SCRIPT,
			NAME,
			GROUP,
			PRIORITY,
			MODULE_SCRIPT,
			MODULE_NAME,
			RECENT,
			ME,
			ALL,
			CODETYPE_END
		};

	public:
		NECodeType();
		NECodeType(CodeType new_codetype);
		NECodeType(CodeType new_codetype, type_bool is_type_fixed);
		NECodeType(const ThisClass& rhs);

	public:
		ThisClass& operator=(const ThisClass& source);
		type_bool operator==(const NECodeType::CodeType source) const;
		type_bool operator==(const ThisClass& source) const;
		type_bool operator!=(const NECodeType::CodeType source) const;
		type_bool operator!=(const ThisClass& source) const;
		type_bool operator<(const NECodeType::CodeType source) const;
		type_bool operator<(const ThisClass& source) const;
		type_bool operator<=(const NECodeType::CodeType source) const;
		type_bool operator<=(const ThisClass& source) const;
		type_bool operator>(const NECodeType::CodeType source) const;
		type_bool operator>(const ThisClass& source) const;
		type_bool operator>=(const NECodeType::CodeType source) const;
		type_bool operator>=(const ThisClass& source) const;

	public:
		CodeType getCodeType() const;
		type_result setCodeType(CodeType new_one);
		type_bool isTypeFixed() const;
		type_bool isAcceptableType(const NECodeType& source) const;

	public:
		virtual void release();
		virtual type_result isValid() const;
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);

	private:
		ThisClass& _assign(const ThisClass& source);

	private:
		CodeType _codetype;
		type_bool _is_type_fixed;
	};
}
