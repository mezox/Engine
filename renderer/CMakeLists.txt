cmake_minimum_required(VERSION 3.7)
project(Renderer)

add_definitions(-D_RENDERERDLL)

include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}/interface/
	${CMAKE_CURRENT_SOURCE_DIR}/include/
	${CMAKE_CURRENT_SOURCE_DIR}/../
)

if(WIN32)
	add_definitions(-DGLFW_EXPOSE_NATIVE_WIN32)
endif()

set(RENDERER_PUBLIC_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/interface/RendererBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/Vertex.h
	${CMAKE_CURRENT_SOURCE_DIR}/interface/Renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/Buffer.h
)

set(RENDERER_PRIVATE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/BufferImpl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/RendererVK.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/Texture.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/Object3D.h

	${CMAKE_CURRENT_SOURCE_DIR}/include/Demo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/files.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Math/Vector2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Math/Vector3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Math/Vector4.h

	#3rdParty
	${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/tiny_obj_loader.h
)

set(RENDERER_PRIVATE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RendererVK.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BufferImpl.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Demo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Texture.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Object3D.cpp
)

set(RENDERER_RESOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/resources/triangle.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/triangle.frag
)

add_library(
	${PROJECT_NAME}
	${RENDERER_PUBLIC_HEADERS}
	${RENDERER_PRIVATE_HEADERS}
	${RENDERER_PRIVATE_SOURCES}
	${RENDERER_RESOURCES}
)

target_link_libraries(${PROJECT_NAME}
	PAL_Graphics
	glm
	glfw ${GLFW_LIBRARIES}
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

find_program(GLSLANG_VALIDATOR names glslangValidator HINTS "${GLSLANG_INSTALL_DIR}/bin")

message(STATUS "GLSLANG_VALIDATOR: " ${GLSLANG_VALIDATOR})

add_custom_command(#POST_BUILD
					TARGET ${PROJECT_NAME}
					COMMENT "Compiling cube vertex shader"
					COMMAND ${GLSLANG_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/resources/triangle.vert -o ${EXECUTABLE_OUTPUT_PATH}/Debug/resources/shaders/vert.spv)

add_custom_command(	#POST_BUILD
					TARGET ${PROJECT_NAME}
					COMMENT "Compiling cube fragment shader"
					COMMAND ${GLSLANG_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/resources/triangle.frag -o ${EXECUTABLE_OUTPUT_PATH}/Debug/resources/shaders/frag.spv)