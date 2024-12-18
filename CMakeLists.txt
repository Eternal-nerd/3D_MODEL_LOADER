cmake_minimum_required(VERSION 3.5)
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
	src/util.cpp
)

# add headers
set(HEADERS
	src/engine.h
	src/util.h
	src/types.h
	src/data.h
)

# add executable:
add_executable(3D_MODEL_LOADER ${SOURCES} ${HEADERS})

# add libs
target_link_libraries(3D_MODEL_LOADER PRIVATE Vulkan::Vulkan SDL3::SDL3)

# compile shaders
add_custom_target(compile_shaders
	COMMENT "Compiling shaders..."
	WORKING_DIRECTORY "../../../shaders"
	COMMAND "compile.bat"
)

add_dependencies(3D_MODEL_LOADER compile_shaders)
