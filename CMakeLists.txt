cmake_minimum_required(VERSION 3.0)

project(crays)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(SDL2_PATH "/usr/local/include/SDL2")
find_package(SDL2 REQUIRED)
include_directories( ${SDL2_INCLUDE_DIRS} )
add_executable(
	crays
	src/main.c
	src/rays.c
)
target_link_libraries(crays -lm SDL2 )
