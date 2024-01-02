#pragma once

#include "Eis/Core/Layer.h"

#include "Eis/Events/KeyEvent.h"
#include "Eis/Events/MouseEvent.h"
#include "Eis/Events/ApplicationEvent.h"

namespace Eis
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	};
}