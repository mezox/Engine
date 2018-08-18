cmake_minimum_required(VERSION 3.7)
project(TestApp)
find_package(vulkan REQUIRED)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(MACOS_APPLICATION_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/Demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/DemoViewController.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/files.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/RendererVK.cpp

    # Renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/src/BufferImpl.cpp
)

set(MACOS_APPLICATION_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/Demo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DemoViewController.h
    ${CMAKE_CURRENT_SOURCE_DIR}/files.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Vertex.h

    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/interface/Buffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/include/BufferImpl.h

    ${CMAKE_CURRENT_SOURCE_DIR}/RendererVK.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector4.h
)

set(MACOS_APPLICATION_RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Main.storyboard
    ${CMAKE_CURRENT_SOURCE_DIR}/vert.spv
    ${CMAKE_CURRENT_SOURCE_DIR}/frag.spv
)

add_definitions(-DVK_USE_PLATFORM_MACOS_MVK)
include_directories(./)
include_directories(renderer/interface)
include_directories(renderer/include)

add_executable(TestApp MACOSX_BUNDLE
	${MACOS_APPLICATION_HEADERS}
	${MACOS_APPLICATION_SRCS}
	${MACOS_APPLICATION_RESOURCES})

target_link_libraries(TestApp Vulkan::Vulkan "-framework AppKit -framework QuartzCore")

set_target_properties(TestApp PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
)

set_source_files_properties(${MACOS_APPLICATION_RESOURCES} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources"
)
