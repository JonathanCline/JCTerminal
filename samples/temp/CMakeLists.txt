
add_executable(JCTerminalSample "JCTerminalSample.cpp")
target_link_libraries(JCTerminalSample PUBLIC JCTerminal PRIVATE SAELib)
set_target_properties(JCTerminalSample PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED True)
