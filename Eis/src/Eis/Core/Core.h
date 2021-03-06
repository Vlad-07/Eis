#pragma once

#include <memory>

#ifdef EIS_PLATFORM_WINDOWS
#ifdef EIS_DYNAMIC_LINK // Used in previous builds to a .dll file
	#ifdef EIS_BUILD_DLL
		#define EIS_API __declspec(dllexport)
	#else
		#define EIS_API __declspec(dllimport)
	#endif
#else
	#define EIS_API
#endif
#else
	#error Eis supports only windows
#endif

#ifdef EIS_DEBUG
	#define EIS_ENABLE_ASSERTS
#endif

#ifdef EIS_ENABLE_ASSERTS
	#define EIS_ASSERT(x, ...) {if(!(x)) { EIS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define EIS_CORE_ASSERT(x, ...) {if(!(x)) { EIS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
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

	template<typename T>
	using Ref = std::shared_ptr<T>;
}