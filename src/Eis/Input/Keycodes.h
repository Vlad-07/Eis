#pragma once

#include <ostream>


namespace Eis
{
	enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */

		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */

		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	};

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}


// From glfw3.h

#define EIS_KEY_SPACE			::Eis::KeyCode::Space
#define EIS_KEY_APOSTROPHE		::Eis::KeyCode::Apostrophe		/* ' */
#define EIS_KEY_COMMA			::Eis::KeyCode::Comma			/* , */
#define EIS_KEY_MINUS			::Eis::KeyCode::Minus			/* - */
#define EIS_KEY_PERIOD			::Eis::KeyCode::Period			/* . */
#define EIS_KEY_SLASH			::Eis::KeyCode::Slash			/* / */
#define EIS_KEY_0				::Eis::KeyCode::D0
#define EIS_KEY_1				::Eis::KeyCode::D1
#define EIS_KEY_2				::Eis::KeyCode::D2
#define EIS_KEY_3				::Eis::KeyCode::D3
#define EIS_KEY_4				::Eis::KeyCode::D4
#define EIS_KEY_5				::Eis::KeyCode::D5
#define EIS_KEY_6				::Eis::KeyCode::D6
#define EIS_KEY_7				::Eis::KeyCode::D7
#define EIS_KEY_8				::Eis::KeyCode::D8
#define EIS_KEY_9				::Eis::KeyCode::D9
#define EIS_KEY_SEMICOLON		::Eis::KeyCode::Semicolon		/* ; */
#define EIS_KEY_EQUAL			::Eis::KeyCode::Equal			/* = */
#define EIS_KEY_A				::Eis::KeyCode::A
#define EIS_KEY_B				::Eis::KeyCode::B
#define EIS_KEY_C				::Eis::KeyCode::C
#define EIS_KEY_D				::Eis::KeyCode::D
#define EIS_KEY_E				::Eis::KeyCode::E
#define EIS_KEY_F				::Eis::KeyCode::F
#define EIS_KEY_G				::Eis::KeyCode::G
#define EIS_KEY_H				::Eis::KeyCode::H
#define EIS_KEY_I				::Eis::KeyCode::I
#define EIS_KEY_J				::Eis::KeyCode::J
#define EIS_KEY_K				::Eis::KeyCode::K
#define EIS_KEY_L				::Eis::KeyCode::L
#define EIS_KEY_M				::Eis::KeyCode::M
#define EIS_KEY_N				::Eis::KeyCode::N
#define EIS_KEY_O				::Eis::KeyCode::O
#define EIS_KEY_P				::Eis::KeyCode::P
#define EIS_KEY_Q				::Eis::KeyCode::Q
#define EIS_KEY_R				::Eis::KeyCode::R
#define EIS_KEY_S				::Eis::KeyCode::S
#define EIS_KEY_T				::Eis::KeyCode::T
#define EIS_KEY_U				::Eis::KeyCode::U
#define EIS_KEY_V				::Eis::KeyCode::V
#define EIS_KEY_W				::Eis::KeyCode::W
#define EIS_KEY_X				::Eis::KeyCode::X
#define EIS_KEY_Y				::Eis::KeyCode::Y
#define EIS_KEY_Z				::Eis::KeyCode::Z
#define EIS_KEY_LEFT_BRACKET	::Eis::KeyCode::LeftBracket		/* [ */
#define EIS_KEY_BACKSLASH		::Eis::KeyCode::Backslash		/* \ */
#define EIS_KEY_RIGHT_BRACKET	::Eis::KeyCode::RightBracket	/* ] */
#define EIS_KEY_GRAVE_ACCENT	::Eis::KeyCode::GraveAccent		/* ` */
#define EIS_KEY_WORLD_1			::Eis::KeyCode::World1			/* non-US #1 */
#define EIS_KEY_WORLD_2			::Eis::KeyCode::World2			/* non-US #2 */

/* Function keys */

