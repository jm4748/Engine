#include "input.h"


void Input::SetupKeyMap()
{
	keyMap[ 0x1B ] = K_ESCAPE;
	keyMap[ 0x70 ] = K_F1;
	keyMap[ 0x71 ] = K_F2;
	keyMap[ 0x72 ] = K_F3;
	keyMap[ 0x73 ] = K_F4;
	keyMap[ 0x74 ] = K_F5;
	keyMap[ 0x75 ] = K_F6;
	keyMap[ 0x76 ] = K_F7;
	keyMap[ 0x77 ] = K_F8;
	keyMap[ 0x78 ] = K_F9;
	keyMap[ 0x79 ] = K_F10;
	keyMap[ 0x7A ] = K_F11;
	keyMap[ 0x7B ] = K_F12;
	keyMap[ 0xC0 ] = K_BACKTICK;
	keyMap[ 0x30 ] = K_0;
	keyMap[ 0x31 ] = K_1;
	keyMap[ 0x32 ] = K_2;
	keyMap[ 0x33 ] = K_3;
	keyMap[ 0x34 ] = K_4;
	keyMap[ 0x35 ] = K_5;
	keyMap[ 0x36 ] = K_6;
	keyMap[ 0x37 ] = K_7;
	keyMap[ 0x38 ] = K_8;
	keyMap[ 0x39 ] = K_9;
	keyMap[ 0xBD ] = K_MINUSSIGN;
	keyMap[ 0xBB ] = K_EQUALSIGN;
	keyMap[ 0x08 ] = K_BACKSPACE;
	keyMap[ 0x09 ] = K_TAB;
	keyMap[ 0x41 ] = K_A;
	keyMap[ 0x42 ] = K_B;
	keyMap[ 0x43 ] = K_C;
	keyMap[ 0x44 ] = K_D;
	keyMap[ 0x45 ] = K_E;
	keyMap[ 0x46 ] = K_F;
	keyMap[ 0x47 ] = K_G;
	keyMap[ 0x48 ] = K_H;
	keyMap[ 0x49 ] = K_I;
	keyMap[ 0x4A ] = K_J;
	keyMap[ 0x4B ] = K_K;
	keyMap[ 0x4C ] = K_L;
	keyMap[ 0x4D ] = K_M;
	keyMap[ 0x4E ] = K_N;
	keyMap[ 0x4F ] = K_O;
	keyMap[ 0x50 ] = K_P;
	keyMap[ 0x51 ] = K_Q;
	keyMap[ 0x52 ] = K_R;
	keyMap[ 0x53 ] = K_S;
	keyMap[ 0x54 ] = K_T;
	keyMap[ 0x55 ] = K_U;
	keyMap[ 0x56 ] = K_V;
	keyMap[ 0x57 ] = K_W;
	keyMap[ 0x58 ] = K_X;
	keyMap[ 0x59 ] = K_Y;
	keyMap[ 0x5A ] = K_Z;
	keyMap[ 0xDB ] = K_LBRACKET;
	keyMap[ 0xDD ] = K_RBRACKET;
	keyMap[ 0xDC ] = K_BACKSLASH;
	keyMap[ 0x14 ] = K_CAPSLOCK;
	keyMap[ 0xBA ] = K_SEMICOLON;
	keyMap[ 0xDE ] = K_SINGLEQUOTE;
	keyMap[ 0x0D ] = K_ENTER;
	keyMap[ 0xA0 ] = K_LSHIFT;
	keyMap[ 0xA1 ] = K_RSHIFT;
	keyMap[ 0xBC ] = K_COMMA;
	keyMap[ 0xBE ] = K_PERIOD;
	keyMap[ 0xBF ] = K_FORWARDSLASH;
	keyMap[ 0xA2 ] = K_LCTRL;
	keyMap[ 0xA3 ] = K_RCTRL;
	keyMap[ 0xA4 ] = K_LALT;
	keyMap[ 0xA5 ] = K_RALT;
	keyMap[ 0x2C ] = K_PRINTSCREEN;
	keyMap[ 0x13 ] = K_PAUSE;
	keyMap[ 0x2D ] = K_INSERT;
	keyMap[ 0x24 ] = K_HOME;
	keyMap[ 0x21 ] = K_PAGEUP;
	keyMap[ 0x22 ] = K_PAGEDOWN;
	keyMap[ 0x2E ] = K_DELETE;
	keyMap[ 0x23 ] = K_END;
	keyMap[ 0x26 ] = K_UP;
	keyMap[ 0x28 ] = K_DOWN;
	keyMap[ 0x25 ] = K_LEFT;
	keyMap[ 0x27 ] = K_RIGHT;
}


