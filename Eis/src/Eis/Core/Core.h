#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define EIS_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
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
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection


// Debugging tools
#ifdef EIS_DEBUG
	#ifdef EIS_PLATFORM_WINDOWS
		#define EIS_DEBUGBREAK() __debugbreak();
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif

	#define EIS_ENABLE_ASSERTS
	#define EIS_PROFILE
	#define EIS_PROFILE_RENDERER
#else
	#define EIS_DEBUGBREAK()
#endif

#ifdef EIS_ENABLE_ASSERTS
	#define EIS_ASSERT(x, ...) {if(!(x)) { EIS_ERROR("Assertion Failed: {0}", __VA_ARGS__); EIS_DEBUGBREAK(); } }
	#define EIS_CORE_ASSERT(x, ...) {if(!(x)) { EIS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); EIS_DEBUGBREAK(); } }
#else
	#define EIS_ASSERT(x, ...)
	#define EIS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x) // Used for event categories

#define EIS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1) // Used for event functions


// Wraps for smart pointers
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