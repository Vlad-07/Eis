#pragma once

#include "Core.h"
#include "Layer.h"

#include <vector>


namespace Eis
{
	class EIS_API LayerStack // A stack that events are propagated trough, and updated/rendered in order
	{
	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;

	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	};
}