﻿using Nuclear.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using static System.Runtime.CompilerServices.RuntimeHelpers;

namespace Nuclear.Platform
{
    public class Input
    {
        public enum KeyCode
        {
            UNKNOWN = 0,

            A = 4,
            B = 5,
            C = 6,
            D = 7,
            E = 8,
            F = 9,
            G = 10,
            H = 11,
            I = 12,
            J = 13,
            K = 14,
            L = 15,
            M = 16,
            N = 17,
            O = 18,
            P = 19,
            Q = 20,
            R = 21,
            S = 22,
            T = 23,
            U = 24,
            V = 25,
            W = 26,
            X = 27,
            Y = 28,
            Z = 29,

            NUM1 = 30,
            NUM2 = 31,
            NUM3 = 32,
            NUM4 = 33,
            NUM5 = 34,
            NUM6 = 35,
            NUM7 = 36,
            NUM8 = 37,
            NUM9 = 38,
            NUM0 = 39,

            RETURN = 40,
            ESCAPE = 41,
            BACKSPACE = 42,
            TAB = 43,
            SPACE = 44,

            MINUS = 45,
            EQUALS = 46,
            LEFTBRACKET = 47,
            RIGHTBRACKET = 48,
            BACKSLASH = 49,
            NONUSHASH = 50,
            SEMICOLON = 51,
            APOSTROPHE = 52,
            GRAVE = 53,
            COMMA = 54,
            PERIOD = 55,
            SLASH = 56,

            CAPSLOCK = 57,

            F1 = 58,
            F2 = 59,
            F3 = 60,
            F4 = 61,
            F5 = 62,
            F6 = 63,
            F7 = 64,
            F8 = 65,
            F9 = 66,
            F10 = 67,
            F11 = 68,
            F12 = 69,

            PRINTSCREEN = 70,
            SCROLLLOCK = 71,
            PAUSE = 72,
            INSERT = 73,
            HOME = 74,
            PAGEUP = 75,
            DELETE = 76,
            END = 77,
            PAGEDOWN = 78,
            RIGHT = 79,
            LEFT = 80,
            DOWN = 81,
            UP = 82,

            NUMLOCKCLEAR = 83,
            KP_DIVIDE = 84,
            KP_MULTIPLY = 85,
            KP_MINUS = 86,
            KP_PLUS = 87,
            KP_ENTER = 88,
            KP_1 = 89,
            KP_2 = 90,
            KP_3 = 91,
            KP_4 = 92,
            KP_5 = 93,
            KP_6 = 94,
            KP_7 = 95,
            KP_8 = 96,
            KP_9 = 97,
            KP_0 = 98,
            KP_PERIOD = 99,

            NONUSBACKSLASH = 100,
            APPLICATION = 101,
            POWER = 102,
            KP_EQUALS = 103,
            F13 = 104,
            F14 = 105,
            F15 = 106,
            F16 = 107,
            F17 = 108,
            F18 = 109,
            F19 = 110,
            F20 = 111,
            F21 = 112,
            F22 = 113,
            F23 = 114,
            F24 = 115,
            EXECUTE = 116,
            HELP = 117,
            MENU = 118,
            SELECT = 119,
            STOP = 120,    /**< AC Stop */
            AGAIN = 121,   /**< AC Redo/Repeat */
            UNDO = 122,    /**< AC Undo */
            CUT = 123,     /**< AC Cut */
            COPY = 124,    /**< AC Copy */
            PASTE = 125,   /**< AC Paste */
            FIND = 126,    /**< AC Find */
            MUTE = 127,
            VOLUMEUP = 128,
            VOLUMEDOWN = 129,

            KP_COMMA = 133,
            KP_EQUALSAS400 = 134,

            INTERNATIONAL1 = 135,
            INTERNATIONAL2 = 136,
            INTERNATIONAL3 = 137, /**< Yen */
            INTERNATIONAL4 = 138,
            INTERNATIONAL5 = 139,
            INTERNATIONAL6 = 140,
            INTERNATIONAL7 = 141,
            INTERNATIONAL8 = 142,
            INTERNATIONAL9 = 143,
            LANG1 = 144, /**< Hangul/English toggle */
            LANG2 = 145, /**< Hanja conversion */
            LANG3 = 146, /**< Katakana */
            LANG4 = 147, /**< Hiragana */
            LANG5 = 148, /**< Zenkaku/Hankaku */
            LANG6 = 149, /**< reserved */
            LANG7 = 150, /**< reserved */
            LANG8 = 151, /**< reserved */
            LANG9 = 152, /**< reserved */

