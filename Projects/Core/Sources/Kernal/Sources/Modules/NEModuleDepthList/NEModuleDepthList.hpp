#pragma once

#include "../NETList/NETList.hpp"
#include "../../Commons/Units/NEModuleDepthUnit/NEModuleDepthUnit.hpp"

namespace NE
{
	template
	class NE_DLL NETList<NEModuleDepthUnit, false>;
	typedef NETList<NEModuleDepthUnit, false> NEModuleDepthList;
}