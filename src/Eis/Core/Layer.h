#pragma once

#include <optional>
#include <functional>

#include "Eis/Core/Core.h"
#include "Eis/Core/Buffer.h"
#include "Eis/Core/EisContext.h"
#include "Eis/Events/Event.h"


namespace Eis
{
	class Layer
	{
	public:
		Layer(const std::string& name) : m_Name(name) {}
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
		EisContext Eis{};

		friend class LayerLib;
	};
}