            ALTERASE = 153,    /**< Erase-Eaze */
            SYSREQ = 154,
            CANCEL = 155,      /**< AC Cancel */
            CLEAR = 156,
            PRIOR = 157,
            RETURN2 = 158,
            SEPARATOR = 159,
            OUT = 160,
            OPER = 161,
            CLEARAGAIN = 162,
            CRSEL = 163,
            EXSEL = 164,

            KP_00 = 176,
            KP_000 = 177,
            THOUSANDSSEPARATOR = 178,
            DECIMALSEPARATOR = 179,
            CURRENCYUNIT = 180,
            CURRENCYSUBUNIT = 181,
            KP_LEFTPAREN = 182,
            KP_RIGHTPAREN = 183,
            KP_LEFTBRACE = 184,
            KP_RIGHTBRACE = 185,
            KP_TAB = 186,
            KP_BACKSPACE = 187,
            KP_A = 188,
            KP_B = 189,
            KP_C = 190,
            KP_D = 191,
            KP_E = 192,
            KP_F = 193,
            KP_XOR = 194,
            KP_POWER = 195,
            KP_PERCENT = 196,
            KP_LESS = 197,
            KP_GREATER = 198,
            KP_AMPERSAND = 199,
            KP_DBLAMPERSAND = 200,
            KP_VERTICALBAR = 201,
            KP_DBLVERTICALBAR = 202,
            KP_COLON = 203,
            KP_HASH = 204,
            KP_SPACE = 205,
            KP_AT = 206,
            KP_EXCLAM = 207,
            KP_MEMSTORE = 208,
            KP_MEMRECALL = 209,
            KP_MEMCLEAR = 210,
            KP_MEMADD = 211,
            KP_MEMSUBTRACT = 212,
            KP_MEMMULTIPLY = 213,
            KP_MEMDIVIDE = 214,
            KP_PLUSMINUS = 215,
            KP_CLEAR = 216,
            KP_CLEARENTRY = 217,
            KP_BINARY = 218,
            KP_OCTAL = 219,
            KP_DECIMAL = 220,
            KP_HEXADECIMAL = 221,

            LCTRL = 224,
            LSHIFT = 225,
            LALT = 226, /**< alt, option */
            LGUI = 227, /**< windows, command (apple), meta */
            RCTRL = 228,
            RSHIFT = 229,
            RALT = 230, /**< alt gr, option */
            RGUI = 231, /**< windows, command (apple), meta */

            MODE = 257,

            AUDIONEXT = 258,
            AUDIOPREV = 259,
            AUDIOSTOP = 260,
            AUDIOPLAY = 261,
            AUDIOMUTE = 262,
            MEDIASELECT = 263,
            WWW = 264,             /**< AL Internet Browser */
            MAIL = 265,
            CALCULATOR = 266,      /**< AL Calculator */
            COMPUTER = 267,
            AC_SEARCH = 268,       /**< AC Search */
            AC_HOME = 269,         /**< AC Home */
            AC_BACK = 270,         /**< AC Back */
            AC_FORWARD = 271,      /**< AC Forward */
            AC_STOP = 272,         /**< AC Stop */
            AC_REFRESH = 273,      /**< AC Refresh */
            AC_BOOKMARKS = 274,    /**< AC Bookmarks */



            BRIGHTNESSDOWN = 275,
            BRIGHTNESSUP = 276,
            DISPLAYSWITCH = 277,
            KBDILLUMTOGGLE = 278,
            KBDILLUMDOWN = 279,
            KBDILLUMUP = 280,
            EJECT = 281,
            SLEEP = 282,           /**< SC System Sleep */

            APP1 = 283,
            APP2 = 284,


            AUDIOREWIND = 285,
            AUDIOFASTFORWARD = 286,

            SOFTLEFT = 287,
            SOFTRIGHT = 288,
            CALL = 289,
            ENDCALL = 290,

            NUM_SCANCODES = 512
        };

        public static bool IsKeyPressed(KeyCode keycode)
        {
            return IsKeyPressed_Native(keycode);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsKeyPressed_Native(KeyCode key);
    }
}
