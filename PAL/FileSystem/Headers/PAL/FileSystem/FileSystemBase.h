#pragma once

#ifndef FILESYSTEM_API
#   ifdef _NOEXPORT
#       define FILESYSTEM_API
#   elif _FILESYSTEMDLL
#       define FILESYSTEM_API __declspec(dllexport)
#   else
#       define FILESYSTEM_API __declspec(dllimport)
#   endif
#endif
