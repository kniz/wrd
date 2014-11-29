#pragma once

#include "../WND.hpp"

namespace NE
{
	class NE_DLL Window : public WND
	{
	public:
		typedef WND SuperClass;
		typedef Window ThisClass;

	public:
		friend class WindowPanel;

	public:
#include "innerclass/ErrorCode/ErrorCode.hpp"
		NETArgument<NETStringKey> arg_title;

	public:
		Window();
		Window(const Window& source);

	public:
		virtual ~Window();

	public:
		virtual type_result initialize();

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const;
		virtual LPCTSTR getErrorMessage(type_errorcode errorcode) const;

	public:
		virtual NEObject& clone() const;
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual void release();

	public:
		const MSG& getMSG() const;
		const WNDCLASS& getWndClass() const;
		DWORD getStyle() const;
		/*
			change와 set의 차이. 
				set은 데이터 자체의 변경.
				change는 데이터가 변경뿐만 아니라, 그걸 적용.
		*/
		type_result changeTitle(const NETString& title);		

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray);
		virtual type_result _onExecute();

	protected:
		type_result _initializeWindow();
		type_result _registerWindowClass();
		type_result _createWindow();		
		type_result _initializeTitle();
		void _storeMyMessageFromWinProc();

	private:
		void _release();
		const Window& _assign(const Window& source);
		

		//	멤버변수:
		/*
			다음 변수들은 외부에 공개될 수 없으므로 키로 선언하지 않는다.
			그러나, 패널에 의해서는 변경될 여지가 존재하기 때문에 상단에 WindowPanel을
			friend로 선언했다.			
		*/
	protected:
		/*
			스크립트에서 에디트된, 처음 생성될때 사용되는 변수들이다.
			프로세스 도중 changeRect등의 변경자함수가 호출될때 다음의 변수들은
			전혀 영향을 받을 필요가 없다. 
			이미 윈도우가 생성되었다면 역할을 다 한셈.
		*/
		MSG _message;
		WNDCLASS _class;
		DWORD _style;
		/*
			실제로 할당되어있는 윈도우의 상태변수.
			외부에 입력되는 getKeySet() 데이터와 비교하여, 차이가 발생하는 경우는 자동으로
			윈도우와 동기화시킨다.
		*/
		NETString _saved_title;
	};
}