#pragma once

#include <vector>

#include "Eis/Core/Layer.h"


namespace Eis
{
	class LayerLib
	{
	public:
		LayerLib() = default;
		~LayerLib() = default;

		void SetContext(const EisContext& context) { m_Context = context; }

		void RegisterLayer(const Layer::Factory& factory, const std::string& name);

		Scope<Layer> MakeLayer(const std::string& name) const;
		Scope<Layer> MakeLayer(int32_t id) const;

		int32_t GetLayerId(const std::string& name) const;

	private:
		std::vector<std::pair<Layer::Factory, std::string>> m_LayerFactories;
		EisContext m_Context;
	};
}