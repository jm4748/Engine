#include "core.h"


Core* Core::core;


LRESULT Core::ProcessMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
	case WM_CLOSE:
	{
		closed = true;

		return 0;
	}

	case WM_INPUT:
	{
		input.ProcessInput( lParam );

		return 0;
	}

	case WM_ENTERSIZEMOVE:
	{
		resizing = true;

		return 0;
	}

	case WM_EXITSIZEMOVE:
	{
		resizing = false;

		Resize();

		return 0;
	}

	case WM_SIZE:
	{
		if ( wParam != SIZE_MINIMIZED )
		{
			cw = LOWORD( lParam );
			ch = HIWORD( lParam );

			if ( !resizing ) Resize();
		}

		return 0;
	}

	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* minMaxInfo = ( MINMAXINFO* )lParam;

		int width = 320;
		int height = 240;

		GetCorrectSize( width, height );

		minMaxInfo->ptMinTrackSize.x = width;
		minMaxInfo->ptMinTrackSize.y = height;

		return 0;
	}

	case WM_PAINT:
	{
		graphics.Clear( 0.0f, 0.0f, 0.0f, 0.0f );
		graphics.Present();
	}

	default:
	{
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	}
}

LRESULT CALLBACK Core::WindowProcedure( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return core->ProcessMessage( hWnd, message, wParam, lParam );
}


void Core::GetCorrectSize( int& width, int& height )
{
	RECT rect;

	rect.left   = 0;
	rect.top    = 0;
	rect.right  = width;
	rect.bottom = height;

	AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, FALSE );

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
}


void Core::Resize()
{
	if ( cw == lw && ch == lh ) return;

	lw = cw;
	lh = ch;

	graphics.ResizeBackBuffer( cw, ch );

	resized = true;
}


Core::Core()
{
	core = this;

	hWnd = NULL;

	closed = false;

	//----------

	resizing = false;
	cw = ch = lw = lh = 0;
	resized = false;
}


bool Core::Startup( const char* title, int width, int height )
{
	WNDCLASS wndClass;

	wndClass.style         = 0;
	wndClass.lpfnWndProc   = WindowProcedure;
	wndClass.cbClsExtra    = 0;
	wndClass.cbWndExtra    = 0;
	wndClass.hInstance     = GetModuleHandle( NULL );
	wndClass.hIcon         = NULL;
	wndClass.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName  = NULL;
	wndClass.lpszClassName = "Window";

	RegisterClass( &wndClass );

	int w = width;
	int h = height;

	GetCorrectSize( w, h );

	/* doesn't work for high res
	int x = ( GetSystemMetrics( SM_CXFULLSCREEN ) - w ) / 2;
	int y = ( GetSystemMetrics( SM_CYFULLSCREEN ) - h ) / 2;
	*/

	DEVMODE devMode;

	ZeroMemory( &devMode, sizeof( DEVMODE ) );
	devMode.dmSize = sizeof( DEVMODE );

	EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &devMode );

	int x = ( devMode.dmPelsWidth  - w ) / 2;
	int y = ( devMode.dmPelsHeight - h ) / 2;

	hWnd = CreateWindow( "Window", title, WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, GetModuleHandle( NULL ), NULL );

	if ( hWnd == NULL ) return false;

	//----------

	if ( !input.Startup( hWnd ) ) return false;

	//----------

	if ( !graphics.Startup( width, height, hWnd ) ) return false;

	//----------

	ShowWindow( hWnd, SW_SHOW );

	return true;
}

void Core::Cleanup()
{
	graphics.Cleanup();

	//----------

	input.Cleanup();

	//----------

	if ( hWnd != NULL ) DestroyWindow( hWnd );

	UnregisterClass( "Window", GetModuleHandle( NULL ) );
}


void Core::Service()
{
	MSG msg;

	while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) { TranslateMessage(&msg); DispatchMessage( &msg ); }
}

void Core::ResetFlags()
{
	closed = false;

	//----------

	input.ResetFlags();

	//----------

	resized = false;
}


bool Core::WindowClosed()
{
	return closed;
}

bool Core::WindowResized( int& width, int& height )
{
	width = cw;
	height = ch;

	return resized;
}


Input* Core::GetInput()
{
	return &input;
}

Graphics* Core::GetGraphics()
{
	return &graphics;
}


//time in microseconds (10^6)
__int64 Core::Time()
{
	static __int64 frequency = 0;

	if ( frequency == 0 ) QueryPerformanceFrequency( ( LARGE_INTEGER* )&frequency );

	__int64 counter;

	QueryPerformanceCounter( ( LARGE_INTEGER* )&counter );

	return 1'000'000 * counter / frequency;
}


void Core::GetWindowSize( int& width, int& height )
{
	width = cw;
	height = ch;
}