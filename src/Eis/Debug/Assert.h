#pragma once

#include "Eis/Core/Core.h"
#include "Eis/Core/Log.h"


#ifdef EIS_ENABLE_ASSERTS
	#define EIS_CORE_ASSERT(x, ...) { if(!(x)) { EIS_CORE_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); EIS_DEBUGBREAK(); } }
	#define EIS_ASSERT(x, ...) { if(!(x)) { EIS_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); EIS_DEBUGBREAK(); } }
#else
	#define EIS_CORE_ASSERT(x, ...) ((void)0)
	#define EIS_ASSERT(x, ...) ((void)0)
#endif