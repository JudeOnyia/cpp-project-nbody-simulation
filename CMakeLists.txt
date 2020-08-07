# Specify Minimum Required Version
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# Specify Project and Language
project(cpp_project LANGUAGES CXX)

# Find Packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Hack needed for some systems
if (NOT GLUT_Xmu_LIBRARY OR NOT GLUT_Xmi_LIBRARY)
	if (NOT GLUT_Xmu_LIBRARY)
		set(GLUT_Xmu_LIBRARY "")
	endif()
	if (NOT GLUT_Xmi_LIBRARY)
		set(GLUT_Xmi_LIBRARY "")
	endif()
	set(GLUT_LIBRARIES "${GLUT_glut_LIBRARY}")
endif()

# Set Include Directory and Libraries
include_directories(include ${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
link_libraries(${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

include(Sanitizers.cmake) # REMOVEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
set(CMAKE_BUILD_TYPE Debug) # REMOVEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

# Add Executable Program
add_executable(n_body n_body.cpp)

install(TARGETS n_body DESTINATION bin)
install(PROGRAMS demo DESTINATION bin)
install(FILES initial_state.txt DESTINATION bin)
