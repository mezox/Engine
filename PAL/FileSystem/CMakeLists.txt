cmake_minimum_required(VERSION 3.7)

project(PAL_FileSystem)

if (${BUILD_SHARED_LIBS})
	message("Build shared libraries option ON")
else()
	add_definitions("-D_NOEXPORT")
endif()


include_directories(
	"Headers"
	"Headers/PAL/FileSystem/"
	"Source"
)

set(SOURCES
# ---- public headers -----
	Headers/PAL/FileSystem/FileSystemBase.h
	Headers/PAL/FileSystem/LowFileSystem.h
	Headers/PAL/FileSystem/File.h
	Headers/PAL/FileSystem/FileTypes.h

# ---- private sources -----
	Source/File.cpp
)

if (TARGET_WINDOWS)
	list(APPEND SOURCES
# ---- public headers -----

# ---- private sources -----

	)

elseif (TARGET_MACOS)
	set(OBJC_SOURCES
        Source/iOS/LowFileSystem.cpp
	)

	list(APPEND SOURCES
# ---- public headers -----

# ---- private sources -----
		${OBJC_SOURCES}
	)

	objc_sourcess(${OBJC_SOURCES})

    set(PLATFORM_INTERFACES
       Source/iOS/Headers
    )

	include_directories(${PLATFORM_INTERFACES})

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

elseif (TARGET_MACOS)
	list(APPEND SOURCES
# ---- public headers -----

# ---- private sources -----
	)

endif()

add_library(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Headers/ ${PLATFORM_INTERFACES})
