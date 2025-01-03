﻿cmake_minimum_required(VERSION 3.5)
project(3D_MODEL_LOADER)

# set standard:
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vulkan
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

# SDL3 
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
include_directories(${SDL3_INCLUDE_DIRS})

# add source files:
set(SOURCES
	src/main.cpp
	src/engine.cpp
	src/gfx.cpp
	src/util.cpp
)

# add headers
set(HEADERS
	src/engine.h
	src/gfx.h
	src/util.h
	src/data.h
	libs/stb_image.h
	libs/stb_image_resize2.h
)

# add executable:
add_executable(3D_MODEL_LOADER ${SOURCES} ${HEADERS})

# add libs
target_link_libraries(3D_MODEL_LOADER PRIVATE Vulkan::Vulkan SDL3::SDL3)

# Specify the output directory for the binary
set_target_properties(3D_MODEL_LOADER PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if (WIN32)
	# Windows-specific stuff

	# compile shaders on windows
	add_custom_target(compile_shaders
		COMMENT "Compiling shaders..."
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shaders
		COMMAND "compile.bat"
	)
	add_dependencies(3D_MODEL_LOADER compile_shaders)

else()
	# Linux/other (only Linux for now)
	
	# custom target for run script on Linux (run.sh)
	# command to run in root dir:
	add_custom_target(run
		COMMAND 3D_MODEL_LOADER
		DEPENDS 3D_MODEL_LOADER
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
		COMMENT "Running 3D_MODEL_LOADER"
	)

endif()


