#pragma once
#include <NE_Common.h>

#define MOUSE_BUTTON(X)       (1 << ((X)-1))
#define MOUSE_BUTTON_LEFT     1
#define MOUSE_BUTTON_MIDDLE   2
#define MOUSE_BUTTON_RIGHT    3
#define MOUSE_BUTTON_X1       4
#define MOUSE_BUTTON_X2       5
#define MOUSE_BUTTON_LMASK    MOUSE_BUTTON(MOUSE_BUTTON_LEFT)
#define MOUSE_BUTTON_MMASK    MOUSE_BUTTON(MOUSE_BUTTON_MIDDLE)
#define MOUSE_BUTTON_RMASK    MOUSE_BUTTON(MOUSE_BUTTON_RIGHT)
#define MOUSE_BUTTON_X1MASK   MOUSE_BUTTON(MOUSE_BUTTON_X1)
#define MOUSE_BUTTON_X2MASK   MOUSE_BUTTON(MOUSE_BUTTON_X2)

namespace Nuclear
{
	namespace Platform
	{
		class NEAPI Input
		{
		public:
			enum class MouseInputMode { Virtual, Normal, Hidden };

            enum KeyboardKey
            {
                KEYBOARD_KEY_UNKNOWN = 0,

                KEYBOARD_KEY_A = 4,
                KEYBOARD_KEY_B = 5,
                KEYBOARD_KEY_C = 6,
                KEYBOARD_KEY_D = 7,
                KEYBOARD_KEY_E = 8,
                KEYBOARD_KEY_F = 9,
                KEYBOARD_KEY_G = 10,
                KEYBOARD_KEY_H = 11,
                KEYBOARD_KEY_I = 12,
                KEYBOARD_KEY_J = 13,
                KEYBOARD_KEY_K = 14,
                KEYBOARD_KEY_L = 15,
                KEYBOARD_KEY_M = 16,
                KEYBOARD_KEY_N = 17,
                KEYBOARD_KEY_O = 18,
                KEYBOARD_KEY_P = 19,
                KEYBOARD_KEY_Q = 20,
                KEYBOARD_KEY_R = 21,
                KEYBOARD_KEY_S = 22,
                KEYBOARD_KEY_T = 23,
                KEYBOARD_KEY_U = 24,
                KEYBOARD_KEY_V = 25,
                KEYBOARD_KEY_W = 26,
                KEYBOARD_KEY_X = 27,
                KEYBOARD_KEY_Y = 28,
                KEYBOARD_KEY_Z = 29,

                KEYBOARD_KEY_1 = 30,
                KEYBOARD_KEY_2 = 31,
                KEYBOARD_KEY_3 = 32,
                KEYBOARD_KEY_4 = 33,
                KEYBOARD_KEY_5 = 34,
                KEYBOARD_KEY_6 = 35,
                KEYBOARD_KEY_7 = 36,
                KEYBOARD_KEY_8 = 37,
                KEYBOARD_KEY_9 = 38,
                KEYBOARD_KEY_0 = 39,

                KEYBOARD_KEY_RETURN = 40,
                KEYBOARD_KEY_ESCAPE = 41,
                KEYBOARD_KEY_BACKSPACE = 42,
                KEYBOARD_KEY_TAB = 43,
                KEYBOARD_KEY_SPACE = 44,

                KEYBOARD_KEY_MINUS = 45,
                KEYBOARD_KEY_EQUALS = 46,
                KEYBOARD_KEY_LEFTBRACKET = 47,
                KEYBOARD_KEY_RIGHTBRACKET = 48,
                KEYBOARD_KEY_BACKSLASH = 49,
                KEYBOARD_KEY_NONUSHASH = 50,
                KEYBOARD_KEY_SEMICOLON = 51,
                KEYBOARD_KEY_APOSTROPHE = 52,
                KEYBOARD_KEY_GRAVE = 53,
                KEYBOARD_KEY_COMMA = 54,
                KEYBOARD_KEY_PERIOD = 55,
                KEYBOARD_KEY_SLASH = 56,

                KEYBOARD_KEY_CAPSLOCK = 57,

