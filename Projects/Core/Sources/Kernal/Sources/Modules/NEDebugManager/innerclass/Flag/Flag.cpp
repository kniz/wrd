#ifdef DEVELOPER

#include "../../NEDebugManager.hpp"
#include "../../define/define.hpp"

namespace NE
{
	NE_DLL NEDebugManager::Flag::Flag()
		: Super()
	{
		release();
	}

	NE_DLL NEDebugManager::Flag::Flag(const TCHAR& source)
		: Super()
	{
		_flag = source;
	}

	NE_DLL NEDebugManager::Flag::Flag(const This& source)
		: Super(source)
	{
		_flag = source._flag;
	}

	NE_DLL NEDebugManager::Flag::~Flag()
	{
		
	}
	
	const NEDebugManager::Flag NE_DLL &NEDebugManager::Flag::operator=(const This& source)
	{
		return _assign(source);
	}

	type_bool NE_DLL NEDebugManager::Flag::operator==(const This& source) const
	{
		if(this == &source) return true;
		if(getFlag() != source.getFlag()) return false;

		return true;
	}

	type_bool NE_DLL NEDebugManager::Flag::operator!=(const This& source) const
	{
		return !(operator==(source));
	}
	
	TCHAR NE_DLL NEDebugManager::Flag::getFlag() const
	{
		return _flag;
	}

	void NE_DLL NEDebugManager::Flag::setFlag(TCHAR flag)
	{
		_flag = flag;
	}

	type_bool NE_DLL NEDebugManager::Flag::isLogShown() const
	{
		return (_flag & _LOG) != 0;
	}

	void NE_DLL NEDebugManager::Flag::setLogShown(type_bool enable)
	{
		if(enable)
			_flag = _flag | _LOG;
		else
			_flag = _flag & (~_LOG);
	}

	type_bool NE_DLL NEDebugManager::Flag::isConsoleShown() const
	{
		return (_flag & _CONSOLE) != 0;
	}

	void NE_DLL NEDebugManager::Flag::setConsoleShown(type_bool enable)
	{
		if(enable)
			_flag = _flag | _CONSOLE;
		else
			_flag = _flag & (~_CONSOLE);
	}

	type_bool NE_DLL NEDebugManager::Flag::isMessageBoxShown() const
	{
		return (_flag & _MESSAGEBOX) != 0;
	}

	void NE_DLL NEDebugManager::Flag::setMessageBoxShown(type_bool enable)
	{
		if(enable)
			_flag = _flag | _MESSAGEBOX;
		else
			_flag = _flag & (~_MESSAGEBOX);
	}

	type_bool NE_DLL NEDebugManager::Flag::isDebugWindowShown() const
	{
		return (_flag & _DEBUGWINDOW) != 0;
	}

	void NE_DLL NEDebugManager::Flag::setDebugWindowShown(type_bool enable)
	{
		if(enable)
			_flag = _flag | _DEBUGWINDOW;
		else
			_flag = _flag & (~_DEBUGWINDOW);
	}

	void NE_DLL NEDebugManager::Flag::release()
	{
		_flag = _DEFAULT_MODULE_FLAG;
	}

	type_result NE_DLL NEDebugManager::Flag::isValid() const
	{
		if(_flag < 0 || _flag > (_MESSAGEBOX | _CONSOLE | _DEBUGWINDOW | _LOG)) return RESULT_TYPE_ERROR;

		return RESULT_SUCCESS;
	}

	NEBinaryFileSaver NE_DLL &NEDebugManager::Flag::serialize( NEBinaryFileSaver& saver) const
	{
		return saver << _flag;
	}

	NEBinaryFileLoader NE_DLL &NEDebugManager::Flag::serialize(NEBinaryFileLoader& loader)
	{
		return loader >> _flag;
	}

	const NEDebugManager::Flag& NEDebugManager::Flag::_assign(const This& source)
	{
		if(this == &source) return *this;
		
		setFlag(source.getFlag());
		
		return *this;
	}
}

#endif
