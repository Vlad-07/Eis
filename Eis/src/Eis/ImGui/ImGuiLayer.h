#pragma once

#include "Eis/Core/Layer.h"


namespace Eis
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() : Layer("ImGuiLayer") {}
		virtual ~ImGuiLayer() = default;

		virtual void Attach() override;
		virtual void Detach() override;

		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();

	private:
		bool m_BlockEvents{};
	};
}