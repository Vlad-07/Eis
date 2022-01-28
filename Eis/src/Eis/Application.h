#pragma once

#include "Core.h"

namespace Eis
{
	class EIS_API Application
	{


	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// Is defined in CLIENT:
	Application* CreateApplication();
}