Key Input::VKToKey( USHORT vk )
{
	std::map< USHORT, Key >::iterator it = keyMap.find( vk );

	if ( it != keyMap.end() )
		return it->second;
	else
		return K_COUNT;
}


void Input::SetupKeyNames()
{
	keyNames[ K_ESCAPE ]       = "Escape";
	keyNames[ K_F1 ]           = "F1";
	keyNames[ K_F2 ]           = "F2";
	keyNames[ K_F3 ]           = "F3";
	keyNames[ K_F4 ]           = "F4";
	keyNames[ K_F5 ]           = "F5";
	keyNames[ K_F6 ]           = "F6";
	keyNames[ K_F7 ]           = "F7";
	keyNames[ K_F8 ]           = "F8";
	keyNames[ K_F9 ]           = "F9";
	keyNames[ K_F10 ]          = "F10";
	keyNames[ K_F11 ]          = "F11";
	keyNames[ K_F12 ]          = "F12";
	keyNames[ K_BACKTICK ]     = "Backtick";
	keyNames[ K_0 ]            = "0";
	keyNames[ K_1 ]            = "1";
	keyNames[ K_2 ]            = "2";
	keyNames[ K_3 ]            = "3";
	keyNames[ K_4 ]            = "4";
	keyNames[ K_5 ]            = "5";
	keyNames[ K_6 ]            = "6";
	keyNames[ K_7 ]            = "7";
	keyNames[ K_8 ]            = "8";
	keyNames[ K_9 ]            = "9";
	keyNames[ K_MINUSSIGN ]    = "Minus Sign";
	keyNames[ K_EQUALSIGN ]    = "Equal Sign";
	keyNames[ K_BACKSPACE ]    = "Backspace";
	keyNames[ K_TAB ]          = "Tab";
	keyNames[ K_A ]            = "A";
	keyNames[ K_B ]            = "B";
	keyNames[ K_C ]            = "C";
	keyNames[ K_D ]            = "D";
	keyNames[ K_E ]            = "E";
	keyNames[ K_F ]            = "F";
	keyNames[ K_G ]            = "G";
	keyNames[ K_H ]            = "H";
	keyNames[ K_I ]            = "I";
	keyNames[ K_J ]            = "J";
	keyNames[ K_K ]            = "K";
	keyNames[ K_L ]            = "L";
	keyNames[ K_M ]            = "M";
	keyNames[ K_N ]            = "N";
	keyNames[ K_O ]            = "O";
	keyNames[ K_P ]            = "P";
	keyNames[ K_Q ]            = "Q";
	keyNames[ K_R ]            = "R";
	keyNames[ K_S ]            = "S";
	keyNames[ K_T ]            = "T";
	keyNames[ K_U ]            = "U";
	keyNames[ K_V ]            = "V";
	keyNames[ K_W ]            = "W";
	keyNames[ K_X ]            = "X";
	keyNames[ K_Y ]            = "Y";
	keyNames[ K_Z ]            = "Z";
	keyNames[ K_LBRACKET ]     = "Left Bracket";
	keyNames[ K_RBRACKET ]     = "Right Bracket";
	keyNames[ K_BACKSLASH ]    = "Backslash";
	keyNames[ K_CAPSLOCK ]     = "Caps Lock";
	keyNames[ K_SEMICOLON ]    = "Semicolon";
	keyNames[ K_SINGLEQUOTE ]  = "Single Quote";
	keyNames[ K_ENTER ]        = "Enter";
	keyNames[ K_LSHIFT ]       = "Left Shift";
	keyNames[ K_RSHIFT ]       = "Right Shift";
	keyNames[ K_COMMA ]        = "Comma";
	keyNames[ K_PERIOD ]       = "Period";
	keyNames[ K_FORWARDSLASH ] = "Forward Slash";
	keyNames[ K_LCTRL ]        = "Left Ctrl";
	keyNames[ K_RCTRL ]        = "Right Ctrl";
	keyNames[ K_LALT ]         = "Left Alt";
	keyNames[ K_RALT ]         = "Right Alt";
	keyNames[ K_PRINTSCREEN ]  = "Print Screen";
	keyNames[ K_PAUSE ]        = "Pause";
	keyNames[ K_INSERT ]       = "Insert";
	keyNames[ K_HOME ]         = "Home";
	keyNames[ K_PAGEUP ]       = "Page Up";
	keyNames[ K_PAGEDOWN ]     = "Page Down";
	keyNames[ K_DELETE ]       = "Delete";
	keyNames[ K_END ]          = "End";
	keyNames[ K_UP ]           = "Up";
	keyNames[ K_DOWN ]         = "Down";
	keyNames[ K_LEFT ]         = "Left";
	keyNames[ K_RIGHT ]        = "Right";
	keyNames[ K_COUNT ]        = "Unknown";
}