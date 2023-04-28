#pragma once

#include "Eis/Core/Layer.h"

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
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	};
}