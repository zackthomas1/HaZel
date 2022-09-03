#pragma once

#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_BUILD_DLL
		#define HAZZEL_API __declspec(dllexport)
	#else
		#define HAZZEL_API __declspec(dllimport)
	#endif // HZ_BUILD_DLL
#else
	#error Hazzel only supports Windows;

#endif // HZ_PLATFORM_WINDOWS

#define BIT(x) (1 << x)
