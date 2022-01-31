#pragma once

#ifdef EIS_PLATFORM_WINDOWS
	#ifdef EIS_BUILD_DLL
		#define EIS_API __declspec(dllexport)
	#else
		#define EIS_API __declspec(dllimport)
	
	#endif
#else
	#error Eis supports only windows
#endif

#ifdef EIS_ENABLE_ASSERTS
	#define EIS_ASSERT(x, ...) {if(!(x)) { EIS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define EIS_CORE_ASSERT(x, ...) {if(!(x)) { EIS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define EIS_ASSERT(x, ...)
	#define EIS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)