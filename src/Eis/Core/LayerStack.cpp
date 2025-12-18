#include "Eispch.h"
#include "LayerStack.h"


namespace Eis
{
	void LayerStack::PushLayer(Scope<Layer> layer)
	{
		layer->Attach();
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Scope<Layer> overlay)
	{
		overlay->Attach();
		m_Layers.emplace_back(std::move(overlay));
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find_if(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex,
								[&](const Scope<Layer>& l) { return l.get() == layer; });
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			layer->Detach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find_if(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(),
								[&](const Scope<Layer>& o) { return o.get() == overlay; });
		if (it != m_Layers.end())
		{
			overlay->Detach();
			m_Layers.erase(it);
		}
	}


	void LayerStack::Clear()
	{
		for (Scope<Layer>& layer : m_Layers)
			layer->Detach();

		m_Layers.clear();
		m_LayerInsertIndex = 0;
	}
}