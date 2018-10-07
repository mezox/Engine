cmake_minimum_required(VERSION 3.7)

project(PAL_Graphics)

if (${BUILD_SHARED_LIBS})
	message("Build shared libraries option ON")
else()
	add_definitions("-D_NOEXPORT")
endif()

find_package(vulkan REQUIRED)

include_directories(
	"Headers"
	"Headers/PAL/Graphics/"
	"Source"
)

set(SOURCES
# ---- public headers -----
	Headers/PAL/Graphics/GraphicsBase.h
	Headers/PAL/Graphics/LowVK.h
	Headers/PAL/Graphics/VKAny.h

# ---- private sources -----
	Source/Any/LowVKAny.cpp
	Source/Any/VKAny.cpp
	Source/Any/LayersExtensions.inl
)

if (TARGET_WINDOWS)
	list(APPEND SOURCES
# ---- public headers -----

# ---- private sources -----

	)

	add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)

elseif (TARGET_IOS)
	set(OBJC_SOURCES
	)

	list(APPEND SOURCES
# ---- public headers -----

# ---- private sources -----
		${OBJC_SOURCES}
	)

elseif (TARGET_ANDROID)
	list(APPEND SOURCES
# ---- public headers -----

# ---- private sources -----

	)

elseif (TARGET_LINUX)
	list(APPEND SOURCES
# ---- public headers -----

# ---- private sources -----
	)

	if (NOT GRAPHICS_SUBSYSTEM)
		# default graphics system for Linux is X11
		set(GRAPHICS_SUBSYSTEM "X11")
	endif()

elseif (TARGET_MACOS)
	list(APPEND SOURCES
# ---- public headers -----

# ---- private sources -----
	)

endif()

add_library(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Headers/ ${CMAKE_CURRENT_SOURCE_DIR}/Headers/PAL/Graphics/)