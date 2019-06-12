cmake_minimum_required(VERSION 3.0)

project(crays)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( OpenGL_GL_PREFERENCE GLVND )
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
include_directories( ${GLUT_INCLUDE_DIRS} )
include_directories( ${GLFW_INCLUDE_DIRS} )

add_executable(
	crays
	src/main.c
)
target_link_libraries(crays ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES} )
