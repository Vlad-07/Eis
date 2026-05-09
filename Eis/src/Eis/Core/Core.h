#pragma once

#include <memory>

#include "PlatformDetection.h"


// Configuration check
#if !(defined(EIS_DEBUG) || defined(EIS_RELEASE))
	#error Configuration not specified (EIS_DEBUG/EIS_RELEASE)
#endif


// Debugging tools
#ifdef EIS_DEBUG
	#define EIS_ENABLE_ASSERTS

	#ifdef EIS_PLATFORM_WINDOWS
		#define EIS_DEBUGBREAK() __debugbreak();
//		#define EIS_PROFILE
//		#define EIS_PROFILE_RENDERER
	#else
		#define EIS_DEBUGBREAK()
	#endif
#else
	#define EIS_DEBUGBREAK()
#endif


// Rendering
#ifdef EIS_PLATFORM_WINDOWS
	#define EIS_IMGUI_VIEWPORTS
#endif


// Logging
#ifndef __FUNCSIG__
	#define __FUNCSIG__ __PRETTY_FUNCTION__
#endif

// TODO: this should not be a macro
#ifdef EIS_PLATFORM_WINDOWS
	#define EIS_LOG_FILE
#elif defined(EIS_PLATFORM_WEB)
	#define EIS_LOG_NOFILE
#endif


// Networking
#ifdef EIS_PLATFORM_WINDOWS
	#define EIS_NETWORKING_ENABLE
#endif



#define BIT(x) (1 << x) // Used for event categories

// Used for event functions
#define EIS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


#define EIS_DEPRECATED [[deprecated]]
#define EIS_DEPRECATED_MSG(msg) [[deprecated(msg)]]


// Wrappers for smart pointers
// Code looks nicer + can easily switch to an actual class in the future for an asset manager

namespace Eis
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}