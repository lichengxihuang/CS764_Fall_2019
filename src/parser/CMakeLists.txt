add_library(
        parser
        Parser.cpp Parser.h
        ParserNode.cpp ParserNode.h
)

target_link_libraries(
        parser
        sqlite3
)

##set_target_properties(CS764_lichengxi_huang
##        PROPERTIES
##        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#
##add_subdirectory(scheduler)