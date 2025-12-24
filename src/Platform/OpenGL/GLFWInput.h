#pragma once

#include <glm/glm.hpp>
#include "Eis/Input/InputContext.h"


struct GLFWwindow;

namespace Eis
{
	class GLFWInput : public InputContext
	{
	public:
		GLFWInput(void* window) : m_Window(static_cast<GLFWwindow*>(window)) {}
		virtual ~GLFWInput() = default;


		virtual bool IsKeyPressedImpl(KeyCode keycode) const override;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) const override;
		virtual glm::vec2 GetMousePosImpl() const override;

	private:
		GLFWwindow* m_Window = nullptr;
	};
}