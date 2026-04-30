#pragma once

#include <glm/glm.hpp>

#include "Eis/Core/Core.h"
#include "Eis/Input/Keycodes.h"
#include "Eis/Input/MouseCodes.h"


namespace Eis
{
	class Input
	{
	public:
		Input() = delete;

		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePos();

		static float GetMouseX();
		static float GetMouseY();
	};
}