                KEYBOARD_KEY_F1 = 58,
                KEYBOARD_KEY_F2 = 59,
                KEYBOARD_KEY_F3 = 60,
                KEYBOARD_KEY_F4 = 61,
                KEYBOARD_KEY_F5 = 62,
                KEYBOARD_KEY_F6 = 63,
                KEYBOARD_KEY_F7 = 64,
                KEYBOARD_KEY_F8 = 65,
                KEYBOARD_KEY_F9 = 66,
                KEYBOARD_KEY_F10 = 67,
                KEYBOARD_KEY_F11 = 68,
                KEYBOARD_KEY_F12 = 69,

                KEYBOARD_KEY_PRINTSCREEN = 70,
                KEYBOARD_KEY_SCROLLLOCK = 71,
                KEYBOARD_KEY_PAUSE = 72,
                KEYBOARD_KEY_INSERT = 73,
                KEYBOARD_KEY_HOME = 74,
                KEYBOARD_KEY_PAGEUP = 75,
                KEYBOARD_KEY_DELETE = 76,
                KEYBOARD_KEY_END = 77,
                KEYBOARD_KEY_PAGEDOWN = 78,
                KEYBOARD_KEY_RIGHT = 79,
                KEYBOARD_KEY_LEFT = 80,
                KEYBOARD_KEY_DOWN = 81,
                KEYBOARD_KEY_UP = 82,

                KEYBOARD_KEY_NUMLOCKCLEAR = 83,
                KEYBOARD_KEY_KP_DIVIDE = 84,
                KEYBOARD_KEY_KP_MULTIPLY = 85,
                KEYBOARD_KEY_KP_MINUS = 86,
                KEYBOARD_KEY_KP_PLUS = 87,
                KEYBOARD_KEY_KP_ENTER = 88,
                KEYBOARD_KEY_KP_1 = 89,
                KEYBOARD_KEY_KP_2 = 90,
                KEYBOARD_KEY_KP_3 = 91,
                KEYBOARD_KEY_KP_4 = 92,
                KEYBOARD_KEY_KP_5 = 93,
                KEYBOARD_KEY_KP_6 = 94,
                KEYBOARD_KEY_KP_7 = 95,
                KEYBOARD_KEY_KP_8 = 96,
                KEYBOARD_KEY_KP_9 = 97,
                KEYBOARD_KEY_KP_0 = 98,
                KEYBOARD_KEY_KP_PERIOD = 99,

                KEYBOARD_KEY_NONUSBACKSLASH = 100,
                KEYBOARD_KEY_APPLICATION = 101,
                KEYBOARD_KEY_POWER = 102,
                KEYBOARD_KEY_KP_EQUALS = 103,
                KEYBOARD_KEY_F13 = 104,
                KEYBOARD_KEY_F14 = 105,
                KEYBOARD_KEY_F15 = 106,
                KEYBOARD_KEY_F16 = 107,
                KEYBOARD_KEY_F17 = 108,
                KEYBOARD_KEY_F18 = 109,
                KEYBOARD_KEY_F19 = 110,
                KEYBOARD_KEY_F20 = 111,
                KEYBOARD_KEY_F21 = 112,
                KEYBOARD_KEY_F22 = 113,
                KEYBOARD_KEY_F23 = 114,
                KEYBOARD_KEY_F24 = 115,
                KEYBOARD_KEY_EXECUTE = 116,
                KEYBOARD_KEY_HELP = 117,
                KEYBOARD_KEY_MENU = 118,
                KEYBOARD_KEY_SELECT = 119,
                KEYBOARD_KEY_STOP = 120,    /**< AC Stop */
                KEYBOARD_KEY_AGAIN = 121,   /**< AC Redo/Repeat */
                KEYBOARD_KEY_UNDO = 122,    /**< AC Undo */
                KEYBOARD_KEY_CUT = 123,     /**< AC Cut */
                KEYBOARD_KEY_COPY = 124,    /**< AC Copy */
                KEYBOARD_KEY_PASTE = 125,   /**< AC Paste */
                KEYBOARD_KEY_FIND = 126,    /**< AC Find */
                KEYBOARD_KEY_MUTE = 127,
                KEYBOARD_KEY_VOLUMEUP = 128,
                KEYBOARD_KEY_VOLUMEDOWN = 129,

                KEYBOARD_KEY_KP_COMMA = 133,
                KEYBOARD_KEY_KP_EQUALSAS400 = 134,

