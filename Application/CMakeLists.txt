cmake_minimum_required(VERSION 3.7)
project(Application)

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

target_link_libraries(Application Renderer glfw ${GLFW_LIBRARIES})
set_target_properties(Application PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Bin/Debug")

else()
#macOS
#
#set(MACOS_APPLICATION_HEADERS
#	${CMAKE_CURRENT_SOURCE_DIR}/Demo.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/Application.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/DemoViewController.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/files.h
#
#    ${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector2.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector3.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector4.h
#)
#
#set(MACOS_APPLICATION_SRCS
#	${CMAKE_CURRENT_SOURCE_DIR}/Demo.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/main.mm
#    ${CMAKE_CURRENT_SOURCE_DIR}/Application.mm
#    ${CMAKE_CURRENT_SOURCE_DIR}/DemoViewController.mm
#    ${CMAKE_CURRENT_SOURCE_DIR}/files.mm
#)
#
#set(MACOS_APPLICATION_RESOURCES
#    ${CMAKE_CURRENT_SOURCE_DIR}/Main.storyboard
#    ${CMAKE_CURRENT_SOURCE_DIR}/vert.spv
#    ${CMAKE_CURRENT_SOURCE_DIR}/frag.spv
#)
#
#add_definitions(-DVK_USE_PLATFORM_MACOS_MVK)
#
#add_executable(TestApp MACOSX_BUNDLE
#	${MACOS_APPLICATION_HEADERS}
#	${MACOS_APPLICATION_SRCS}
#	${MACOS_APPLICATION_RESOURCES})
#
#target_link_libraries(TestApp Vulkan::Vulkan "-framework AppKit -framework QuartzCore")
#
#set_target_properties(TestApp PROPERTIES
#    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
#)
#
#set_source_files_properties(${MACOS_APPLICATION_RESOURCES} PROPERTIES
#    MACOSX_PACKAGE_LOCATION "Resources"
#)
endif()