#include "Eispch.h"

#include "Eis/Input/Input.h"

#if defined(EIS_PLATFORM_WINDOWS) || defined(EIS_PLATFORM_WEB)
    #include "Platform/OpenGL/GLFWInput.h"
#endif


namespace Eis
{
    Scope<Input> Input::s_Instance = Input::Create();

    Scope<Input> Eis::Input::Create()
    {
#if defined(EIS_PLATFORM_WINDOWS) || defined(EIS_PLATFORM_WEB)
        return CreateScope<GLFWInput>();
#else
        #error Unknown platform!
        return nullptr;
#endif
    }
}