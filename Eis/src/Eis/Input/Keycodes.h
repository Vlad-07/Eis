#pragma once

namespace Eis
{
	typedef enum class KeyCode : uint16_t
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
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}


// From glfw3.h
#define EIS_KEY_SPACE			::Eis::Key::Space
#define EIS_KEY_APOSTROPHE		::Eis::Key::Apostrophe		/* ' */
#define EIS_KEY_COMMA			::Eis::Key::Comma			/* , */
#define EIS_KEY_MINUS			::Eis::Key::Minus			/* - */
#define EIS_KEY_PERIOD			::Eis::Key::Period			/* . */
#define EIS_KEY_SLASH			::Eis::Key::Slash			/* / */
#define EIS_KEY_0				::Eis::Key::D0
#define EIS_KEY_1				::Eis::Key::D1
#define EIS_KEY_2				::Eis::Key::D2
#define EIS_KEY_3				::Eis::Key::D3
#define EIS_KEY_4				::Eis::Key::D4
#define EIS_KEY_5				::Eis::Key::D5
#define EIS_KEY_6				::Eis::Key::D6
#define EIS_KEY_7				::Eis::Key::D7
#define EIS_KEY_8				::Eis::Key::D8
#define EIS_KEY_9				::Eis::Key::D9
#define EIS_KEY_SEMICOLON		::Eis::Key::Semicolon		/* ; */
#define EIS_KEY_EQUAL			::Eis::Key::Equal			/* = */
#define EIS_KEY_A				::Eis::Key::A
#define EIS_KEY_B				::Eis::Key::B
#define EIS_KEY_C				::Eis::Key::C
#define EIS_KEY_D				::Eis::Key::D
#define EIS_KEY_E				::Eis::Key::E
#define EIS_KEY_F				::Eis::Key::F
#define EIS_KEY_G				::Eis::Key::G
#define EIS_KEY_H				::Eis::Key::H
#define EIS_KEY_I				::Eis::Key::I
#define EIS_KEY_J				::Eis::Key::J
#define EIS_KEY_K				::Eis::Key::K
#define EIS_KEY_L				::Eis::Key::L
#define EIS_KEY_M				::Eis::Key::M
#define EIS_KEY_N				::Eis::Key::N
#define EIS_KEY_O				::Eis::Key::O
#define EIS_KEY_P				::Eis::Key::P
#define EIS_KEY_Q				::Eis::Key::Q
#define EIS_KEY_R				::Eis::Key::R
#define EIS_KEY_S				::Eis::Key::S
#define EIS_KEY_T				::Eis::Key::T
#define EIS_KEY_U				::Eis::Key::U
#define EIS_KEY_V				::Eis::Key::V
#define EIS_KEY_W				::Eis::Key::W
#define EIS_KEY_X				::Eis::Key::X
#define EIS_KEY_Y				::Eis::Key::Y
#define EIS_KEY_Z				::Eis::Key::Z
#define EIS_KEY_LEFT_BRACKET	::Eis::Key::LeftBracket		/* [ */
#define EIS_KEY_BACKSLASH		::Eis::Key::Backslash		/* \ */
#define EIS_KEY_RIGHT_BRACKET	::Eis::Key::RightBracket	/* ] */
#define EIS_KEY_GRAVE_ACCENT	::Eis::Key::GraveAccent		/* ` */
#define EIS_KEY_WORLD_1			::Eis::Key::World1			/* non-US #1 */
#define EIS_KEY_WORLD_2			::Eis::Key::World2			/* non-US #2 */

