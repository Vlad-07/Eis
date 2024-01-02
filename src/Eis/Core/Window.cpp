#include "Eispch.h"
#include "Window.h"

#ifdef EIS_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#endif

namespace Eis
{
    Scope<Window> Eis::Window::Create(const WindowProps& props)
    {
#ifdef EIS_PLATFORM_WINDOWS
        return CreateScope<WindowsWindow>(props);
#else
        EIS_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
}