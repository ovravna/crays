cmake_minimum_required(VERSION 3.0)

project(crays)
set(CMAKE_BUILD_TYPE Debug)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(SDL2_PATH "/usr/include/SDL2")
# set(GLIB2_PATH "/usr/include/glib-2.0")
find_package(SDL2 REQUIRED)
include_directories( ${SDL2_INCLUDE_DIRS} )
# find_package(GLIB2 REQUIRED)
# include_directories( ${GLIB2_INCLUDE_DIRS} )
add_executable(
	crays
	src/main.c
	src/game.c
	# src/drawline.c
	src/inputs.c
)
set_property(TARGET crays PROPERTY C_STANDARD 99)
target_link_libraries(crays -lm SDL2)
