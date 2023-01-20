add_executable(pyraminx_cli interactive.cpp)

target_link_libraries(pyraminx_cli PRIVATE pyraminx)
set_property(TARGET pyraminx_cli PROPERTY CXX_STANDARD 11)