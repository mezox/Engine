#pragma once

#ifndef GRAPHICS_API
	#ifdef _NOEXPORT
		#define GRAPHICS_API
	#elif _GRAPHICSDLL
		#define GRAPHICS_API __declspec(dllexport)
	#else
		#define GRAPHICS_API __declspec(dllimport)
	#endif
#endif
