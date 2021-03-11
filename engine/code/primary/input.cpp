#include "input.h"


Input::Input()
{
	SetupKeyMap();

	for ( int i = 0; i < K_COUNT; ++i )
	{
		keys[ i ].down     = false;
		keys[ i ].pressed  = false;
		keys[ i ].released = false;
	}

	mousePositionDeltaX = 0;
	mousePositionDeltaY = 0;

	for ( int i = 0; i < MB_COUNT; ++i )
	{
		mouseButtons[ i ].down     = false;
		mouseButtons[ i ].pressed  = false;
		mouseButtons[ i ].released = false;
	}

	mouseWheelDelta = 0;

	lastKey = K_COUNT;

	SetupKeyNames();
}


bool Input::Startup( HWND hWnd )
{
	this->hWnd = hWnd;

	RAWINPUTDEVICE inputDevices[ 2 ];

	//keyboard
	inputDevices[ 0 ].usUsagePage = 0x01;
	inputDevices[ 0 ].usUsage     = 0x06;
	inputDevices[ 0 ].dwFlags     = 0;
	inputDevices[ 0 ].hwndTarget  = hWnd;

	//mouse
	inputDevices[ 1 ].usUsagePage = 0x01;
	inputDevices[ 1 ].usUsage     = 0x02;
	inputDevices[ 1 ].dwFlags     = 0;
	inputDevices[ 1 ].hwndTarget  = hWnd;

	RegisterRawInputDevices( inputDevices, 2, sizeof( RAWINPUTDEVICE ) );

	return true;
}

void Input::Cleanup()
{
	//nothing needs to be done
}


void Input::ProcessInput( LPARAM lParam )
{
	RAWINPUT input;
	UINT size = sizeof( RAWINPUT );

	GetRawInputData( ( HRAWINPUT )lParam, RID_INPUT, &input, &size, sizeof( RAWINPUTHEADER ) );

	if ( input.header.dwType == RIM_TYPEKEYBOARD )
	{
		USHORT scanCode = input.data.keyboard.MakeCode;
		USHORT flags = input.data.keyboard.Flags;
		USHORT virtualKey = input.data.keyboard.VKey;

		bool e0 = ( ( flags & RI_KEY_E0 ) != 0 );

		switch ( virtualKey )
		{
		case 255: //fake keys
		{
			return;
		}

		case VK_SHIFT:
		{
			virtualKey = MapVirtualKey( scanCode, MAPVK_VSC_TO_VK_EX );

			break;
		}

		case VK_CONTROL:
		{
			if ( e0 )
				virtualKey = VK_RCONTROL;
			else
				virtualKey = VK_LCONTROL;

			break;
		}

		case VK_MENU: //alt key
		{
			if ( e0 )
				virtualKey = VK_RMENU;
			else
				virtualKey = VK_LMENU;

			break;
		}
		}

		bool pressed = ( ( flags & RI_KEY_BREAK ) == 0 );

		Key key = VKToKey( virtualKey );

		if ( key == K_COUNT ) return; //unrecognized key

		keys[ key ].down     =  pressed;
		keys[ key ].pressed  =  pressed;
		keys[ key ].released = !pressed;

		if ( pressed ) lastKey = key;
	}
	else if ( input.header.dwType == RIM_TYPEMOUSE ) //relative motion only
	{
		USHORT mouseButtonFlags = input.data.mouse.usButtonFlags;

		bool leftPressed = ( ( mouseButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN ) != 0 );

		mouseButtons[ MB_LEFT ].down     = leftPressed;
		mouseButtons[ MB_LEFT ].pressed  = leftPressed;
		mouseButtons[ MB_LEFT ].released = !leftPressed;

		bool rightPressed = ( ( mouseButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN ) != 0 );

		mouseButtons[ MB_RIGHT ].down     = rightPressed;
		mouseButtons[ MB_RIGHT ].pressed  = rightPressed;
		mouseButtons[ MB_RIGHT ].released = !rightPressed;

		bool middlePressed = ( ( mouseButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN ) != 0 );

		mouseButtons[ MB_MIDDLE ].down     = middlePressed;
		mouseButtons[ MB_MIDDLE ].pressed  = middlePressed;
		mouseButtons[ MB_MIDDLE ].released = !middlePressed;

		mousePositionDeltaX += input.data.mouse.lLastX;
		mousePositionDeltaY += input.data.mouse.lLastY;

		if ( ( mouseButtonFlags & RI_MOUSE_WHEEL ) != 0 )
			mouseWheelDelta += ( short )input.data.mouse.usButtonData;
	}
}


void Input::ResetFlags()
{
	for ( int i = 0; i < K_COUNT; ++i )
	{
		keys[ i ].pressed  = false;
		keys[ i ].released = false;
	}

	for ( int i = 0; i < MB_COUNT; ++i )
	{
		mouseButtons[ i ].pressed  = false;
		mouseButtons[ i ].released = false;
	}

	mousePositionDeltaX = 0;
	mousePositionDeltaY = 0;

	mouseWheelDelta = 0;

	lastKey = K_COUNT;
}


bool Input::KeyDown( Key key )
{
	return keys[ key ].down;
}

bool Input::KeyPressed( Key key )
{
	return keys[ key ].pressed;
}

bool Input::KeyReleased( Key key )
{
	return keys[ key ].released;
}


bool Input::MouseButtonDown( MouseButton mouseButton )
{
	return mouseButtons[ mouseButton ].down;
}

bool Input::MouseButtonPressed( MouseButton mouseButton )
{
	return mouseButtons[ mouseButton ].pressed;
}

bool Input::MouseButtonReleased( MouseButton mouseButton )
{
	return mouseButtons[ mouseButton ].released;
}


int Input::MousePositionDeltaX()
{
	return mousePositionDeltaX;
}

int Input::MousePositionDeltaY()
{
	return mousePositionDeltaY;
}


int Input::MouseWheelDelta()
{
	return mouseWheelDelta;
}


Key Input::LastKey()
{
	return lastKey;
}


int Input::CursorPositionX() //maybe rework this
{
	POINT point;

	GetCursorPos( &point );
	
	ScreenToClient( hWnd, &point );
	
	return point.x;
}

int Input::CursorPositionY() //maybe rework this
{
	POINT point;
	
	GetCursorPos( &point );
	
	ScreenToClient( hWnd, &point );
	
	return point.y;
}


std::string Input::KeyToString( Key key )
{
	return keyNames[ key ];
}