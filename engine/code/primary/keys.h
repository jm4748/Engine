#pragma once


enum Key //no numpad
{
	K_ESCAPE,
	K_F1, K_F2, K_F3, K_F4, K_F5, K_F6, K_F7, K_F8, K_F9, K_F10, K_F11, K_F12,
	K_BACKTICK,
	K_0, K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9,
	K_MINUSSIGN,
	K_EQUALSIGN,
	K_BACKSPACE,
	K_TAB,
	K_A, K_B, K_C, K_D, K_E, K_F, K_G, K_H, K_I, K_J, K_K, K_L, K_M, K_N, K_O, K_P, K_Q, K_R, K_S, K_T, K_U, K_V, K_W, K_X, K_Y, K_Z,
	K_LBRACKET, K_RBRACKET,
	K_BACKSLASH,
	K_CAPSLOCK,
	K_SEMICOLON,
	K_SINGLEQUOTE,
	K_ENTER,
	K_LSHIFT, K_RSHIFT,
	K_COMMA,
	K_PERIOD,
	K_FORWARDSLASH,
	K_LCTRL, K_RCTRL,
	K_LALT, K_RALT,
	K_PRINTSCREEN,
	K_PAUSE,
	K_INSERT,
	K_HOME,
	K_PAGEUP, K_PAGEDOWN,
	K_DELETE,
	K_END,
	K_UP, K_DOWN, K_LEFT, K_RIGHT,

	K_COUNT
};

#undef MB_RIGHT

enum MouseButton
{
	MB_LEFT, MB_MIDDLE, MB_RIGHT,

	MB_COUNT
};