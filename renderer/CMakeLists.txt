cmake_minimum_required(VERSION 3.7)
project(Renderer)

if(WIN32)
	add_definitions(-D_RENDERERDLL)
else()
	add_definitions(-D_NOEXPORT)
endif()

include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}/interface/
	${CMAKE_CURRENT_SOURCE_DIR}/include/
	${CMAKE_CURRENT_SOURCE_DIR}/../
)

set(RENDERER_PUBLIC_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/interface/RendererBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/Vertex.h
	${CMAKE_CURRENT_SOURCE_DIR}/interface/Renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/Buffer.h
	${CMAKE_CURRENT_SOURCE_DIR}/interface/Window.h
	${CMAKE_CURRENT_SOURCE_DIR}/interface/SwapChain.h
)

set(RENDERER_PRIVATE_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/include/SwapChainImpl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/RendererVK.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/Texture.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/Object3D.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/Effect.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/Engine.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/WindowImpl.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/RendererResourceStateVK.h

	${CMAKE_CURRENT_SOURCE_DIR}/include/Demo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/files.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/../Math/Vector2.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/../Math/Vector3.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/../Math/Vector4.h

	#3rdParty
	${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/tiny_obj_loader.h
)

set(RENDERER_PRIVATE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/SwapChainImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RendererVK.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Demo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Texture.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Object3D.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Effect.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Engine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/RendererResourceStateVK.cpp
)

if(WIN32)
set(RENDERER_WIN32_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/WindowImpl.cpp
)

add_library(
	${PROJECT_NAME}
	${RENDERER_PUBLIC_HEADERS}
	${RENDERER_PRIVATE_HEADERS}
	${RENDERER_PRIVATE_SOURCES}
	${RENDERER_WIN32_SOURCES}
	${RENDERER_RESOURCES}
)
else()
set(RENDERER_PRIVATE_SOURCES_OBJC
	${CMAKE_CURRENT_SOURCE_DIR}/src/files.mm
	${CMAKE_CURRENT_SOURCE_DIR}/src/WindowImplMac.cpp
)

add_library(
	${PROJECT_NAME}
	${RENDERER_PUBLIC_HEADERS}
	${RENDERER_PRIVATE_HEADERS}
	${RENDERER_PRIVATE_SOURCES}
	${RENDERER_RESOURCES}
	${RENDERER_PRIVATE_SOURCES_OBJC}
)
endif()

target_link_libraries(${PROJECT_NAME}
	PAL_Graphics
	glm
)

target_include_directories(${PROJECT_NAME} INTERFACE "include/" "interface/")

#install(DIRECTORY "include/" "interface/" DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

SET(TARGET_PROJECT_FOLDER "")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "${TARGET_PROJECT_FOLDER}")

install(
	TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

if(TARGET_WIN32)
	find_program(GLSLANG_VALIDATOR names glslangValidator HINTS "${GLSLANG_INSTALL_DIR}/bin")

	add_custom_command(#POST_BUILD
					TARGET ${PROJECT_NAME}
					COMMENT "Compiling cube vertex shader"
					COMMAND ${GLSLANG_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/resources/triangle.vert -o ${EXECUTABLE_OUTPUT_PATH}/Debug/resources/shaders/vert.spv)

	add_custom_command(	#POST_BUILD
					TARGET ${PROJECT_NAME}
					COMMENT "Compiling cube fragment shader"
					COMMAND ${GLSLANG_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/resources/triangle.frag -o ${EXECUTABLE_OUTPUT_PATH}/Debug/resources/shaders/frag.spv)
else()
	find_program(GLSLANG_VALIDATOR names glslangValidator HINTS "${VULKAN_SDK}/bin")

	add_custom_command(#POST_BUILD
					TARGET ${PROJECT_NAME}
					COMMENT "Compiling cube vertex shader"
					COMMAND ${GLSLANG_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/resources/triangle.vert -o ${EXECUTABLE_OUTPUT_PATH}/Debug/vert.spv)

	add_custom_command(	#POST_BUILD
					TARGET ${PROJECT_NAME}
					COMMENT "Compiling cube fragment shader"
					COMMAND ${GLSLANG_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/resources/triangle.frag -o ${EXECUTABLE_OUTPUT_PATH}/Debug/frag.spv)

endif()

message(STATUS "GLSLANG_VALIDATOR: " ${GLSLANG_VALIDATOR})

