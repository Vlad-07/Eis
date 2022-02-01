#pragma once

#include "Eis/Layer.h"
#include "Eis/Events/KeyEvent.h"
#include "Eis/Events/MouseEvent.h"
#include "Eis/Events/ApplicationEvent.h"

namespace Eis
{
	class EIS_API ImGuiLayer : public Layer
	{
	private:
		float m_Time = 0;

	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate() override;
		void OnEvent(Event& event) override;

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
		bool OnMouseMovedEvent(MouseMovedEvent& event);
		bool OnMouseMouseScrolledEvent(MouseScrolledEvent& event);
		bool OnKeyPressedEvent (KeyPressedEvent& event);
		bool OnKeyReleasedEvent (KeyReleasedEvent& event);
		bool OnKeyTypedEvent(KeyTypedEvent& event);
		bool OnWindowResizedEvent(WindowResizeEvent& event);
	};
}