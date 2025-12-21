#pragma once

#include <memory>


// Configuration check
#if !(defined(EIS_DEBUG) || defined(EIS_RELEASE))
	#error Configuration not specified (EIS_DEBUG/EIS_RELEASE)
#endif

// Platform detection
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#ifndef EIS_PLATFORM_WINDOWS
			#define EIS_PLATFORM_WINDOWS
		#endif
	#else
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define EIS_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define EIS_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define EIS_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define EIS_PLATFORM_LINUX
	#error "Linux is not supported!"
#elif defined(__EMSCRIPTEN__)
	#define EIS_PLATFORM_WEB
	#include <emscripten.h>
#else
	#error "Unknown platform!"
#endif


// Debugging tools
#ifdef EIS_DEBUG
	#define EIS_ENABLE_ASSERTS

	#ifdef EIS_PLATFORM_WINDOWS
		#define EIS_DEBUGBREAK() __debugbreak();
		#define EIS_PROFILE
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

#define EIS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1) // Used for event functions


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