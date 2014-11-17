#pragma once

#include "../../Includes/Includes.hpp"
#include <windows.h>

namespace NE
{
	class NE_DLL WND : public NEModule
	{
	public:
		typedef NEModule SuperClass;
		typedef WND ThisClass;

	public:
		NEITArgument<NEIntKey> arg_x;
		NEITArgument<NEIntKey> arg_y;
		NEITArgument<NEIntKey> arg_height;
		NEITArgument<NEIntKey> arg_width;
		NEITArgument<NEIntKey> arg_how_to_show_window;

	public:
		WND();
		WND(const WND& source);

	public:
		virtual ~WND();

	public:
		virtual type_result initialize();

	public:
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);

	public:
		HWND getHWND() const;
		HWND getParentHWND() const;
		type_result changeDomain(HWND insert_after, int x, int y, int width, int height, UINT flag);
		type_result changeShowCommand(int style);
		type_result changeText(const NETString& new_text);
		type_result destroy();

	protected:
		virtual type_result _onArgumentsFetched(NEArgumentInterfaceList& tray);

	protected:
		bool _hasSavedStatesAssigned();
		
	private:
		void _release();
		const WND& _assign(const WND& source);
		

		//	멤버변수:
	protected:
		HWND _hwnd;
		HWND _parent_hwnd;
		int _saved_x;
		int _saved_y;
		int _saved_width;
		int _saved_height;
		int _saved_command_show;

	protected:
		static LRESULT CALLBACK _wndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	
	protected:
		static MSG _message_from_wnd_proc;
	};
}