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

		virtual void Attach() override;
		virtual void Detach() override;

		void Begin();
		void End();
	};
}