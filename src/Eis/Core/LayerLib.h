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

		void RegisterLayer(const Layer::Factory& factory, const std::string& name);

		Scope<Layer> MakeLayer(uint8_t id) const;
		Scope<Layer> MakeLayer(const std::string& name) const;

		int32_t GetLayerId(const std::string& name) const;

	private:
		std::vector<std::pair<Layer::Factory, std::string>> m_LayerFactories;
	};
}