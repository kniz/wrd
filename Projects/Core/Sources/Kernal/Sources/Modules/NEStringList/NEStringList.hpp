#pragma once

#include "../NEString/NEString.hpp"
#include "../NETList/NETList.hpp"

namespace NE
{
	template
	class NE_DLL NETList<NEString, false>;
	typedef NETList<NEString, false> NEStringList;
}
