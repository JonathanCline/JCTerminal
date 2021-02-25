cmake_minimum_required(VERSION 3.8)

option(JCTERMINAL_BUILD_SAMPLES ON)

project(JCTerminal)

add_subdirectory("JCTerminal")


#
#	Remove me
#

add_subdirectory("lib")
add_subdirectory("samples")

