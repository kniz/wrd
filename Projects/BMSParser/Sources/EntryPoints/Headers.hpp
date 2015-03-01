#pragma once

#include "../Modules/BMSParser.hpp"

#ifndef BMSPARSER_EXPORTS
#	ifdef DEVELOPER
#		ifdef _DEBUG
#			ifdef _UNICODE
#				pragma comment(lib, "BMSParser.dev.dbg.lib")
#			else
#				pragma comment(lib, "BMSParser.dev.dbg.mbcs.lib")
#			endif
#		else
#			ifdef _UNICODE
#				pragma comment(lib, "BMSParser.dev.lib")
#			else
#				pragma comment(lib, "BMSParser.dev.mbcs.lib")
#			endif
#		endif
#	else
#		ifdef _UNICODE
#			pragma comment(lib, "BMSParser.lib")
#		else
#			pragma comment(lib, "BMSParser.mbcs.lib")
#		endif
#	endif
#endif
