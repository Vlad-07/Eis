#include "Eispch.h"
#include "Platform.h"


namespace Eis
{
	Platform::PlatformType Platform::s_Platform =
#ifdef EIS_PLATFORM_WINDOWS
		Platform::PlatformType::WINDOWS;
#elif defined(EIS_PLATFORM_WEB)
		Platform::PlatformType::WEB;
#endif
}