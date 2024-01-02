#pragma once

namespace Eis
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}


// From glfw3.h
#define EIS_MOUSE_BUTTON_0      ::Eis::Mouse::Button0
#define EIS_MOUSE_BUTTON_1      ::Eis::Mouse::Button1
#define EIS_MOUSE_BUTTON_2      ::Eis::Mouse::Button2
#define EIS_MOUSE_BUTTON_3      ::Eis::Mouse::Button3
#define EIS_MOUSE_BUTTON_4      ::Eis::Mouse::Button4
#define EIS_MOUSE_BUTTON_5      ::Eis::Mouse::Button5
#define EIS_MOUSE_BUTTON_6      ::Eis::Mouse::Button6
#define EIS_MOUSE_BUTTON_7      ::Eis::Mouse::Button7
#define EIS_MOUSE_BUTTON_LAST   ::Eis::Mouse::ButtonLast
#define EIS_MOUSE_BUTTON_LEFT   ::Eis::Mouse::ButtonLeft
#define EIS_MOUSE_BUTTON_RIGHT  ::Eis::Mouse::ButtonRight
#define EIS_MOUSE_BUTTON_MIDDLE ::Eis::Mouse::ButtonMiddle