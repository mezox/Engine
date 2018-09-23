cmake_minimum_required(VERSION 3.7)
project(Application)


find_package(vulkan REQUIRED)

# include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/win/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# external libs
if(WIN32)

set(WIN32_APPLICATION_HEADERS
)

set(WIN32_APPLICATION_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/win/src/App.cpp
)

add_executable(Application
	${WIN32_APPLICATION_HEADERS}
	${WIN32_APPLICATION_SRCS}
	${WIN32_APPLICATION_RESOURCES})

target_link_libraries(Application Renderer)
set_target_properties(Application PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Bin/Debug")

else()
#macOS
add_definitions("-D_NOEXPORT")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/macOS/include)


set(MACOS_APPLICATION_HEADERS
   ${CMAKE_CURRENT_SOURCE_DIR}/macOS/include/Application.h
   ${CMAKE_CURRENT_SOURCE_DIR}/macOS/include/DemoViewController.h
)

set(MACOS_APPLICATION_SRCS
   ${CMAKE_CURRENT_SOURCE_DIR}/macOS/src/main.mm
   ${CMAKE_CURRENT_SOURCE_DIR}/macOS/src/Application.mm
   ${CMAKE_CURRENT_SOURCE_DIR}/macOS/src/DemoViewController.mm
)

set(MACOS_APPLICATION_RESOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/macOS/resources/Main.storyboard
   ${EXECUTABLE_OUTPUT_PATH}/Debug/frag.spv
   ${EXECUTABLE_OUTPUT_PATH}/Debug/vert.spv
)

add_definitions(-DVK_USE_PLATFORM_MACOS_MVK)

add_executable(Application MACOSX_BUNDLE
	${MACOS_APPLICATION_HEADERS}
	${MACOS_APPLICATION_SRCS}
	${MACOS_APPLICATION_RESOURCES})

target_link_libraries(Application Renderer "-framework AppKit -framework QuartzCore")

set_target_properties(Application PROPERTIES
   MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/macOS/resources/Info.plist
)

set_source_files_properties(${RENDERER_RESOURCES} PROPERTIES
   MACOSX_PACKAGE_LOCATION "Resources"
)
endif()