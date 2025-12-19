#pragma once

#include <functional>

#include "Eis/Core/Core.h"
#include "Eis/Events/Event.h"


namespace Eis
{
	class Layer
	{
	public:
		using Factory = std::function<Scope<Layer>(const std::string& layerName)>;

	public:
		Layer(const std::string& name = "Layer") : m_Name(name) {}
		virtual ~Layer() = default;

		virtual void Attach() {}
		virtual void Detach() {}

		virtual void Update() {}
		virtual void FixedUpdate() {}

		virtual void Render() {}
		virtual void ImGuiRender() {}

		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_Name; }

	protected:
		std::string m_Name;
	};
}