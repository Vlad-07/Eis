#include "Eispch.h"
#include "LayerLib.h"


namespace Eis
{
	void LayerLib::RegisterLayer(const LayerFactory& factory, const std::string& name)
	{
		m_LayerFactories.push_back(NamedFactory{ name, factory });
	}


	Scope<Layer> LayerLib::MakeLayer(const std::string& name, std::optional<Buffer> data) const
	{
		const int32_t id = GetLayerId(name);
		return MakeLayer(id, data);
	}

	Scope<Layer> LayerLib::MakeLayer(int32_t id, std::optional<Buffer> data) const
	{
		EIS_CORE_ASSERT(m_LayerFactories.size(), "No layers registered!");
		EIS_CORE_ASSERT(0 <= id && id < m_LayerFactories.size(), "Invalid layer id requested: {}!", id);

		const auto& NamedFactory = m_LayerFactories[id];
		auto layer = NamedFactory.Factory(data);
		layer->Eis = m_Context;
		return layer;
	}


	int32_t LayerLib::GetLayerId(const std::string& name) const
	{
		int32_t id = 0;
		for (const auto& NamedFactory : m_LayerFactories)
		{
			if (name == NamedFactory.Name)
				return id;
			id++;
		}
		EIS_CORE_ERROR("No layer named {} found!", name);
		return -1;
	}
}