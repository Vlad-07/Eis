#include "Eispch.h"
#include "LayerLib.h"


namespace Eis
{
	void LayerLib::RegisterLayer(const Layer::Factory& factory, const std::string& name)
	{
		m_LayerFactories.emplace_back(factory, name);
	}

	Scope<Layer> LayerLib::MakeLayer(const std::string& name) const
	{
		int32_t id = GetLayerId(name);
		return MakeLayer(id);
	}

	Scope<Layer> LayerLib::MakeLayer(int32_t id) const
	{
		EIS_CORE_ASSERT(m_LayerFactories.size(), "No layers registered!");
		EIS_CORE_ASSERT(0 <= id && id < m_LayerFactories.size(), "Invalid layer id requested: {}!", id);

		const auto& pair = m_LayerFactories[id];
		auto layer = pair.first(pair.second);
		layer->Eis = m_Context;
		return layer;
	}

	int32_t LayerLib::GetLayerId(const std::string& name) const
	{
		int32_t id = 0;
		for (const auto& pair : m_LayerFactories)
		{
			if (name == pair.second)
				return id;
			id++;
		}
		EIS_CORE_ERROR("No layer named {} found!", name);
		return -1;
	}
}