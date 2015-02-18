/**
 * @file	WindowController.h
 * @date	2015-01-03
 * @author	Emile Bergeron <contact+source@prismalstudio.com>
 * @brief	
 */

#ifndef WINDOWCONTROLLER_H_
#define WINDOWCONTROLLER_H_

#include "Util/TSingleton.h"

#include "WindowParam.h"
//#include <windef.h>
#include <gdiplus.h>
namespace xover
{

class WindowController: public Util::TSingleton<WindowController>
{
		friend class Util::TSingleton<WindowController>;

	public:

		bool registerInstance(HINSTANCE hInstance);
		bool create(int nCmdShow);
		int messageLoop();
		void draw(HDC hdc);

		//void onCreate(HWND hwnd);

		//void onDestroy(HWND hwnd);

		bool setImage(PCWSTR pszFileName);

		// GETTERS SETTERS
		void setWindowPosition(HWND hwnd, POINT p, bool centered = false);

		const WindowParam& getParam() const;
		void setParam(const WindowParam& param);

		bool isChangingPosition() const;
		void setChangingPosition(bool changingPosition);

	private:
		// functions
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam,
				LPARAM lParam);
		// Window Messages
		void onCreate(HWND hwnd);
		void onChar(HWND hwnd, wchar_t c);
		void onPaint(HWND hwnd);
		void onSize(HWND hwnd);
		void onKillFocus(HWND hwnd);
		void onFileOpen(HWND hwnd);
		void onDestroy();

		void NotifyError(HWND hwnd, LPCSTR pszMessage);

		// singleton
		WindowController();
		~WindowController();

	private:
		// members
		HINSTANCE mHInstance; // application instance
		HWND mHwnd; // main window handle
		Gdiplus::GdiplusStartupInput mGdiplusStartupInput;
		ULONG_PTR mGdiplusToken;

		//HBRUSH mBackground;

		WindowParam mParam;
		bool mChangingPosition;
		//HBITMAP mBitmap;
		Gdiplus::Image * mImage;
};

} /* namespace xover */

#endif /* WINDOWCONTROLLER_H_ */
