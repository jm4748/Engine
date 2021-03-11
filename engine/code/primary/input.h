#pragma once


#include <Windows.h>

#include <map>
#include <string>

#include "keys.h"


class Input
{
private:
	struct KeyState //*todo* replace bools with a single char and use the bits
	{
		bool down;
		bool pressed;
		bool released;
	};

	typedef KeyState MouseButtonState;

private:
	HWND hWnd;

	std::map< USHORT, Key > keyMap;

	KeyState keys[ K_COUNT ];

	MouseButtonState mouseButtons[ MB_COUNT ];

	int mousePositionDeltaX;
	int mousePositionDeltaY;

	short mouseWheelDelta;

	Key lastKey;

	std::string keyNames[ K_COUNT + 1 ]; //add 1 for unknown

private:
	void SetupKeyMap();

	Key VKToKey( USHORT vk );

	void SetupKeyNames();

public:
	Input();

	bool Startup( HWND hWnd );
	void Cleanup();

	void ProcessInput( LPARAM lParam );

	void ResetFlags();

	bool KeyDown( Key key );
	bool KeyPressed( Key key );
	bool KeyReleased( Key key );

	bool MouseButtonDown( MouseButton mouseButton );
	bool MouseButtonPressed( MouseButton mouseButton );
	bool MouseButtonReleased( MouseButton mouseButton );

	int MousePositionDeltaX();
	int MousePositionDeltaY();

	int MouseWheelDelta();

	Key LastKey();

	int CursorPositionX();
	int CursorPositionY();

	std::string KeyToString( Key key );
};