#define EIS_KEY_ESCAPE			::Eis::KeyCode::Escape
#define EIS_KEY_ENTER			::Eis::KeyCode::Enter
#define EIS_KEY_TAB				::Eis::KeyCode::Tab
#define EIS_KEY_BACKSPACE		::Eis::KeyCode::Backspace
#define EIS_KEY_INSERT			::Eis::KeyCode::Insert
#define EIS_KEY_DELETE			::Eis::KeyCode::Delete
#define EIS_KEY_RIGHT			::Eis::KeyCode::Right
#define EIS_KEY_LEFT			::Eis::KeyCode::Left
#define EIS_KEY_DOWN			::Eis::KeyCode::Down
#define EIS_KEY_UP				::Eis::KeyCode::Up
#define EIS_KEY_PAGE_UP			::Eis::KeyCode::PageUp
#define EIS_KEY_PAGE_DOWN		::Eis::KeyCode::PageDown
#define EIS_KEY_HOME			::Eis::KeyCode::Home
#define EIS_KEY_END				::Eis::KeyCode::End
#define EIS_KEY_CAPS_LOCK		::Eis::KeyCode::CapsLock
#define EIS_KEY_SCROLL_LOCK		::Eis::KeyCode::ScrollLock
#define EIS_KEY_NUM_LOCK		::Eis::KeyCode::NumLock
#define EIS_KEY_PRINT_SCREEN	::Eis::KeyCode::PrintScreen
#define EIS_KEY_PAUSE			::Eis::KeyCode::Pause
#define EIS_KEY_F1				::Eis::KeyCode::F1
#define EIS_KEY_F2				::Eis::KeyCode::F2
#define EIS_KEY_F3				::Eis::KeyCode::F3
#define EIS_KEY_F4				::Eis::KeyCode::F4
#define EIS_KEY_F5				::Eis::KeyCode::F5
#define EIS_KEY_F6				::Eis::KeyCode::F6
#define EIS_KEY_F7				::Eis::KeyCode::F7
#define EIS_KEY_F8				::Eis::KeyCode::F8
#define EIS_KEY_F9				::Eis::KeyCode::F9
#define EIS_KEY_F10				::Eis::KeyCode::F10
#define EIS_KEY_F11				::Eis::KeyCode::F11
#define EIS_KEY_F12				::Eis::KeyCode::F12
#define EIS_KEY_F13				::Eis::KeyCode::F13
#define EIS_KEY_F14				::Eis::KeyCode::F14
#define EIS_KEY_F15				::Eis::KeyCode::F15
#define EIS_KEY_F16				::Eis::KeyCode::F16
#define EIS_KEY_F17				::Eis::KeyCode::F17
#define EIS_KEY_F18				::Eis::KeyCode::F18
#define EIS_KEY_F19				::Eis::KeyCode::F19
#define EIS_KEY_F20				::Eis::KeyCode::F20
#define EIS_KEY_F21				::Eis::KeyCode::F21
#define EIS_KEY_F22				::Eis::KeyCode::F22
#define EIS_KEY_F23				::Eis::KeyCode::F23
#define EIS_KEY_F24				::Eis::KeyCode::F24
#define EIS_KEY_F25				::Eis::KeyCode::F25

/* Keypad */

#define EIS_KEY_KP_0			::Eis::KeyCode::KP0
#define EIS_KEY_KP_1			::Eis::KeyCode::KP1
#define EIS_KEY_KP_2			::Eis::KeyCode::KP2
#define EIS_KEY_KP_3			::Eis::KeyCode::KP3
#define EIS_KEY_KP_4			::Eis::KeyCode::KP4
#define EIS_KEY_KP_5			::Eis::KeyCode::KP5
#define EIS_KEY_KP_6			::Eis::KeyCode::KP6
#define EIS_KEY_KP_7			::Eis::KeyCode::KP7
#define EIS_KEY_KP_8			::Eis::KeyCode::KP8
#define EIS_KEY_KP_9			::Eis::KeyCode::KP9
#define EIS_KEY_KP_DECIMAL		::Eis::KeyCode::KPDecimal
#define EIS_KEY_KP_DIVIDE		::Eis::KeyCode::KPDivide
#define EIS_KEY_KP_MULTIPLY		::Eis::KeyCode::KPMultiply
#define EIS_KEY_KP_SUBTRACT		::Eis::KeyCode::KPSubtract
#define EIS_KEY_KP_ADD			::Eis::KeyCode::KPAdd
#define EIS_KEY_KP_ENTER		::Eis::KeyCode::KPEnter
#define EIS_KEY_KP_EQUAL		::Eis::KeyCode::KPEqual

#define EIS_KEY_LEFT_SHIFT		::Eis::KeyCode::LeftShift
#define EIS_KEY_LEFT_CONTROL	::Eis::KeyCode::LeftControl
#define EIS_KEY_LEFT_ALT		::Eis::KeyCode::LeftAlt
#define EIS_KEY_LEFT_SUPER		::Eis::KeyCode::LeftSuper
#define EIS_KEY_RIGHT_SHIFT		::Eis::KeyCode::RightShift
#define EIS_KEY_RIGHT_CONTROL	::Eis::KeyCode::RightControl
#define EIS_KEY_RIGHT_ALT		::Eis::KeyCode::RightAlt
#define EIS_KEY_RIGHT_SUPER		::Eis::KeyCode::RightSuper
#define EIS_KEY_MENU			::Eis::KeyCode::Menu