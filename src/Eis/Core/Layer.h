#pragma once

#include "Eis/Core/Core.h"
#include "Eis/Events/Event.h"


namespace Eis
{
	class Layer
	{
	public:
		Layer(const std::string& debugName = "Layer") : m_DebugName(debugName) {}
		virtual ~Layer() = default;

		virtual void Attach() {}
		virtual void Detach() {}

		virtual void Update() {}
		virtual void FixedUpdate() {}

		virtual void Render() {}
		virtual void ImGuiRender() {}

		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}