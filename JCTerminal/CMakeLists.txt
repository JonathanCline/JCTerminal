
set(lib_cxx 20)

set(jcterminal_c_header "include/c/JCTerminal.h")
set(jcterminal_cpp_header "include/cpp/JCTerminal.hpp")


set(lib_sources 
	
	"${jcterminal_c_header}"
	"${jcterminal_cpp_header}"

)

add_library(${PROJECT_NAME} STATIC "${lib_sources}")

target_include_directories(${PROJECT_NAME} PUBLIC "include" PRIVATE "source")

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		glfw
		glad
		lodepng
		freetype
		glm

		SAELib

)

set_target_properties(${PROJECT_NAME} PROPERTIES 
	CXX_STANDARD ${lib_cxx} 
	CXX_STANDARD_REQUIRED True)
	
	
add_subdirectory("source/Callback")
add_subdirectory("source/CellBuffer")
add_subdirectory("source/Color")
add_subdirectory("source/Interface")
add_subdirectory("source/Settings")
add_subdirectory("source/Terminal")
add_subdirectory("source/Text")
add_subdirectory("source/Window")




