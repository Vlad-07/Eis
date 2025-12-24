#pragma once

#include <ostream>


namespace Eis
{
	enum class MouseCode : uint16_t
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

		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2,
		ButtonLast = Button7
	};

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}


// From glfw3.h
#define EIS_MOUSE_BUTTON_0      ::Eis::MouseCode::Button0
#define EIS_MOUSE_BUTTON_1      ::Eis::MouseCode::Button1
#define EIS_MOUSE_BUTTON_2      ::Eis::MouseCode::Button2
#define EIS_MOUSE_BUTTON_3      ::Eis::MouseCode::Button3
#define EIS_MOUSE_BUTTON_4      ::Eis::MouseCode::Button4
#define EIS_MOUSE_BUTTON_5      ::Eis::MouseCode::Button5
#define EIS_MOUSE_BUTTON_6      ::Eis::MouseCode::Button6
#define EIS_MOUSE_BUTTON_7      ::Eis::MouseCode::Button7
#define EIS_MOUSE_BUTTON_LEFT   ::Eis::MouseCode::ButtonLeft
#define EIS_MOUSE_BUTTON_RIGHT  ::Eis::MouseCode::ButtonRight
#define EIS_MOUSE_BUTTON_MIDDLE ::Eis::MouseCode::ButtonMiddle
#define EIS_MOUSE_BUTTON_LAST   ::Eis::MouseCode::ButtonLast