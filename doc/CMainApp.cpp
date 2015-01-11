/**
* Jean Michel Courivaud https://groups.google.com/d/msg/comp.os.ms-windows.programmer.win32/YUu2XZIhhcU/XukpZZKH7_cJ
*/

------------------- .h --------------------------------

#pragma once
#include <windows.h>
#include <windowsx.h>
#include <gdiplus.h>
using namespace Gdiplus;
#include "resource.h"

//
// CMainApp
// Main Window
//
class CMainApp
{
private:
	HINSTANCE m_hInstance; // application instance
	HWND m_hwnd; // main window handle
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam,
	LPARAM lParam);
	// Window Messages
	void OnCreate(HWND hwnd);
	void OnDestroy();
public:
	CMainApp();
	~CMainApp();
	void Register(HINSTANCE hInstance);
	void Create(int);
	int MessageLoop();
	void Draw(HDC hDC);
	void HandlePaint();
};
extern CMainApp* _MainApp;

-----------------------------------------------

----------------- .cpp -----------------------------

#include "stdafx.h"
#include "resource.h"
#include "ULWSample.h"

#define WINDOW_CLASS TEXT("SampleWindowClass")
CMainApp* _MainApp = 0;
int PASCAL WinMain( HINSTANCE hInstance,
HINSTANCE hPrevInstance,
LPSTR lpCmdLine,
int nCmdShow)
{
	_MainApp = new CMainApp;
	_MainApp->Register(hInstance);
	_MainApp->Create(nCmdShow);
	int nReturn = _MainApp->MessageLoop();
	delete _MainApp;
	return nReturn;
}
LRESULT CALLBACK CMainApp::WndProc(HWND hwnd, UINT msg, WPARAM wParam,
LPARAM lParam)
{
	CMainApp* mainApp = (CMainApp *)GetWindowLong(hwnd, GWL_USERDATA);
	LPCREATESTRUCT pcs;
	switch(msg)
	{
	case WM_NCCREATE:
		pcs = (LPCREATESTRUCT)lParam;
		mainApp = (CMainApp*)pcs->lpCreateParams;
		SetWindowLong(hwnd, GWL_USERDATA, (LONG)mainApp);
		break;
	case WM_CREATE:
		mainApp->OnCreate(hwnd);
		PostMessage(hwnd, WM_PAINT, 0, 0);
		break;
	case WM_DESTROY:
		mainApp->OnDestroy();
		break;
	case WM_PAINT:
		mainApp->HandlePaint();
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

CMainApp::CMainApp()
{
	m_hInstance = NULL;
	m_hwnd = NULL;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}
CMainApp::~CMainApp()
{
	GdiplusShutdown(gdiplusToken);
	OnDestroy();
}
void CMainApp::OnCreate(HWND hWnd)
{
	m_hwnd = hWnd;
}
void CMainApp::OnDestroy()
{
	SetWindowLong(m_hwnd, GWL_USERDATA, (long)0);
	PostQuitMessage(0);
}
void CMainApp::Register(HINSTANCE hInstance)
{
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_OWNDC | CS_SAVEBITS | CS_DBLCLKS;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ULWSAMPLE));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wndclass.lpszMenuName = 0;
	wndclass.lpszClassName = WINDOW_CLASS;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&wndclass);
	m_hInstance = hInstance;
}
void CMainApp::Create(int nShow)
{
	TCHAR szTitle[MAX_PATH+1];
	LoadString(m_hInstance, IDS_APP_TITLE, szTitle, MAX_PATH+1);
	m_hwnd = CreateWindow( WINDOW_CLASS,
	szTitle,
	WS_OVERLAPPEDWINDOW,
	100,
	100,
	100,
	100,
	NULL,
	NULL,
	m_hInstance,
	this);
	SetWindowLong(m_hwnd, GWL_EXSTYLE, WS_EX_LAYERED);
	ShowWindow(m_hwnd, nShow);
	UpdateWindow(m_hwnd);
}
int CMainApp::MessageLoop()
{
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
void CMainApp::HandlePaint()
{
	BLENDFUNCTION blend;
	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendFlags = 0;
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	PAINTSTRUCT ps;
	HDC hdcOff;
	HBRUSH BackgroundBrush = CreateSolidBrush(RGB(255,255,255));
	HDC hdc = GetDC(0);
	RECT r;
	GetClientRect(m_hwnd, &r);
	hdcOff = CreateCompatibleDC(hdc);
	HBITMAP hbmp = CreateCompatibleBitmap(hdc,
	r.right-r.left,r.bottom-r.top);
	HGDIOBJ holdbmp = SelectObject(hdcOff, hbmp);
	FillRect(hdcOff, &(ps.rcPaint), BackgroundBrush);
	Draw(hdcOff);
	SIZE s;
	s.cx = r.right-r.left;
	s.cy = r.bottom-r.top;
	POINT p;
	p.x = r.left;
	p.y = r.top;
	POINT p2;
	p2.x = 0;
	p2.y = 0;
	if (UpdateLayeredWindow(m_hwnd, hdc, NULL, &s, hdcOff, &p2,
				RGB(255,255,255), &blend, ULW_ALPHA) == 0) {
		DWORD err = GetLastError();
		exit(0);
	}
	DeleteObject(hbmp);
	DeleteDC(hdcOff);
	DeleteObject(BackgroundBrush);
}
void CMainApp::Draw(HDC hDC)
{
	Graphics graphics(hDC);
	SolidBrush brush(Color(255, 0, 0, 0));
	FontFamily fontFamily(L"Times New Roman");
	Font font(&fontFamily, 20, FontStyleRegular, UnitPixel);
	PointF pointF(10.0f, 20.0f);
	Pen opaquePen(Color(255, 0, 0, 255), 15);
	Pen semiTransPen(Color(128, 0, 0, 255), 15);
	graphics.DrawLine(&opaquePen, 0, 20, 100, 20);
	graphics.DrawLine(&semiTransPen, 0, 40, 100, 40);
	graphics.SetCompositingQuality(CompositingQualityGammaCorrected);
	graphics.DrawLine(&semiTransPen, 0, 60, 100, 60);
	graphics.DrawString(L"Hello World!", -1, &font, pointF, &brush);
}