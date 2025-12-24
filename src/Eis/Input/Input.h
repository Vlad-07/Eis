#pragma once

#include <glm/glm.hpp>

#include "Eis/Core/Core.h"
#include "Eis/Input/Keycodes.h"
#include "Eis/Input/MouseCodes.h"


namespace Eis
{
	class InputContext;

	// Pimpl (should use oo factory?)

	class Input
	{
	public:
		Input();
		~Input();
		Input& operator=(const Input&) = delete;

		// Only called by engine
		void Init(void* window);


		bool IsKeyPressed(KeyCode key) const;

		bool IsMouseButtonPressed(MouseCode button) const;
		glm::vec2 GetMousePos() const;

		EIS_DEPRECATED
		float GetMouseX() const;
		EIS_DEPRECATED
		float GetMouseY() const;

	private:
		Scope<InputContext> m_Context;
	};
}