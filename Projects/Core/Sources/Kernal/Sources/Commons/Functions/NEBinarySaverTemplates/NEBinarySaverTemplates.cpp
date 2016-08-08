#include "NEBinarySaverTemplates.hpp"

namespace NE
{
	template <>
	NEBinaryFileSaver NE_DLL &operator<<(NEBinaryFileSaver& saver, const type_char& data)
	{
		saver.execute(reinterpret_cast<const char* const>(&data), sizeof(data));

		return saver;
	}
	template <>
	NEBinaryFileSaver NE_DLL &operator<<(NEBinaryFileSaver& saver, const type_wchar& data)
	{
		saver.execute(reinterpret_cast<const char* const>(&data), sizeof(data));

		return saver;
	}
	template <>
	NEBinaryFileSaver NE_DLL &operator<<(NEBinaryFileSaver& saver, const type_bool& data)
	{
		saver.execute(reinterpret_cast<const char* const>(&data), sizeof(data));

		return saver;
	}
	template <>
	NEBinaryFileSaver NE_DLL &operator<<(NEBinaryFileSaver& saver, const type_short& data)
	{
		saver.execute(reinterpret_cast<const char* const>(&data), sizeof(data));

		return saver;
	}	
	template <>
	NEBinaryFileSaver NE_DLL &operator<<(NEBinaryFileSaver& saver, const type_int& data)
	{
		saver.execute(reinterpret_cast<const char* const>(&data), sizeof(data));

		return saver;
	}
	template <>
	NEBinaryFileSaver NE_DLL &operator<<(NEBinaryFileSaver& saver, const type_int64& data)
	{
		saver.execute(reinterpret_cast<const char* const>(&data), sizeof(data));

		return saver;
	}
	template <>
	NEBinaryFileSaver NE_DLL &operator<<(NEBinaryFileSaver& saver, const type_float& data)
	{
		saver.execute(reinterpret_cast<const char* const>(&data), sizeof(data));

		return saver;
	}
	template <>
	NEBinaryFileSaver NE_DLL &operator<<(NEBinaryFileSaver& saver, const type_double& data)
	{
		saver.execute(reinterpret_cast<const char* const>(&data), sizeof(data));

		return saver;
	}
	template <>
	NEBinaryFileSaver NE_DLL &operator<<(NEBinaryFileSaver& saver, const type_uchar& data)
	{
		saver.execute(reinterpret_cast<const char* const>(&data), sizeof(data));

		return saver;
	}
	template <>
	NEBinaryFileSaver NE_DLL &operator<<(NEBinaryFileSaver& saver, const type_ushort& data)
	{
		saver.execute(reinterpret_cast<const char* const>(&data), sizeof(data));

		return saver;
	}	
	template <>
	NEBinaryFileSaver NE_DLL &operator<<(NEBinaryFileSaver& saver, const type_uint& data)
	{
		saver.execute(reinterpret_cast<const char* const>(&data), sizeof(data));

		return saver;
	}
	template <>
	NEBinaryFileSaver NE_DLL &operator<<(NEBinaryFileSaver& saver, const type_uint64& data)
	{
		saver.execute(reinterpret_cast<const char* const>(&data), sizeof(data));

		return saver;
	}	
}
