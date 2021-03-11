#pragma once


#include <Windows.h>

#include "input.h"
#include "graphics.h"


class Core
{
private:
	static Core* core;

	HWND hWnd;

	bool closed;

	//----------

	Input input;

	//----------

	bool resizing;
	int cw, ch, lw, lh;
	bool resized;

	Graphics graphics;

private:
	LRESULT ProcessMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	static LRESULT CALLBACK WindowProcedure( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	void GetCorrectSize( int& width, int& height );

	void Resize();

public:
	Core();

	bool Startup( const char* title, int width, int height );
	void Cleanup();

	void Service();
	void ResetFlags();

	bool WindowClosed();
	bool WindowResized( int& width, int& height );

	Input* GetInput();
	Graphics* GetGraphics();

	__int64 Time();

	void GetWindowSize( int& width, int& height );
};