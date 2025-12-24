#pragma once

#include <glm/glm.hpp>

#include "Eis/Core/Platform.h"
#include "Eis/Input/KeyCodes.h"
#include "Eis/Input/MouseCodes.h"


namespace Eis
{
	class InputContext
	{
	public:
		InputContext() = default;
		virtual ~InputContext() = default;


		virtual bool IsKeyPressedImpl(KeyCode key) const = 0;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) const = 0;
		virtual glm::vec2 GetMousePosImpl() const = 0;


		static Scope<InputContext> Create(void* window);
	};
}