                KEYBOARD_KEY_INTERNATIONAL1 = 135,
                KEYBOARD_KEY_INTERNATIONAL2 = 136,
                KEYBOARD_KEY_INTERNATIONAL3 = 137, /**< Yen */
                KEYBOARD_KEY_INTERNATIONAL4 = 138,
                KEYBOARD_KEY_INTERNATIONAL5 = 139,
                KEYBOARD_KEY_INTERNATIONAL6 = 140,
                KEYBOARD_KEY_INTERNATIONAL7 = 141,
                KEYBOARD_KEY_INTERNATIONAL8 = 142,
                KEYBOARD_KEY_INTERNATIONAL9 = 143,
                KEYBOARD_KEY_LANG1 = 144, /**< Hangul/English toggle */
                KEYBOARD_KEY_LANG2 = 145, /**< Hanja conversion */
                KEYBOARD_KEY_LANG3 = 146, /**< Katakana */
                KEYBOARD_KEY_LANG4 = 147, /**< Hiragana */
                KEYBOARD_KEY_LANG5 = 148, /**< Zenkaku/Hankaku */
                KEYBOARD_KEY_LANG6 = 149, /**< reserved */
                KEYBOARD_KEY_LANG7 = 150, /**< reserved */
                KEYBOARD_KEY_LANG8 = 151, /**< reserved */
                KEYBOARD_KEY_LANG9 = 152, /**< reserved */

                KEYBOARD_KEY_ALTERASE = 153,    /**< Erase-Eaze */
                KEYBOARD_KEY_SYSREQ = 154,
                KEYBOARD_KEY_CANCEL = 155,      /**< AC Cancel */
                KEYBOARD_KEY_CLEAR = 156,
                KEYBOARD_KEY_PRIOR = 157,
                KEYBOARD_KEY_RETURN2 = 158,
                KEYBOARD_KEY_SEPARATOR = 159,
                KEYBOARD_KEY_OUT = 160,
                KEYBOARD_KEY_OPER = 161,
                KEYBOARD_KEY_CLEARAGAIN = 162,
                KEYBOARD_KEY_CRSEL = 163,
                KEYBOARD_KEY_EXSEL = 164,

                KEYBOARD_KEY_KP_00 = 176,
                KEYBOARD_KEY_KP_000 = 177,
                KEYBOARD_KEY_THOUSANDSSEPARATOR = 178,
                KEYBOARD_KEY_DECIMALSEPARATOR = 179,
                KEYBOARD_KEY_CURRENCYUNIT = 180,
                KEYBOARD_KEY_CURRENCYSUBUNIT = 181,
                KEYBOARD_KEY_KP_LEFTPAREN = 182,
                KEYBOARD_KEY_KP_RIGHTPAREN = 183,
                KEYBOARD_KEY_KP_LEFTBRACE = 184,
                KEYBOARD_KEY_KP_RIGHTBRACE = 185,
                KEYBOARD_KEY_KP_TAB = 186,
                KEYBOARD_KEY_KP_BACKSPACE = 187,
                KEYBOARD_KEY_KP_A = 188,
                KEYBOARD_KEY_KP_B = 189,
                KEYBOARD_KEY_KP_C = 190,
                KEYBOARD_KEY_KP_D = 191,
                KEYBOARD_KEY_KP_E = 192,
                KEYBOARD_KEY_KP_F = 193,
                KEYBOARD_KEY_KP_XOR = 194,
                KEYBOARD_KEY_KP_POWER = 195,
                KEYBOARD_KEY_KP_PERCENT = 196,
                KEYBOARD_KEY_KP_LESS = 197,
                KEYBOARD_KEY_KP_GREATER = 198,
                KEYBOARD_KEY_KP_AMPERSAND = 199,
                KEYBOARD_KEY_KP_DBLAMPERSAND = 200,
                KEYBOARD_KEY_KP_VERTICALBAR = 201,
                KEYBOARD_KEY_KP_DBLVERTICALBAR = 202,
                KEYBOARD_KEY_KP_COLON = 203,
                KEYBOARD_KEY_KP_HASH = 204,
                KEYBOARD_KEY_KP_SPACE = 205,
                KEYBOARD_KEY_KP_AT = 206,
                KEYBOARD_KEY_KP_EXCLAM = 207,
                KEYBOARD_KEY_KP_MEMSTORE = 208,
                KEYBOARD_KEY_KP_MEMRECALL = 209,
                KEYBOARD_KEY_KP_MEMCLEAR = 210,
                KEYBOARD_KEY_KP_MEMADD = 211,
                KEYBOARD_KEY_KP_MEMSUBTRACT = 212,
                KEYBOARD_KEY_KP_MEMMULTIPLY = 213,
                KEYBOARD_KEY_KP_MEMDIVIDE = 214,
                KEYBOARD_KEY_KP_PLUSMINUS = 215,
                KEYBOARD_KEY_KP_CLEAR = 216,
                KEYBOARD_KEY_KP_CLEARENTRY = 217,
                KEYBOARD_KEY_KP_BINARY = 218,
                KEYBOARD_KEY_KP_OCTAL = 219,
                KEYBOARD_KEY_KP_DECIMAL = 220,
                KEYBOARD_KEY_KP_HEXADECIMAL = 221,

