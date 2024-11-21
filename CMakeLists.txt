cmake_minimum_required(VERSION 3.5)
project(3D_MODEL_LOADER)

# set standard:
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# compiler flags:
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

# Vulkan
find_package(Vulkan REQUIRED)

# SDL3 
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

# Set the path to the Vulkan SDK
# set(VULKAN_SDK_PATH "/usr" CACHE PATH "Path to Vulkan SDK")

# add source files:
set(SOURCES
	src/main.cpp
	src/engine.cpp
)

# add headers
set(HEADERS
	src/engine.h
)

# add executable:
add_executable(3D_MODEL_LOADER ${SOURCES} ${HEADERS})

# add libs
target_link_libraries(3D_MODEL_LOADER PRIVATE Vulkan::Vulkan SDL3::SDL3)

# Specify the output directory for the binary
set_target_properties(3D_MODEL_LOADER PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# command to run in root dir:
add_custom_target(run
	COMMAND 3D_MODEL_LOADER
	DEPENDS 3D_MODEL_LOADER
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
	COMMENT "Running 3D_MODEL_LOADER"
)
