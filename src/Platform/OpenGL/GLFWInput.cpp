#include "Eispch.h"
#include "GLFWInput.h"

#include "Eis/Core/Application.h"

#include <GLFW/glfw3.h>


namespace Eis
{
	bool GLFWInput::IsKeyPressedImpl(KeyCode keycode) const
	{
		auto state = glfwGetKey(m_Window, static_cast<int32_t>(keycode));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool GLFWInput::IsMouseButtonPressedImpl(MouseCode button) const
	{
		auto state = glfwGetMouseButton(m_Window, static_cast<int32_t>(button));

		return state == GLFW_PRESS;
	}

	glm::vec2 GLFWInput::GetMousePosImpl() const
	{
		double xPos, yPos;
		glfwGetCursorPos(m_Window, &xPos, &yPos);

		return glm::vec2(static_cast<float>(xPos), static_cast<float>(yPos)) * Application::GetWindow().GetScale();
	}
}