/* Function keys */
#define EIS_KEY_ESCAPE			::Eis::Key::Escape
#define EIS_KEY_ENTER			::Eis::Key::Enter
#define EIS_KEY_TAB				::Eis::Key::Tab
#define EIS_KEY_BACKSPACE		::Eis::Key::Backspace
#define EIS_KEY_INSERT			::Eis::Key::Insert
#define EIS_KEY_DELETE			::Eis::Key::Delete
#define EIS_KEY_RIGHT			::Eis::Key::Right
#define EIS_KEY_LEFT			::Eis::Key::Left
#define EIS_KEY_DOWN			::Eis::Key::Down
#define EIS_KEY_UP				::Eis::Key::Up
#define EIS_KEY_PAGE_UP			::Eis::Key::PageUp
#define EIS_KEY_PAGE_DOWN		::Eis::Key::PageDown
#define EIS_KEY_HOME			::Eis::Key::Home
#define EIS_KEY_END				::Eis::Key::End
#define EIS_KEY_CAPS_LOCK		::Eis::Key::CapsLock
#define EIS_KEY_SCROLL_LOCK		::Eis::Key::ScrollLock
#define EIS_KEY_NUM_LOCK		::Eis::Key::NumLock
#define EIS_KEY_PRINT_SCREEN	::Eis::Key::PrintScreen
#define EIS_KEY_PAUSE			::Eis::Key::Pause
#define EIS_KEY_F1				::Eis::Key::F1
#define EIS_KEY_F2				::Eis::Key::F2
#define EIS_KEY_F3				::Eis::Key::F3
#define EIS_KEY_F4				::Eis::Key::F4
#define EIS_KEY_F5				::Eis::Key::F5
#define EIS_KEY_F6				::Eis::Key::F6
#define EIS_KEY_F7				::Eis::Key::F7
#define EIS_KEY_F8				::Eis::Key::F8
#define EIS_KEY_F9				::Eis::Key::F9
#define EIS_KEY_F10				::Eis::Key::F10
#define EIS_KEY_F11				::Eis::Key::F11
#define EIS_KEY_F12				::Eis::Key::F12
#define EIS_KEY_F13				::Eis::Key::F13
#define EIS_KEY_F14				::Eis::Key::F14
#define EIS_KEY_F15				::Eis::Key::F15
#define EIS_KEY_F16				::Eis::Key::F16
#define EIS_KEY_F17				::Eis::Key::F17
#define EIS_KEY_F18				::Eis::Key::F18
#define EIS_KEY_F19				::Eis::Key::F19
#define EIS_KEY_F20				::Eis::Key::F20
#define EIS_KEY_F21				::Eis::Key::F21
#define EIS_KEY_F22				::Eis::Key::F22
#define EIS_KEY_F23				::Eis::Key::F23
#define EIS_KEY_F24				::Eis::Key::F24
#define EIS_KEY_F25				::Eis::Key::F25

/* Keypad */
#define EIS_KEY_KP_0			::Eis::Key::KP0
#define EIS_KEY_KP_1			::Eis::Key::KP1
#define EIS_KEY_KP_2			::Eis::Key::KP2
#define EIS_KEY_KP_3			::Eis::Key::KP3
#define EIS_KEY_KP_4			::Eis::Key::KP4
#define EIS_KEY_KP_5			::Eis::Key::KP5
#define EIS_KEY_KP_6			::Eis::Key::KP6
#define EIS_KEY_KP_7			::Eis::Key::KP7
#define EIS_KEY_KP_8			::Eis::Key::KP8
#define EIS_KEY_KP_9			::Eis::Key::KP9
#define EIS_KEY_KP_DECIMAL		::Eis::Key::KPDecimal
#define EIS_KEY_KP_DIVIDE		::Eis::Key::KPDivide
#define EIS_KEY_KP_MULTIPLY		::Eis::Key::KPMultiply
#define EIS_KEY_KP_SUBTRACT		::Eis::Key::KPSubtract
#define EIS_KEY_KP_ADD			::Eis::Key::KPAdd
#define EIS_KEY_KP_ENTER		::Eis::Key::KPEnter
#define EIS_KEY_KP_EQUAL		::Eis::Key::KPEqual

#define EIS_KEY_LEFT_SHIFT		::Eis::Key::LeftShift
#define EIS_KEY_LEFT_CONTROL	::Eis::Key::LeftControl
#define EIS_KEY_LEFT_ALT		::Eis::Key::LeftAlt
#define EIS_KEY_LEFT_SUPER		::Eis::Key::LeftSuper
#define EIS_KEY_RIGHT_SHIFT		::Eis::Key::RightShift
#define EIS_KEY_RIGHT_CONTROL	::Eis::Key::RightControl
#define EIS_KEY_RIGHT_ALT		::Eis::Key::RightAlt
#define EIS_KEY_RIGHT_SUPER		::Eis::Key::RightSuper
#define EIS_KEY_MENU			::Eis::Key::Menu