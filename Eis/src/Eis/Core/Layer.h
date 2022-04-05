#pragma once

#include "Eispch.h"
#include "Core.h"
#include "../Events/Event.h"
#include "Eis/Core/TimeStep.h"

namespace Eis
{
	class EIS_API Layer
	{
	protected:
		std::string m_DebugName;

	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	};
}