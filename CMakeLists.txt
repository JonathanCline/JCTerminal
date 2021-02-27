cmake_minimum_required(VERSION 3.8)

option(JCTERMINAL_BUILD_SAMPLES ON)
option(JCTERMINAL_GIT_CLONE_DEPENDENCIES "Clones the dependencies if the targets are not already defined" ON)

project(JCTerminal)

add_subdirectory("JCTerminal")



set(JCTERMINAL_INSTALL_HEADERS_PATH "include")
set(JCTERMINAL_INSTALL_STATIC_LIBS_PATH "lib")



set(lib_dir "lib")


find_package(Git QUIET)



macro(ADD_GIT_DEPENDENCY depLibraryDir depTarget depName githubURL)
if(NOT TARGET ${depTarget})
	execute_process(COMMAND 
		${GIT_EXECUTABLE} clone ${githubURL} ${depLibraryDir}/${depName}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		RESULT_VARIABLE GIT_SUBMOD_RESULT)	
	add_subdirectory("${depLibraryDir}/${depName}")	
endif()
endmacro()

macro(ADD_GIT_DEPENDENCY_NO_CMAKE depLibraryDir depTarget depName githubURL)
if(NOT TARGET ${depTarget})
	execute_process(COMMAND 
		${GIT_EXECUTABLE} clone ${githubURL} ${depLibraryDir}/${depName}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		RESULT_VARIABLE GIT_SUBMOD_RESULT)
endif()
endmacro()




#
#	GLFW 		https://github.com/glfw/glfw
#

ADD_GIT_DEPENDENCY("${lib_dir}" GLFW glfw "https://github.com/glfw/glfw")


#
#	GLM			https://github.com/g-truc/glm
#

ADD_GIT_DEPENDENCY("${lib_dir}" glm glm  "https://github.com/g-truc/glm")


#
#	Freetype	https://github.com/freetype/freetype
#

ADD_GIT_DEPENDENCY("${lib_dir}" freetype freetype "https://github.com/freetype/freetype")


#
#	SAELib		https://github.com/JonathanCline/SAELib
#

ADD_GIT_DEPENDENCY("${lib_dir}" SAELib SAELib "https://github.com/JonathanCline/SAELib")


#
#	Glad
#

if(NOT TARGET glad)
	add_subdirectory("${lib_dir}/glad")
endif()


# 
#	lodepng
#

if(NOT TARGET lodepng)
	add_library(lodepng STATIC "${lib_dir}/lodepng/lodepng.h" "${lib_dir}/lodepng/lodepng.cpp")
	target_include_directories(lodepng PUBLIC "${lib_dir}/lodepng")
endif()




add_subdirectory("samples")

