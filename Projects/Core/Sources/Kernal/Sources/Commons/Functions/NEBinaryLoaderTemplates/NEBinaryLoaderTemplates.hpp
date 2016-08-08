//	---------------------------------------------------------------------------------
//	파일명	:	NEBinaryLoaderTemplates.hpp
//	설명	:	NEBinaryFileLoader의 operator >>를 오버로딩해서, 데이터 저장을 호출한다.
//	메모	:	Kernal 프로젝트에서 사용된다.
//	히스토리:	2011-06-04	이태훈	개발 완료
//	---------------------------------------------------------------------------------
#pragma once

//	include:
#include "../../../Includes/Includes.hpp"

namespace NE
{
	//	객체와 Primitive데이터와 동일한 인터페이스로 처리하기 위해서 operator>>를 스트림 전용 연산자로 지정함
	//	전역함수(global 함수)
	template <typename T>
	NEBinaryFileLoader& operator>>(NEBinaryFileLoader& loader, T& data)
	{
		data.serialize(loader);

		return loader;
	}	
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_char& data);
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_wchar& data);
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_bool& data);
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_short& data);
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_int& data);
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_int64& data);	
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_float& data);
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_double& data);
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_uchar& data);	
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_ushort& data);
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_uint& data);
	template <>
	NEBinaryFileLoader NE_DLL &operator>>(NEBinaryFileLoader& loader, type_uint64& data);
	//	NETList나 Array에 *가 들어온 경우는 아무런 작업도 하지 않는다.
	//	사용자가 직접 함수를 오버라이딩해서 처리해야함.
	//	만약 T*가 primitive 변수의 포인터일 경우는 에러가 발생한다.
}
