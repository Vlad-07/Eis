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

		void Begin();
		void End();
	};
}