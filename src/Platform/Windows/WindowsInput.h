#pragma once

#include "Eis/Input/Input.h"
#include <glm/glm.hpp>

namespace Eis
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(KeyCode keycode) override;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
		virtual glm::vec2 GetMousePosImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}