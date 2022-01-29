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

#define BIT(x) (1 << x)