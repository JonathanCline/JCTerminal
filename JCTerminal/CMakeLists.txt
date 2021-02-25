
set(lib_cxx 20)

set(lib_sources 
	"include/${PROJECT_NAME}.h"
	"source/${PROJECT_NAME}.cpp"
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
	


add_subdirectory("source/Text")



