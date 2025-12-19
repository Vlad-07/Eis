#include "Eispch.h"
#include "LayerLib.h"


namespace Eis
{
	void LayerLib::RegisterLayer(const Layer::Factory& factory, const std::string& name)
	{
		m_LayerFactories.emplace_back(factory, name);
	}

	Scope<Layer> LayerLib::MakeLayer(uint8_t id) const
	{
		EIS_CORE_ASSERT(m_LayerFactories.size(), "No layers registered!");
		EIS_CORE_ASSERT(id < m_LayerFactories.size(), "Invalid layer id requested: {}!", id);

		const auto& pair = m_LayerFactories[id];
		return pair.first(pair.second);
	}

	Scope<Layer> LayerLib::MakeLayer(const std::string& name) const
	{
		EIS_CORE_ASSERT(m_LayerFactories.size(), "No layers registered!");

		for (auto&[factory, n] : m_LayerFactories)
		{
			if (name == n)
				return factory(n);
		}
		EIS_CORE_CRITICAL("No layer named {} found!", name);
		return m_LayerFactories[0].first("");
	}

	int32_t LayerLib::GetLayerId(const std::string& name) const
	{
		int i = 0;
		for (auto& [factory, n] : m_LayerFactories)
		{
			if (name == n)
				return i;
			i++;
		}
		EIS_CORE_ERROR("No layer named {} found!", name);
		return -1;
	}
}