                KEYBOARD_KEY_LCTRL = 224,
                KEYBOARD_KEY_LSHIFT = 225,
                KEYBOARD_KEY_LALT = 226, /**< alt, option */
                KEYBOARD_KEY_LGUI = 227, /**< windows, command (apple), meta */
                KEYBOARD_KEY_RCTRL = 228,
                KEYBOARD_KEY_RSHIFT = 229,
                KEYBOARD_KEY_RALT = 230, /**< alt gr, option */
                KEYBOARD_KEY_RGUI = 231, /**< windows, command (apple), meta */

                KEYBOARD_KEY_MODE = 257,

                KEYBOARD_KEY_AUDIONEXT = 258,
                KEYBOARD_KEY_AUDIOPREV = 259,
                KEYBOARD_KEY_AUDIOSTOP = 260,
                KEYBOARD_KEY_AUDIOPLAY = 261,
                KEYBOARD_KEY_AUDIOMUTE = 262,
                KEYBOARD_KEY_MEDIASELECT = 263,
                KEYBOARD_KEY_WWW = 264,             /**< AL Internet Browser */
                KEYBOARD_KEY_MAIL = 265,
                KEYBOARD_KEY_CALCULATOR = 266,      /**< AL Calculator */
                KEYBOARD_KEY_COMPUTER = 267,
                KEYBOARD_KEY_AC_SEARCH = 268,       /**< AC Search */
                KEYBOARD_KEY_AC_HOME = 269,         /**< AC Home */
                KEYBOARD_KEY_AC_BACK = 270,         /**< AC Back */
                KEYBOARD_KEY_AC_FORWARD = 271,      /**< AC Forward */
                KEYBOARD_KEY_AC_STOP = 272,         /**< AC Stop */
                KEYBOARD_KEY_AC_REFRESH = 273,      /**< AC Refresh */
                KEYBOARD_KEY_AC_BOOKMARKS = 274,    /**< AC Bookmarks */



                KEYBOARD_KEY_BRIGHTNESSDOWN = 275,
                KEYBOARD_KEY_BRIGHTNESSUP = 276,
                KEYBOARD_KEY_DISPLAYSWITCH = 277,
                KEYBOARD_KEY_KBDILLUMTOGGLE = 278,
                KEYBOARD_KEY_KBDILLUMDOWN = 279,
                KEYBOARD_KEY_KBDILLUMUP = 280,
                KEYBOARD_KEY_EJECT = 281,
                KEYBOARD_KEY_SLEEP = 282,           /**< SC System Sleep */

                KEYBOARD_KEY_APP1 = 283,
                KEYBOARD_KEY_APP2 = 284,


                KEYBOARD_KEY_AUDIOREWIND = 285,
                KEYBOARD_KEY_AUDIOFASTFORWARD = 286,

                KEYBOARD_KEY_SOFTLEFT = 287,
                KEYBOARD_KEY_SOFTRIGHT = 288,
                KEYBOARD_KEY_CALL = 289,
                KEYBOARD_KEY_ENDCALL = 290,

                KEYBOARD_KEY_NUM_SCANCODES = 512
            };

			enum class KeyboardKeyStatus
			{
				Released = 0,
				Pressed = 1
			};

            virtual const Uint8* GetKeyboardState() = 0;

			virtual KeyboardKeyStatus GetKeyStatus(KeyboardKey key) = 0;
			virtual void SetMouseInputMode(const MouseInputMode& mode) = 0;

			virtual Uint32 GetMouseState(int* xpos, int* ypos) = 0;
		};

	}
}