#include "Eispch.h"
#include "GLFWInput.h"

#include "Eis/Core/Application.h"

#include <GLFW/glfw3.h>


namespace Eis
{
	bool GLFWInput::IsKeyPressedImpl(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(keycode));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool GLFWInput::IsMouseButtonPressedImpl(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));

		return state == GLFW_PRESS;
	}

	glm::vec2 GLFWInput::GetMousePosImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return glm::vec2((float)xPos, (float)yPos);
	}

	float GLFWInput::GetMouseXImpl()
	{
		return GetMousePosImpl().x;
	}
	float GLFWInput::GetMouseYImpl()
	{
		return GetMousePosImpl().y;
	}
}