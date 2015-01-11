/*
 * @file	WindowController.cpp
 * @date	2015-01-03
 * @author	Emile
 * @brief	
 */
#define _WIN32_WINNT 0x0501 // needed for UpdateLayeredWindow and its flags
#include "WindowController.h"

#include "WindowParam.h"

#include <windows.h>
//#include <gdiplus.h>
//#include <Winuser.h>

#include <iostream>

#define WINDOW_CLASS TEXT("CrosshairOverlayApp")

// debug using cout
using std::cout;
using std::cerr;
using std::endl;

namespace xover
{

/**
 * Static function callback to handle messages
 */
LRESULT CALLBACK WindowController::WndProc(HWND hwnd, UINT msg, WPARAM wParam,
		LPARAM lParam)
{
	//cout << "WindowController::WndProc : " << msg << endl;

//	WindowController* mainApp = (WindowController *) GetWindowLong(hwnd,
//	GWL_USERDATA);

//LPCREATESTRUCT pcs;
	switch (msg)
	/* handle the messages */
	{
//	case WM_NCCREATE:
//		pcs = (LPCREATESTRUCT) lParam;
//		mainApp = (WindowController*) pcs->lpCreateParams;
//		SetWindowLong(hwnd, GWL_USERDATA, (LONG) mainApp);
//		break;
	case WM_CREATE:
		//mainApp->onCreate(hwnd);
		WindowController::getInstance()->onCreate(hwnd);
		PostMessage(hwnd, WM_PAINT, 0, 0);
		break;
	case WM_DESTROY:
		//mainApp->onDestroy();
		WindowController::getInstance()->onDestroy();
		break;
	case WM_PAINT:
		//mainApp->onPaint(hwnd);
		WindowController::getInstance()->onPaint(hwnd);
		break;
	case WM_CHAR:
		//mainApp->onChar(hwnd, (wchar_t) wParam);
		WindowController::getInstance()->onChar(hwnd, (wchar_t) wParam);
		break;
	case WM_KILLFOCUS:
		//mainApp->onKillFocus(hwnd);
		WindowController::getInstance()->onKillFocus(hwnd);
		break;
//	case WM_SIZE:
//		WindowController::getInstance()->onSize(hwnd);
//		break;
	default: /* for messages that we don't deal with */
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

WindowController::WindowController() :
				mHInstance(NULL),
				mHwnd(NULL),
				//mBackground(NULL),
				//mParam(WindowParam()),
				mChangingPosition(false),
				//mBitmap(NULL),
				mImage(NULL)
{
	Gdiplus::GdiplusStartup(&mGdiplusToken, &mGdiplusStartupInput, NULL);
}

void WindowController::onSize(HWND hwnd)
{
	cout << "WindowController::onSize" << endl;

	if (mImage)
	{
		// adjust the params.
		mParam.setSize(mImage->GetWidth(), mImage->GetHeight());
	}
	else
	{
		// reset the params.
		mParam.setSize();

	}

	SetWindowPos(mHwnd, /* handle */
	HWND_TOPMOST, /* just in case, but useless with the flag SWP_NOZORDER */
	0, /* x pos, useless with flag SWP_NOMOVE */
	0, /* y pos, useless with flag SWP_NOMOVE */
	(int) mParam.getWidth(), /* new width */
	(int) mParam.getHeight(), /* new height  */
	SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE /* flags */
	);

}

WindowController::~WindowController()
{
	//DeleteObject(mBitmap);
	//DeleteObject (mBackground);
	//delete mImage;
	Gdiplus::GdiplusShutdown(mGdiplusToken);
	onDestroy();

}

void WindowController::onDestroy()
{
	//SetWindowLong(mHwnd, GWL_USERDATA, (long) 0);
	PostQuitMessage(0); /* send a WM_QUIT to the message queue */
}

bool WindowController::registerInstance(HINSTANCE hInstance)
{
	WNDCLASSEX wincl; /* Data structure for the windowclass */
	/* The Window structure */
	wincl.hInstance = hInstance;
	wincl.lpszClassName = WINDOW_CLASS;
	wincl.lpfnWndProc = WndProc; /* This function is called by windows */
	wincl.style = CS_DBLCLKS; /* Catch double-clicks */
	wincl.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL; /* No menu */
	wincl.cbClsExtra = 0; /* No extra bytes after the window class */
	wincl.cbWndExtra = 0; /* structure or the window instance */
	/* Use Windows's default color as the background of the window */
	//wincl.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1); //COLOR_BACKGROUND;
	HBRUSH background = CreateSolidBrush(mParam.getTransparencyMaskColor());
	wincl.hbrBackground = background;
	DeleteObject(background);

	/* Register the window class, and if it fails quit the program */
	if (!RegisterClassEx(&wincl))
	{
		return false;
	}

	mHInstance = hInstance;
	return true;
}

bool WindowController::create(int nCmdShow)
{
	cout << "WindowController::create" << endl;
	/* The class is registered, let's create the program*/
	mHwnd =
	CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT, /* Extended possibilites for variation */
	WINDOW_CLASS, /* Classname */
	"Crosshair Overlay", /* Title Text */
	WS_POPUP | WS_VISIBLE, /* default window   WS_OVERLAPPEDWINDOW  */
	mParam.getX(), /* Windows decides the position */
	mParam.getY(), /* where the window ends up on the screen */
	mParam.getWidth(), /* The programs width */
	mParam.getHeight(), /* and height in pixels */
	HWND_DESKTOP, /* The window is a child-window to desktop */
	NULL, /* No menu */
	mHInstance, /* Program Instance handler */
	NULL /* No Window Creation data */
	);
	if (!mHwnd)
	{
		return false;
	}

//	SetLayeredWindowAttributes(mHwnd, /* handle */
//	mParam.getTransparencyMaskColor(), /* transparent mask color */
//	mParam.getOpacity(), /* opacity 0 to 255 */
//	LWA_ALPHA | LWA_COLORKEY); /* LWA_ALPHA, LWA_COLORKEY */

	/* Make the window visible on the screen */
	//SetWindowLong(mHwnd, GWL_EXSTYLE, WS_EX_LAYERED);
	ShowWindow(mHwnd, nCmdShow);
	//UpdateWindow(mHwnd);

	// DEFAULT TEST
	//mParam.setBitmapPath("image/trans_crosshair.bmp");

	// load bitmap file
//	mBitmap = (HBITMAP) LoadImage(
//	NULL, /* HINSTANCE to load from a dll or exe */
//	mParam.getBitmapPath().c_str(), /* path of the bitmap */
//	IMAGE_BITMAP, /* or IMAGE_CURSOR or IMAGE_ICON  */
//	0, /* cxDesired is desired width */
//	0, /* cyDesired is desired height */
//	LR_LOADFROMFILE /* LR_LOADTRANSPARENT, LR_DEFAULTSIZE (if cxDesired or cyDesired or not 0)  */
//	);

// PhotoshopCrosshair.png

	//mImage = new Gdiplus::Image(L"image/PhotoshopCrosshair.png");

	return true;
}

/**
 * on windows creation
 */
void WindowController::onCreate(HWND hwnd)
{
	//mHwnd = hwnd;

	//
	// fight Killing Floor
	//
	HWND killingFloorHwnd = FindWindow(NULL, "Killing Floor");
	if (killingFloorHwnd)
	{
		std::cout << "Getting killing floor window worked!" << std::endl;
		//HWND_NOTOPMOST
		SetWindowPos(killingFloorHwnd,
		HWND_NOTOPMOST, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE);
	}
}

void WindowController::onChar(HWND hwnd, wchar_t c)
{
//	RECT rect;
//	GetWindowRect(hwnd, &rect);
	POINT pos = mParam.getPosition();
//	pos.x = rect.left + 20;
//	pos.y = rect.top + 20;
	c = towlower(c);
	switch (c)
	{
	case L'w':
		pos.y -= 1;
		setWindowPosition(hwnd, pos, true);
		break;
	case L'a':
		pos.x -= 1;
		setWindowPosition(hwnd, pos, true);
		break;
	case L's':
		pos.y += 1;
		setWindowPosition(hwnd, pos, true);
		break;
	case L'd':
		pos.x += 1;
		setWindowPosition(hwnd, pos, true);
		break;
	case L'p':
		// enable position changing by mouse click
		setChangingPosition(true);
		break;
	case L'o':
		//setImage(L"image/PhotoshopCrosshair.png");
		onFileOpen(hwnd);
		break;
	}
}

void WindowController::onPaint(HWND hwnd)
{
	//cout << "WindowController::onPaint is called!" << endl;
//
//	PAINTSTRUCT ps;
//	HDC hdc = BeginPaint(hwnd, &ps);
//
//	draw(hdc);
//
//	EndPaint(hwnd, &ps);

	BLENDFUNCTION blend;
	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendFlags = 0;
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	PAINTSTRUCT ps;
	HDC hdcOff;
	HBRUSH BackgroundBrush = CreateSolidBrush(RGB(255, 255, 255));
	HDC hdc = GetDC(0);
	RECT r;
	GetClientRect(mHwnd, &r);
	hdcOff = CreateCompatibleDC(hdc);
	HBITMAP hbmp = CreateCompatibleBitmap(hdc, r.right - r.left,
			r.bottom - r.top);
	SelectObject(hdcOff, hbmp);
	FillRect(hdcOff, &(ps.rcPaint), BackgroundBrush);
	draw(hdcOff);
	SIZE s;
	s.cx = r.right - r.left;
	s.cy = r.bottom - r.top;
//	POINT p;
//	p.x = r.left;
//	p.y = r.top;
	POINT p2;
	p2.x = 0;
	p2.y = 0;
	if (UpdateLayeredWindow(mHwnd, hdc, NULL, &s, hdcOff, &p2,
			RGB(255, 255, 255), &blend, ULW_ALPHA) == 0)
	{
		DWORD err = GetLastError();
		cout << "UpdateLayeredWindow failed with error: " << err << " hex: 0x"
				<< std::hex << err << endl;
		exit(0);
	}
	DeleteObject(hbmp);
	DeleteDC(hdcOff);
	DeleteObject(BackgroundBrush);
}

void WindowController::draw(HDC hdc)
{
	Gdiplus::Graphics graphics(hdc);

	if (mImage)
	{
		graphics.DrawImage(mImage, 0, 0, 60, 60);
	}
	else
	{
		LONG w = mParam.getWidth();
		LONG h = mParam.getHeight();
		Gdiplus::Pen pen(Gdiplus::Color(128, 0, 0, 255));
		graphics.SetCompositingQuality(
				Gdiplus::CompositingQualityGammaCorrected);
		graphics.DrawLine(&pen, 0, h / 2, w, h / 2);
		graphics.DrawLine(&pen, w / 2, 0, w / 2, h);
	}

	// rendering text
//	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0, 0));
//	Gdiplus::FontFamily fontFamily(L"Times New Roman");
//	Gdiplus::Font font(&fontFamily, 20, Gdiplus::FontStyleRegular,
//			Gdiplus::UnitPixel);
//	Gdiplus::PointF pointF(0.0f, 20.0f);
	//graphics.DrawString(L"Hello World!", -1, &font, pointF, &brush);

	//		// Bitmap rendering
	//		HDC memDC = CreateCompatibleDC(hdc);
	//		// Select the new bitmap
	//		SelectObject(memDC, mBitmap);
	//
	//		// Copy the bits from the memory DC into the current dc
	//		BitBlt(hdc, 0, 0, 40, 40, memDC, 0, 0, SRCCOPY);
	//
	//		// Restore the old bitmap
	//		DeleteDC(memDC);
}

void WindowController::onKillFocus(HWND hwnd)
{
	if (isChangingPosition())
	{
		POINT p;
		if (GetCursorPos(&p))
		{
			//cursor position now in p.x and p.y
			//std::cout << "pos: " << p.x << ", " << p.y << std::endl;
			//SetWindowPosition(hwnd, p);

			setWindowPosition(hwnd, p, true); // move centered

		}
		setChangingPosition(false);
	}
}

void WindowController::onFileOpen(HWND hwnd)
{
	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	WCHAR szFileName[MAX_PATH];
	szFileName[0] = L'\0';

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = GetModuleHandle(NULL);
	ofn.lpstrFilter = L"All (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;

	if (GetOpenFileNameW(&ofn))
	{
		if (!setImage(szFileName))
		{
			cerr << "Failed to load the file..." << endl;
			NotifyError(hwnd, "Cannot open this file.");
			return; // early return on error;
		}

		onSize(mHwnd);
		onPaint(mHwnd);

	}
}

bool WindowController::setImage(PCWSTR pszFileName)
{
	cout << "WindowController::setImage with: " << pszFileName << endl;
	if (mImage)
	{
		delete mImage;
		mImage = NULL;
	}

	mImage = new Gdiplus::Image(pszFileName);

	if (!mImage)
	{
		return false;
	}

	return true;
}

int WindowController::messageLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int) msg.wParam;
}

void WindowController::NotifyError(HWND hwnd, LPCSTR pszMessage)
{
	MessageBox(hwnd, pszMessage, "Error", MB_OK | MB_ICONERROR);
}

void WindowController::setWindowPosition(HWND hwnd, POINT p, bool centered)
{
	LONG w = mParam.getWidth();
	LONG h = mParam.getHeight();
	if (centered)
	{
		MoveWindow(hwnd, p.x - (w / 2), p.y - (h / 2), w, h, true);
	}
	else
	{
		MoveWindow(hwnd, p.x, p.y, w, h, true);
	}
	mParam.setPosition(p);

	//BringWindowToTop(hwnd); // doesn't fight other windows that are on top
}

const WindowParam& WindowController::getParam() const
{
	return mParam;
}

void WindowController::setParam(const WindowParam& param)
{
	mParam = param;
}

bool WindowController::isChangingPosition() const
{
	return mChangingPosition;
}

void WindowController::setChangingPosition(bool changingPosition)
{
	mChangingPosition = changingPosition;
}

} /* namespace xover */
