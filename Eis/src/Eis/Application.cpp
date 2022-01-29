#include "Application.h"

#include "Log.h"
#include "Events/ApplicationEvent.h"

namespace Eis
{

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1200, 600);
		EIS_TRACE(e);

		while (true);
	}
}