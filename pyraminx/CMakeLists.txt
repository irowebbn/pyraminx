add_library(pyraminx 
    Pyraminx.cpp Face.cpp Eq_Pos_Table.cpp
    )

set_property(TARGET pyraminx PROPERTY CXX_STANDARD 11)
target_include_directories(pyraminx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})