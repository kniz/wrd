#include "NEBinaryLoaderTemplates.hpp"

namespace NE
{
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_bool& data)
	{
		loader.execute(reinterpret_cast<char*>(&data), sizeof(data));

		return loader;
	}
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_char& data)
	{
		loader.execute(reinterpret_cast<char*>(&data), sizeof(data));

		return loader;
	}
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_wchar& data)
	{
		loader.execute(reinterpret_cast<char*>(&data), sizeof(data));

		return loader;
	}	
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_short& data)
	{
		loader.execute(reinterpret_cast<char*>(&data), sizeof(data));

		return loader;
	}
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_int& data)
	{
		loader.execute(reinterpret_cast<char*>(&data), sizeof(data));

		return loader;
	}
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_uint64& data)
	{
		loader.execute(reinterpret_cast<char*>(&data), sizeof(data));

		return loader;
	}
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_float& data)
	{
		loader.execute(reinterpret_cast<char*>(&data), sizeof(data));

		return loader;
	}
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_double& data)
	{
		loader.execute(reinterpret_cast<char*>(&data), sizeof(data));

		return loader;
	}
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_uchar& data)
	{
		loader.execute(reinterpret_cast<char*>(&data), sizeof(data));

		return loader;
	}
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_ushort& data)
	{
		loader.execute(reinterpret_cast<char*>(&data), sizeof(data));

		return loader;
	}
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_uint& data)
	{
		loader.execute(reinterpret_cast<char*>(&data), sizeof(data));

		return loader;
	}
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_uint64& data)
	{
		loader.execute(reinterpret_cast<char*>(&data), sizeof(data));

		return loader;
	}	
}
