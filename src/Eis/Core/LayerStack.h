#pragma once

#include <vector>

#include "Eis/Core/Core.h"
#include "Eis/Core/Layer.h"


namespace Eis
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack() { Clear(); }

		void PushLayer(Scope<Layer> layer);
		void PushOverlay(Scope<Layer> overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		void Clear();


		std::vector<Scope<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<Scope<Layer>>::iterator end() { return m_Layers.end(); }

		std::vector<Scope<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Scope<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Scope<Layer>>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Scope<Layer>>::const_iterator end()	const { return m_Layers.end(); }

		std::vector<Scope<Layer>>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Scope<Layer>>::const_reverse_iterator rend() const { return m_Layers.rend(); }

	private:
		std::vector<Scope<Layer>> m_Layers;
		uint32_t m_LayerInsertIndex = 0;
	};
}