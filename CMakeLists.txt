cmake_minimum_required(VERSION 3.8)

project(JCTerminal)







option(JCTERMINAL_BUILD_SAMPLES ON)

if(JCTERMINAL_BUILD_SAMPLES)
	add_subdirectory("samples")
endif()

