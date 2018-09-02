cmake_minimum_required(VERSION 3.7)
project(Engine)

# compiler settings
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

# generuje interface a install commandy pre sdk modul
macro(mark_internal_sdk_module_target)
	target_include_directories(${_target} INTERFACE
		${ARGN}
	)

	install(
		TARGETS ${_target}
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
	)

	set(LOCAL_INTERFACE_INCLUDE_DIRS)

	#treba na koniec kazdeho adresara pripojit delimiter aby sa spravne urcila relativna path, vid manual k install DIRECTORY
	FOREACH(_item ${ARGN})
		string(CONCAT _local_dir ${_item} "/")
		list(APPEND LOCAL_INTERFACE_INCLUDE_DIRS ${_local_dir})
	ENDFOREACH()

	install(
		DIRECTORY ${LOCAL_INTERFACE_INCLUDE_DIRS} DESTINATION include COMPONENT internal_sdk FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
	)
endmacro()

macro(mark_internal_module_target)
	message(STATUS "Mark internal module: ${_target} ${ARGN}")

	mark_internal_sdk_module_target(${_target} ${ARGN})

	if(BUILD_SHARED_LIBS)
		install(
			TARGETS ${_target}
			RUNTIME DESTINATION bin
		)
	endif()

	if (TARGET_WINDOWS)
		install(FILES $<TARGET_PDB_FILE:${_target}> DESTINATION bin OPTIONAL)
	endif()
endmacro()

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/Bin/${CMAKE_BUILD_TYPE} CACHE PATH "Library output path")
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/Bin/${CMAKE_BUILD_TYPE} CACHE PATH "Executable output path")

message("Library output path: " ${LIBRARY_OUTPUT_PATH})
message("Executable output path: " ${EXECUTABLE_OUTPUT_PATH})

if(WIN32)
	set(TARGET_WINDOWS 1)
endif()

add_subdirectory(External)
add_subdirectory(PAL)
add_subdirectory(Renderer)
add_subdirectory(Application)
