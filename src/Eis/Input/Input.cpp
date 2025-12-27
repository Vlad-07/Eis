#include "Eispch.h"
#include "Input.h"

#include "Eis/Input/InputContext.h"


namespace Eis
{
	Input::Input() = default;
	Input::~Input() = default;

	void Input::Init(void* window)
	{
		EIS_CORE_ASSERT(window, "Invalid window handle!");
		m_Context = InputContext::Create(window);
	}


	bool Input::IsKeyPressed(KeyCode key) const
	{
		EIS_CORE_ASSERT(m_Context, "Input context not initialized!");
		return m_Context->IsKeyPressedImpl(key);
	}

	bool Input::IsMouseButtonPressed(MouseCode button) const
	{
		EIS_CORE_ASSERT(m_Context, "Input context not initialized!");
		return m_Context->IsMouseButtonPressedImpl(button);
	}

	glm::vec2 Input::GetMousePos() const
	{
		EIS_CORE_ASSERT(m_Context, "Input context not initialized!");
		return m_Context->GetMousePosImpl();
	}

	float Input::GetMouseX() const
	{
		return m_Context->GetMousePosImpl().x;
	}
	float Input::GetMouseY() const
	{
		return m_Context->GetMousePosImpl().y;
	}
}