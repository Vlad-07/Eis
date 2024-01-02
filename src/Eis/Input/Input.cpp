#include "Eispch.h"

#include "Eis/Input/Input.h"

#ifdef EIS_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsInput.h"
#endif

namespace Eis
{
    Scope<Input> Input::s_Instance = Input::Create();

    Scope<Input> Eis::Input::Create()
    {
#ifdef EIS_PLATFORM_WINDOWS
        return CreateScope<WindowsInput>();
#else
        EIS_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
}