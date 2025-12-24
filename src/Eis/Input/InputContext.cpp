#include "Eispch.h"
#include "InputContext.h"

#include "Platform/OpenGL/GLFWInput.h"


namespace Eis
{
	Scope<InputContext> InputContext::Create(void* window)
	{
		switch (Platform::GetPlatform())
		{
		case Platform::PlatformType::WINDOWS:
		case Platform::PlatformType::WEB:
			return CreateScope<GLFWInput>(window);

		default:
			EIS_CORE_ASSERT(false, "Invalid platform: {}", (uint8_t)Platform::GetPlatform());
			return nullptr;
		}
	}
}