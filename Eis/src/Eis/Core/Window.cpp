#include "Eispch.h"
#include "Window.h"

#ifdef EIS_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#elif defined(EIS_PLATFORM_WEB)
	#include "Platform/Web/WebWindow.h"
#endif


namespace Eis
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef EIS_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#elif defined(EIS_PLATFORM_WEB)
		return CreateScope<WebWindow>(props);
#else
		#error Unknown platform!
		return nullptr;
#endif
	}
}