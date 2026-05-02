#include "Eispch.h"

#include "Eis/Input/Input.h"
#include "Eis/Core/Application.h"

#ifdef EIS_PLATFORM_WEB

#include <GLFW/emscripten_glfw3.h>


namespace Eis
{
	static GLFWwindow* GetNativeWindow()
	{
		return static_cast<GLFWwindow*>(Application::GetWindow().GetNativeWindow());
	}


	bool Input::IsKeyPressed(KeyCode keycode)
	{
		auto state = glfwGetKey(GetNativeWindow(), static_cast<int32_t>(keycode));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto state = glfwGetMouseButton(GetNativeWindow(), static_cast<int32_t>(button));

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

#endif // EIS_PLATFORM_WEB