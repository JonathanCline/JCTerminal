add_library(glad STATIC "include/glad/glad.h" "src/glad.c" "include/KHR/khrplatform.h")
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
