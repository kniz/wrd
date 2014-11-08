#pragma warning(push)
#pragma warning(disable : 4800)
#pragma warning(disable : 4804)
#include "NEKeyTemplate.hpp"
#include "../../../Modules/NEBooleanKey/NEBooleanKey.hpp"
#include "../../../Modules/NEByteKey/NEByteKey.hpp"
#include "../../../Modules/NECharKey/NECharKey.hpp"
#include "../../../Modules/NEUByteKey/NEUByteKey.hpp"
#include "../../../Modules/NEWCharKey/NEWCharKey.hpp"
#include "../../../Modules/NEShortKey/NEShortKey.hpp"
#include "../../../Modules/NEUShortKey/NEUShortKey.hpp"
#include "../../../Modules/NEIntKey/NEIntKey.hpp"
#include "../../../Modules/NEUIntKey/NEUIntKey.hpp"
#include "../../../Modules/NEFloatKey/NEFloatKey.hpp"
#include "../../../Modules/NEDoubleKey/NEDoubleKey.hpp"
#include "../../../Modules/NEInt64Key/NEInt64Key.hpp"
#include "../../../Modules/NEStringKey/NEStringKey.hpp"
#include "../../../Modules/NEWStringKey/NEWStringKey.hpp"
#include "../../../Modules/NEBooleanSetKey/NEBooleanSetKey.hpp"
#include "../../../Modules/NEByteSetKey/NEByteSetKey.hpp"
#include "../../../Modules/NEUByteSetKey/NEUByteSetKey.hpp"
#include "../../../Modules/NEShortSetKey/NEShortSetKey.hpp"
#include "../../../Modules/NEUShortSetKey/NEUShortSetKey.hpp"
#include "../../../Modules/NEIntSetKey/NEIntSetKey.hpp"
#include "../../../Modules/NEUIntSetKey/NEUIntSetKey.hpp"
#include "../../../Modules/NEInt64SetKey/NEInt64SetKey.hpp"
#include "../../../Modules/NEFloatSetKey/NEFloatSetKey.hpp"
#include "../../../Modules/NEDoubleSetKey/NEDoubleSetKey.hpp"
#include "../../../Modules/NEModuleCodeSetKey/NEModuleCodeSetKey.hpp"
#include "../../../Modules/NENodeCodeSetKey/NENodeCodeSetKey.hpp"

namespace NE
{
	template <>
	type_result NE_DLL NEKeyTemplate<type_bool, NEType::NEBOOLEAN_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;

		SuperClass::assign(source);

		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = static_cast<const NEBooleanKey&>(source).getValue();
			break;
		case NEType::NECHAR_KEY:
			_value = ( (static_cast<const NECharKey&>(source)).getValue() != 0);
			break;
		case NEType::NEBYTE_KEY:
			_value = ( (static_cast<const NEByteKey&>(source)).getValue() != 0);
			break;
		case NEType::NEUBYTE_KEY:
			_value = ( (static_cast<const NEUByteKey&>(source)).getValue() != 0);
			break;
		case NEType::NEWCHAR_KEY:
			_value = ( (static_cast<const NEWCharKey&>(source)).getValue() != 0);
			break;
		case NEType::NESHORT_KEY:
			_value = ( (static_cast<const NEShortKey&>(source)).getValue() != 0);
			break;
		case NEType::NEUSHORT_KEY:
			_value = ( (static_cast<const NEUShortKey&>(source)).getValue() != 0);
			break;
		case NEType::NEINT_KEY:
			_value = ( (static_cast<const NEIntKey&>(source)).getValue() != 0);
			break;
		case NEType::NEUINT_KEY:
			_value = ( (static_cast<const NEUIntKey&>(source)).getValue() != 0);
			break;		
		case NEType::NEFLOAT_KEY:
			_value = ( (static_cast<const NEFloatKey&>(source)).getValue() != 0.0f);
			break;
		case NEType::NEDOUBLE_KEY:
			_value = ( (static_cast<const NEDoubleKey&>(source)).getValue() != 0.0);
			break;
		case NEType::NEINT64_KEY:
			_value = ( (static_cast<const NEInt64Key&>(source)).getValue() != 0);
			break;
		case NEType::NESTRING_KEY:
			_value = (static_cast<const NEStringKey&>(source)).getValue().toBoolean();
			break;
		case NEType::NEWSTRING_KEY:
			_value = (static_cast<const NEWStringKey&>(source)).getValue().toBoolean();
			break;

