//============================================================================
// Name        : Main.cpp
// Author      : Emile Bergeron <emile.bergeron1@uqac.ca>
// Version     :
// Copyright   : 
// Description : Windows that act as an always on top overlay.
//============================================================================

#include "WindowController.h"

//#include <windows.h>

#include <iostream>

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow)

{
	using std::cout;
	using std::endl;
	using xover::WindowController;

	// Show a menu on startup
	std::string menuStr =
			std::string() + "Thank you for trying this application.\n\n"
					+ "Press...\n"
					+ "P - to activate the positionning, then click where you want the crosshair.\n"
					+ "O - to open a file (PNG, BMP), alpha channel is supported\n"
					+ "W,A,S,D - moves the crosshair pixel by pixel for fine tuning the position.\n"
					+ "\nN.B. you have to select the Crosshair app in your taskbar\n"
					+ "after each click since the app is clickthrough.";
	MessageBox(NULL, menuStr.c_str(), "Crosshair Overlay Info",
	MB_OK);

	WindowController * mainApp = WindowController::getInstance();
	cout << "WinMain: WinController instance OK" << endl;
	if (!mainApp->registerInstance(hThisInstance))
	{
		return 0; // failed to register
	}
	cout << "WinMain: registering OK" << endl;
	if (!mainApp->create(nCmdShow))
	{
		cout << "WinMain: create failed... hwnd null." << endl;
		return 0;
	}
	cout << "WinMain create OK" << endl;
	int nReturn = mainApp->messageLoop();
	cout << "WinMain returning: " << nReturn << endl;
	WindowController::kill();
	return nReturn;
}

