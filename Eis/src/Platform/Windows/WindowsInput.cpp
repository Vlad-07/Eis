#include "Eispch.h"

#include "Eis/Input/Input.h"
#include "Eis/Core/Application.h"

#include <GLFW/glfw3.h>


#ifdef EIS_PLATFORM_WINDOWS


namespace Eis
{
	namespace
	{
		GLFWwindow* GetNativeWindow()
		{
			return static_cast<GLFWwindow*>(Application::GetWindow().GetNativeWindow());
		}
	}


	bool Input::IsKeyPressed(const KeyCode keycode)
	{
		int state = glfwGetKey(GetNativeWindow(), static_cast<int32_t>(keycode));

		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		int state = glfwGetMouseButton(GetNativeWindow(), static_cast<int32_t>(button));

		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePos()
	{
		double xPos, yPos;
		glfwGetCursorPos(GetNativeWindow(), &xPos, &yPos);

		return glm::vec2(static_cast<float>(xPos), static_cast<float>(yPos)) * Application::GetWindow().GetScale();
	}

	float Input::GetMouseX()
	{
		return GetMousePos().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePos().y;
	}
}

#endif // EIS_PLATFORM_WINDOWS