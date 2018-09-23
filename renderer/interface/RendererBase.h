#pragma once

#define _NOEXPORT

#ifndef RENDERER_API
#	ifdef _NOEXPORT
#		define RENDERER_API
#	elif _RENDERERDLL
#		define RENDERER_API __declspec(dllexport)
#	else
#		define RENDERER_API __declspec(dllimport)
#	endif
#endif
