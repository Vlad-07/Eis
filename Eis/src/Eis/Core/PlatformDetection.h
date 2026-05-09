#pragma once

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