		default:
			return RESULT_TYPE_ERROR;
		}
		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEKeyTemplate<type_char, NEType::NECHAR_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;

		SuperClass::assign(source);

		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			_value = static_cast<const NECharKey&>(source).getValue();
			break;
		case NEType::NEBYTE_KEY:
			{
				type_int byte_to_int = (static_cast<const NEByteKey&>(source)).getValue();
				NEString converter(byte_to_int);
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEUBYTE_KEY:
			{
				type_int ubyte_to_int = (static_cast<const NEUByteKey&>(source)).getValue();
				NEString converter(ubyte_to_int);
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEWCHAR_KEY:
			{
				NEString converter((static_cast<const NEWCharKey&>(source)).getValue());
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NESHORT_KEY:
			{
				type_int casted = static_cast<type_int>( (static_cast<const NEShortKey&>(source)).getValue() );
				NEString converter(casted);
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEUSHORT_KEY:
			{
				type_int casted = static_cast<type_int>( (static_cast<const NEUShortKey&>(source)).getValue() );
				NEString converter(casted);
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEINT_KEY:
			{
				NEString converter( (static_cast<const NEIntKey&>(source)).getValue() );
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEUINT_KEY:
			{
				type_int uint_to_int = (static_cast<const NEUIntKey&>(source)).getValue();
				NEString converter(uint_to_int);
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;		
		case NEType::NEFLOAT_KEY:
			{
				NEString converter( (static_cast<const NEFloatKey&>(source)).getValue() );
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEDOUBLE_KEY:
			{
				NEString converter( (static_cast<const NEDoubleKey&>(source)).getValue() );
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;		
		case NEType::NEINT64_KEY:
			{
				int casted = static_cast<type_int>( (static_cast<const NEInt64Key&>(source)).getValue() );
				NEString converter(casted);
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NESTRING_KEY:
			{
				const NEString& extracted = (static_cast<const NEStringKey&>(source)).getValue();

				if(extracted.getLength() > 0)
					_value = extracted[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEWSTRING_KEY:
			{
				NEString converted( (static_cast<const NEWStringKey&>(source)).getValue() );

				if(converted.getLength() > 0)
					_value = converted[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;

		default:
			return RESULT_TYPE_ERROR;
		}

		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEKeyTemplate<type_byte, NEType::NEBYTE_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;

		SuperClass::assign(source);

		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			_value = static_cast<type_byte>( NEString(static_cast<const NECharKey&>(source).getValue()).toInt() );
			break;
		case NEType::NEBYTE_KEY:
			_value = static_cast<const NEByteKey&>(source).getValue();
			break;
		case NEType::NEUBYTE_KEY:
			_value = static_cast<type_byte>((static_cast<const NEUByteKey&>(source)).getValue());
			break;
		case NEType::NEWCHAR_KEY:
			_value = static_cast<type_byte>( NEWString(static_cast<const NEWCharKey&>(source).getValue()).toInt() );
			break;
		case NEType::NESHORT_KEY:
			_value = static_cast<type_byte>((static_cast<const NEShortKey&>(source)).getValue());
			break;
		case NEType::NEUSHORT_KEY:
			_value = static_cast<type_byte>((static_cast<const NEUShortKey&>(source)).getValue());
			break;
		case NEType::NEINT_KEY:
			_value = static_cast<type_byte>((static_cast<const NEIntKey&>(source)).getValue());
			break;
		case NEType::NEUINT_KEY:
			_value = static_cast<type_byte>((static_cast<const NEUIntKey&>(source)).getValue());
			break;		
		case NEType::NEFLOAT_KEY:
			_value = static_cast<type_byte>((static_cast<const NEFloatKey&>(source)).getValue());
			break;
		case NEType::NEDOUBLE_KEY:
			_value = static_cast<type_byte>((static_cast<const NEDoubleKey&>(source)).getValue());
			break;		
		case NEType::NEINT64_KEY:
			_value = static_cast<type_byte>((static_cast<const NEInt64Key&>(source)).getValue());
			break;
		case NEType::NESTRING_KEY:
			{
				const NEString& extracted = (static_cast<const NEStringKey&>(source)).getValue();

				_value = static_cast<type_byte>(extracted.toInt());
			}
			break;
		case NEType::NEWSTRING_KEY:
			{
				const NEWString& extracted = (static_cast<const NEWStringKey&>(source)).getValue();

				_value = static_cast<type_byte>(extracted.toInt());
			}
			break;

		default:
			return RESULT_TYPE_ERROR;
		}

		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEKeyTemplate<type_ubyte, NEType::NEUBYTE_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;

		SuperClass::assign(source);


		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			_value = static_cast<type_ubyte>( NEString(static_cast<const NECharKey&>(source).getValue()).toInt() );
			break;
		case NEType::NEBYTE_KEY:
			_value = static_cast<type_ubyte>((static_cast<const NEByteKey&>(source)).getValue());
			break;
		case NEType::NEUBYTE_KEY:
			_value = (static_cast<const NEUByteKey&>(source)).getValue();
			break;
		case NEType::NEWCHAR_KEY:
			_value = static_cast<type_ubyte>( NEWString(static_cast<const NEWCharKey&>(source).getValue()).toInt() );
			break;
		case NEType::NESHORT_KEY:
			_value = static_cast<type_ubyte>((static_cast<const NEShortKey&>(source)).getValue());
			break;
		case NEType::NEUSHORT_KEY:
			_value = static_cast<type_ubyte>((static_cast<const NEUShortKey&>(source)).getValue());
			break;
		case NEType::NEINT_KEY:
			_value = static_cast<type_ubyte>((static_cast<const NEIntKey&>(source)).getValue());
			break;
		case NEType::NEUINT_KEY:
			_value = static_cast<type_ubyte>((static_cast<const NEUIntKey&>(source)).getValue());
			break;
		case NEType::NEFLOAT_KEY:
			_value = static_cast<type_ubyte>((static_cast<const NEFloatKey&>(source)).getValue());
			break;
		case NEType::NEDOUBLE_KEY:
			_value = static_cast<type_ubyte>((static_cast<const NEDoubleKey&>(source)).getValue());
			break;		
		case NEType::NEINT64_KEY:
			_value = static_cast<type_ubyte>((static_cast<const NEInt64Key&>(source)).getValue());
			break;
		case NEType::NESTRING_KEY:
			{
				const NEString& extracted = (static_cast<const NEStringKey&>(source)).getValue();

				_value = static_cast<type_ubyte>(extracted.toInt());
			}
			break;
		case NEType::NEWSTRING_KEY:
			{
				const NEWString& extracted = (static_cast<const NEWStringKey&>(source)).getValue();

				_value = static_cast<type_ubyte>(extracted.toInt());
			}
			break;

		default:
			return RESULT_TYPE_ERROR;
		}

		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEKeyTemplate<type_wchar, NEType::NEWCHAR_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;

		SuperClass::assign(source);


		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			{
				NEWString converter((static_cast<const NECharKey&>(source)).getValue());
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEBYTE_KEY:
			{
				type_int byte_to_int = (static_cast<const NEByteKey&>(source)).getValue();
				NEWString converter(byte_to_int);
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEUBYTE_KEY:
			{
				type_int ubyte_to_int = (static_cast<const NEUByteKey&>(source)).getValue();
				NEWString converter(ubyte_to_int);
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEWCHAR_KEY:
			_value = (static_cast<const NEWCharKey&>(source)).getValue();
			break;
		case NEType::NESHORT_KEY:
			{
				int casted = static_cast<type_int>( (static_cast<const NEShortKey&>(source)).getValue() );
				NEWString converter(casted);
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEUSHORT_KEY:
			{
				NEWString converter( (static_cast<const NEUShortKey&>(source)).getValue() );
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEINT_KEY:
			{
				NEWString converter( (static_cast<const NEIntKey&>(source)).getValue() );
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEUINT_KEY:
			{
				type_int uint_to_int = (static_cast<const NEUIntKey&>(source)).getValue();
				NEWString converter(uint_to_int);
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEFLOAT_KEY:
			{
				NEWString converter( (static_cast<const NEFloatKey&>(source)).getValue() );
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEDOUBLE_KEY:
			{
				NEWString converter( (static_cast<const NEDoubleKey&>(source)).getValue() );
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;		
		case NEType::NEINT64_KEY:
			{
				int casted = static_cast<type_int>( (static_cast<const NEInt64Key&>(source)).getValue() );
				NEWString converter(casted);
				if(converter.getLength() > 0)
					_value = converter[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NESTRING_KEY:
			{
				NEWString converted( (static_cast<const NEStringKey&>(source)).getValue() );

				if(converted.getLength() > 0)
					_value = converted[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;
		case NEType::NEWSTRING_KEY:
			{
				const NEWString& extracted = (static_cast<const NEWStringKey&>(source)).getValue();

				if(extracted.getLength() > 0)
					_value = extracted[0];
				else
					return RESULT_TYPE_ERROR;
			}
			break;


		default:
			return RESULT_TYPE_ERROR;
		}

		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEKeyTemplate<type_short, NEType::NESHORT_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;

		SuperClass::assign(source);


		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			{
				NEString converter( (static_cast<const NECharKey&>(source)).getValue() );
				_value = static_cast<type_short>( converter.toInt() );
			}
			break;
		case NEType::NEBYTE_KEY:
			_value = (static_cast<const NEByteKey&>(source)).getValue();
			break;
		case NEType::NEUBYTE_KEY:
			_value = (static_cast<const NEUByteKey&>(source)).getValue();
			break;
		case NEType::NEWCHAR_KEY:
			{
				NEWString converter( (static_cast<const NEWCharKey&>(source)).getValue() );
				_value = static_cast<type_short>( converter.toInt() );
			}
			break;
		case NEType::NESHORT_KEY:
			_value = (static_cast<const NEShortKey&>(source)).getValue();
			break;
		case NEType::NEINT_KEY:
			_value = static_cast<type_short>((static_cast<const NEIntKey&>(source)).getValue());
			break;
		case NEType::NEUINT_KEY:
			_value = static_cast<type_short>((static_cast<const NEUIntKey&>(source)).getValue());
			break;
		case NEType::NEFLOAT_KEY:
			_value = static_cast<type_short>((static_cast<const NEFloatKey&>(source)).getValue());
			break;
		case NEType::NEDOUBLE_KEY:
			_value = static_cast<type_short>((static_cast<const NEDoubleKey&>(source)).getValue());
			break;
		case NEType::NEINT64_KEY:
			_value = static_cast<type_short>((static_cast<const NEInt64Key&>(source)).getValue());
			break;
		case NEType::NESTRING_KEY:
			_value = static_cast<type_short>( (static_cast<const NEStringKey&>(source)).getValue().toInt() );
			break;
		case NEType::NEWSTRING_KEY:
			_value = static_cast<type_short>( (static_cast<const NEWStringKey&>(source)).getValue().toInt() );
			break;
		
		default:
			return RESULT_TYPE_ERROR;
		}

		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEKeyTemplate<type_ushort, NEType::NEUSHORT_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;

		SuperClass::assign(source);


		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			{
				NEString converter( (static_cast<const NECharKey&>(source)).getValue() );
				_value = static_cast<type_ushort>( converter.toInt() );
			}
			break;
		case NEType::NEBYTE_KEY:
			_value = static_cast<type_ushort>((static_cast<const NEByteKey&>(source)).getValue());
			break;
		case NEType::NEUBYTE_KEY:
			_value = (static_cast<const NEUByteKey&>(source)).getValue();
			break;
		case NEType::NEWCHAR_KEY:
			{
				NEWString converter( (static_cast<const NEWCharKey&>(source)).getValue() );
				_value = static_cast<type_ushort>( converter.toInt() );
			}
			break;
		case NEType::NESHORT_KEY:
			_value = static_cast<type_ushort>((static_cast<const NEShortKey&>(source)).getValue());
			break;
		case NEType::NEUSHORT_KEY:
			_value = (static_cast<const NEUShortKey&>(source)).getValue();
			break;
		case NEType::NEINT_KEY:
			_value = static_cast<type_ushort>((static_cast<const NEIntKey&>(source)).getValue());
			break;
		case NEType::NEUINT_KEY:
			_value = static_cast<type_ushort>((static_cast<const NEUIntKey&>(source)).getValue());
			break;
		case NEType::NEFLOAT_KEY:
			_value = static_cast<type_ushort>((static_cast<const NEFloatKey&>(source)).getValue());
			break;
		case NEType::NEDOUBLE_KEY:
			_value = static_cast<type_ushort>((static_cast<const NEDoubleKey&>(source)).getValue());
			break;
		case NEType::NEINT64_KEY:
			_value = static_cast<type_ushort>((static_cast<const NEInt64Key&>(source)).getValue());
			break;
		case NEType::NESTRING_KEY:
			_value = static_cast<type_ushort>( (static_cast<const NEStringKey&>(source)).getValue().toInt() );
			break;
		case NEType::NEWSTRING_KEY:
			_value = static_cast<type_ushort>( (static_cast<const NEWStringKey&>(source)).getValue().toInt() );
			break;
		
		default:
			return RESULT_TYPE_ERROR;
		}

		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEKeyTemplate<type_int, NEType::NEINT_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;

		SuperClass::assign(source);


		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			{
				NEString converter( (static_cast<const NECharKey&>(source)).getValue() );
				_value = converter.toInt();
			}
			break;
		case NEType::NEBYTE_KEY:
			_value = (static_cast<const NEByteKey&>(source)).getValue();
			break;
		case NEType::NEUBYTE_KEY:
			_value = (static_cast<const NEUByteKey&>(source)).getValue();
			break;
		case NEType::NEWCHAR_KEY:
			{
				NEWString converter( (static_cast<const NEWCharKey&>(source)).getValue() );
				_value = converter.toInt();
			}
			break;
		case NEType::NESHORT_KEY:
			_value = (static_cast<const NEShortKey&>(source)).getValue();
			break;
		case NEType::NEUSHORT_KEY:
			_value = (static_cast<const NEUShortKey&>(source)).getValue();
			break;
		case NEType::NEINT_KEY:
			_value = (static_cast<const NEIntKey&>(source)).getValue();
			break;
		case NEType::NEUINT_KEY:
			_value = static_cast<type_int>((static_cast<const NEUIntKey&>(source)).getValue());
			break;
		case NEType::NEFLOAT_KEY:
			_value = static_cast<type_int>((static_cast<const NEFloatKey&>(source)).getValue());
			break;
		case NEType::NEDOUBLE_KEY:
			_value = static_cast<type_int>((static_cast<const NEDoubleKey&>(source)).getValue());
			break;
		case NEType::NEINT64_KEY:
			_value = static_cast<type_int>((static_cast<const NEInt64Key&>(source)).getValue());
			break;
		case NEType::NESTRING_KEY:
			_value = (static_cast<const NEStringKey&>(source)).getValue().toInt();
			break;
		case NEType::NEWSTRING_KEY:
			_value = (static_cast<const NEWStringKey&>(source)).getValue().toInt();
			break;
		
		default:
			return RESULT_TYPE_ERROR;
		}

		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEKeyTemplate<type_uint, NEType::NEUINT_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if(this == &source) return RESULT_TYPE_ERROR;

		SuperClass::assign(source);


		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			{
				NEString converter( (static_cast<const NECharKey&>(source)).getValue() );
				_value = static_cast<type_uint>(converter.toInt());
			}
			break;
		case NEType::NEBYTE_KEY:
			_value = static_cast<type_uint>((static_cast<const NEByteKey&>(source)).getValue());
			break;
		case NEType::NEUBYTE_KEY:
			_value = (static_cast<const NEUByteKey&>(source)).getValue();
			break;
		case NEType::NEWCHAR_KEY:
			{
				NEWString converter( (static_cast<const NEWCharKey&>(source)).getValue() );
				_value = static_cast<type_uint>(converter.toInt());
			}
			break;
		case NEType::NESHORT_KEY:
			_value = static_cast<type_uint>((static_cast<const NEShortKey&>(source)).getValue());
			break;
		case NEType::NEUSHORT_KEY:
			_value = (static_cast<const NEUShortKey&>(source)).getValue();
			break;
		case NEType::NEINT_KEY:
			_value = static_cast<type_uint>((static_cast<const NEIntKey&>(source)).getValue());
			break;
		case NEType::NEUINT_KEY:
			_value = (static_cast<const NEUIntKey&>(source)).getValue();
			break;
		case NEType::NEFLOAT_KEY:
			_value = static_cast<type_uint>((static_cast<const NEFloatKey&>(source)).getValue());
			break;
		case NEType::NEDOUBLE_KEY:
			_value = static_cast<type_uint>((static_cast<const NEDoubleKey&>(source)).getValue());
			break;
		case NEType::NEINT64_KEY:
			_value = static_cast<type_uint>((static_cast<const NEInt64Key&>(source)).getValue());
			break;
		case NEType::NESTRING_KEY:
			_value = (static_cast<const NEStringKey&>(source)).getValue().toInt();
			break;
		case NEType::NEWSTRING_KEY:
			_value = (static_cast<const NEWStringKey&>(source)).getValue().toInt();
			break;
		
		default:
			return RESULT_TYPE_ERROR;
		}

		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEKeyTemplate<type_float, NEType::NEFLOAT_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;

		SuperClass::assign(source);


		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			{
				NEString converter( (static_cast<const NECharKey&>(source)).getValue() );
				_value = static_cast<type_float>( converter.toDouble() );
			}
			break;
		case NEType::NEBYTE_KEY:
		    _value = (static_cast<const NEByteKey&>(source)).getValue();
			break;
		case NEType::NEUBYTE_KEY:
			_value = (static_cast<const NEUByteKey&>(source)).getValue();
			break;
		case NEType::NEWCHAR_KEY:
			{
				NEWString converter( (static_cast<const NEWCharKey&>(source)).getValue() );
				_value = static_cast<type_float>( converter.toDouble() );
			}
			break;
		case NEType::NESHORT_KEY:
			_value = (static_cast<const NEShortKey&>(source)).getValue();
			break;
		case NEType::NEUSHORT_KEY:
			_value = (static_cast<const NEUShortKey&>(source)).getValue();
			break;
		case NEType::NEINT_KEY:
			_value = static_cast<type_float>((static_cast<const NEIntKey&>(source)).getValue());
			break;
		case NEType::NEUINT_KEY:
			_value = static_cast<type_float>((static_cast<const NEUIntKey&>(source)).getValue());
			break;
		case NEType::NEFLOAT_KEY:
			_value = (static_cast<const NEFloatKey&>(source)).getValue();
			break;
		case NEType::NEINT64_KEY:
			_value = static_cast<type_float>((static_cast<const NEInt64Key&>(source)).getValue());
			break;
		case NEType::NEDOUBLE_KEY:
			_value = static_cast<type_float>((static_cast<const NEDoubleKey&>(source)).getValue());
			break;
		case NEType::NESTRING_KEY:
			_value = static_cast<type_float>((static_cast<const NEStringKey&>(source)).getValue().toDouble());
			break;
		case NEType::NEWSTRING_KEY:
			_value = static_cast<type_float>((static_cast<const NEWStringKey&>(source)).getValue().toDouble());
			break;
		
		default:
			return RESULT_TYPE_ERROR;
		}

		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEKeyTemplate<type_double, NEType::NEDOUBLE_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;

		SuperClass::assign(source);


		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			{
				NEString converter( (static_cast<const NECharKey&>(source)).getValue() );
				_value = converter.toDouble();
			}
			break;
		case NEType::NEBYTE_KEY:
			_value = (static_cast<const NEByteKey&>(source)).getValue();
			break;
		case NEType::NEUBYTE_KEY:
			_value = (static_cast<const NEUByteKey&>(source)).getValue();
			break;
		case NEType::NEWCHAR_KEY:
			{
				NEWString converter( (static_cast<const NEWCharKey&>(source)).getValue() );
				_value = converter.toDouble();
			}
			break;
		case NEType::NESHORT_KEY:
			_value = (static_cast<const NEShortKey&>(source)).getValue();
			break;
		case NEType::NEUSHORT_KEY:
			_value = (static_cast<const NEUShortKey&>(source)).getValue();
			break;
		case NEType::NEINT_KEY:
			_value = (static_cast<const NEIntKey&>(source)).getValue();
			break;
		case NEType::NEUINT_KEY:
			_value = (static_cast<const NEUIntKey&>(source)).getValue();
			break;
		case NEType::NEFLOAT_KEY:
			_value = (static_cast<const NEFloatKey&>(source)).getValue();
			break;
		case NEType::NEDOUBLE_KEY:
			_value = (static_cast<const NEDoubleKey&>(source)).getValue();
			break;
		case NEType::NEINT64_KEY:
			_value = static_cast<type_double>((static_cast<const NEInt64Key&>(source)).getValue());
			break;
		case NEType::NESTRING_KEY:
			_value = (static_cast<const NEStringKey&>(source)).getValue().toDouble();
			break;
		case NEType::NEWSTRING_KEY:
			_value = (static_cast<const NEWStringKey&>(source)).getValue().toDouble();
			break;
		
		default:
			return RESULT_TYPE_ERROR;
		}

		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEKeyTemplate<type_int64, NEType::NEINT64_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;

		SuperClass::assign(source);


		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			{
				NEString converter( (static_cast<const NECharKey&>(source)).getValue() );
				_value = converter.toInt();
			}
			break;
		case NEType::NEBYTE_KEY:
			_value = (static_cast<const NEByteKey&>(source)).getValue();
			break;
		case NEType::NEUBYTE_KEY:
			_value = (static_cast<const NEUByteKey&>(source)).getValue();
			break;
		case NEType::NEWCHAR_KEY:
			{
				NEWString converter( (static_cast<const NEWCharKey&>(source)).getValue() );
				_value = converter.toInt();
			}
			break;
		case NEType::NESHORT_KEY:
			_value = (static_cast<const NEShortKey&>(source)).getValue();
			break;
		case NEType::NEUSHORT_KEY:
			_value = (static_cast<const NEUShortKey&>(source)).getValue();
			break;
		case NEType::NEINT_KEY:
			_value = (static_cast<const NEIntKey&>(source)).getValue();
			break;
		case NEType::NEUINT_KEY:
			_value = (static_cast<const NEUIntKey&>(source)).getValue();
			break;
		case NEType::NEFLOAT_KEY:
			_value = static_cast<type_int64>( (static_cast<const NEFloatKey&>(source)).getValue() );
			break;
		case NEType::NEDOUBLE_KEY:
			_value = static_cast<type_int64>( (static_cast<const NEDoubleKey&>(source)).getValue() );
			break;
		case NEType::NEINT64_KEY:
			_value = (static_cast<const NEInt64Key&>(source)).getValue();
			break;
		case NEType::NESTRING_KEY:
			_value = (static_cast<const NEStringKey&>(source)).getValue().toInt();
			break;
		case NEType::NEWSTRING_KEY:
			_value = (static_cast<const NEWStringKey&>(source)).getValue().toInt();
			break;
		
		default:
			return RESULT_TYPE_ERROR;
		}

		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEKeyTemplate<NEString, NEType::NESTRING_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;


		SuperClass::assign(source);

		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			_value = (static_cast<const NECharKey&>(source)).getValue();
			break;
		case NEType::NEBYTE_KEY:
			_value = static_cast<type_int>((static_cast<const NEByteKey&>(source)).getValue());
			break;
		case NEType::NEUBYTE_KEY:
			_value = static_cast<type_int>((static_cast<const NEUByteKey&>(source)).getValue());
			break;
		case NEType::NEWCHAR_KEY:
			_value = (static_cast<const NEWCharKey&>(source)).getValue();
			break;
		case NEType::NESHORT_KEY:			
			_value = static_cast<type_int>((static_cast<const NEShortKey&>(source)).getValue());
			break;
		case NEType::NEUSHORT_KEY:
			_value = (static_cast<const NEUShortKey&>(source)).getValue();;
			break;
		case NEType::NEINT_KEY:
			_value = (static_cast<const NEIntKey&>(source)).getValue();
			break;
		case NEType::NEUINT_KEY:
			_value = static_cast<type_int>((static_cast<const NEUIntKey&>(source)).getValue());
			break;
		case NEType::NEFLOAT_KEY:
			_value = (static_cast<const NEFloatKey&>(source)).getValue();
			break;
		case NEType::NEDOUBLE_KEY:
			_value = (static_cast<const NEDoubleKey&>(source)).getValue();
			break;
		case NEType::NEINT64_KEY:
			_value = static_cast<type_int>((static_cast<const NEInt64Key&>(source)).getValue());
			break;
		case NEType::NESTRING_KEY:
			_value = (static_cast<const NEStringKey&>(source)).getValue();
			break;
		case NEType::NEWSTRING_KEY:
			_value = (static_cast<const NEWStringKey&>(source)).getValue();
			break;
		
		default:
			return RESULT_TYPE_ERROR;
		}

		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEKeyTemplate<NEWString, NEType::NEWSTRING_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;


		SuperClass::assign(source);

		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			_value = (static_cast<const NECharKey&>(source)).getValue();
			break;
		case NEType::NEBYTE_KEY:
			_value = static_cast<type_int>((static_cast<const NEByteKey&>(source)).getValue());
			break;
		case NEType::NEUBYTE_KEY:
			_value = static_cast<type_int>((static_cast<const NEUByteKey&>(source)).getValue());
			break;
		case NEType::NEWCHAR_KEY:
			_value = (static_cast<const NEWCharKey&>(source)).getValue();
			break;
		case NEType::NESHORT_KEY:
			_value = (static_cast<const NEShortKey&>(source)).getValue();
			break;
		case NEType::NEUSHORT_KEY:
			_value = static_cast<type_int>((static_cast<const NEUShortKey&>(source)).getValue());
			break;
		case NEType::NEINT_KEY:
			_value = (static_cast<const NEIntKey&>(source)).getValue();
			break;
		case NEType::NEUINT_KEY:
			_value = static_cast<type_int>((static_cast<const NEUIntKey&>(source)).getValue());
			break;
		case NEType::NEFLOAT_KEY:			
			_value = (static_cast<const NEFloatKey&>(source)).getValue();
			break;
		case NEType::NEDOUBLE_KEY:
			_value = (static_cast<const NEDoubleKey&>(source)).getValue();
			break;
		case NEType::NEINT64_KEY:
			_value = static_cast<type_int>((static_cast<const NEInt64Key&>(source)).getValue());
			break;
		case NEType::NESTRING_KEY:
			_value = (static_cast<const NEStringKey&>(source)).getValue();
			break;
		case NEType::NEWSTRING_KEY:
			_value = (static_cast<const NEWStringKey&>(source)).getValue();
			break;
		
		default:
			return RESULT_TYPE_ERROR;
		}

		return RESULT_SUCCESS;
	}
	




	template <>
	bool NE_DLL NEKeyTemplate<NEString, NEType::NESTRING_KEY>::isLower(const NEKey& source) const
	{
		//	pre:
		if( ! &source) return false;



		//	main:
		//		버퍼 정의:
		NEKeyTemplate<NEString, NEType::NESTRING_KEY> buffer;
		//		버퍼에 할당:
		type_result result = buffer.assign(source);
		if(NEResult::hasError(result))
			return false;
		


		//	post:
		//		버퍼 비교:
		//			타겟팅:
		const NEString&	source_string = getValue(),
					  &	target_string = buffer.getValue();		
		//			사전 예외처리:
		if(target_string.getLength() <= 0) return false;
		type_index length_of_source = getValue().getLength();
		//			루핑:
		for(int n=0; n < length_of_source ;n++)
		{
			//			예외처리:
			/*
							비교대상(= 타겟)이 길이가 작을경우에는, 탐색을 중단하고 이전
							까지의 결과가 true였으므로 "true"를 반환한다.

							그러나 타겟이 길이가 0이라면, for루프 전에서 사전 예외처리를 
							통해 false가 반환될 것이다.
			*/
			if(n > target_string.getLengthLastIndex()) break;

			//			문자 비교:
			//				문자 추출:
			char	source_character = source_string[n],
					target_character = target_string[n];
			//				비교:
			/*
								정상적으로 추출한 두개의 문자를 비교해서 "원본 >= 비교대상"이 
								되는, 순간에는 false를 반환한다.
			*/
			if(source_character >= target_character)
				return false;
		}

		return true;
	}
	template <>
	bool NE_DLL NEKeyTemplate<NEWString, NEType::NEWSTRING_KEY>::isLower(const NEKey& source) const
	{
//	pre:
		if( ! &source) return false;



		//	main:
		//		버퍼 정의:
		NEKeyTemplate<NEWString, NEType::NEWSTRING_KEY> buffer;
		//		버퍼에 할당:
		type_result result = buffer.assign(source);
		if(NEResult::hasError(result))
			return false;
		


		//	post:
		//		버퍼 비교:
		//			타겟팅:
		const NEWString&	source_string = getValue(),
					   &	target_string = buffer.getValue();		
		//			사전 예외처리:
		if(target_string.getLength() <= 0) return false;
		type_index length_of_source = getValue().getLength();
		//			루핑:
		for(int n=0; n < length_of_source ;n++)
		{
			//			예외처리:
			/*
							비교대상(= 타겟)이 길이가 작을경우에는, 탐색을 중단하고 이전
							까지의 결과가 true였으므로 "true"를 반환한다.

							그러나 타겟이 길이가 0이라면, for루프 전에서 사전 예외처리를 
							통해 false가 반환될 것이다.
			*/
			if(n > target_string.getLengthLastIndex()) break;

			//			문자 비교:
			//				문자 추출:
			wchar_t	source_character = source_string[n],
					target_character = target_string[n];
			//				비교:
			/*
								정상적으로 추출한 두개의 문자를 비교해서 "원본 >= 비교대상"이 
								되는, 순간에는 false를 반환한다.
			*/
			if(source_character >= target_character)
				return false;
		}

		return true;
	}

	template <>
	bool NEKeyTemplate<NEBooleanSet, NEType::NEBOOLEAN_SET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
	template <>
	bool NEKeyTemplate<NEByteSet, NEType::NEBYTE_SET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
	template <>
	bool NEKeyTemplate<NEUByteSet, NEType::NEUBYTE_SET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
	template <>
	bool NEKeyTemplate<NEShortSet, NEType::NESHORT_SET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
	template <>
	bool NEKeyTemplate<NEUShortSet, NEType::NEUSHORT_SET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
	template <>
	bool NEKeyTemplate<NEIntSet, NEType::NEINT_SET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
	template <>
	bool NEKeyTemplate<NEUIntSet, NEType::NEUINT_SET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
	template <>
	bool NEKeyTemplate<NEInt64Set, NEType::NEINT64_SET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
	template <>
	bool NEKeyTemplate<NEFloatSet, NEType::NEFLOAT_SET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
	template <>
	bool NEKeyTemplate<NEDoubleSet, NEType::NEDOUBLE_SET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
	template <>
	bool NEKeyTemplate<NEStringSet, NEType::NESTRING_SET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
	template <>
	bool NEKeyTemplate<NEWStringSet, NEType::NEWSTRING_SET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
	template <>
	bool NEKeyTemplate<NEModuleCodeSet, NEType::NEMODULE_CODESET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
	template <>
	bool NEKeyTemplate<NENodeCodeSet, NEType::NENODE_CODESET_KEY>::isLower(const NEKey& source) const
	{
		return false;
	}
}
#pragma warning(pop)