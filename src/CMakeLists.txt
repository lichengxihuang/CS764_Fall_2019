add_executable(
        CS764_lichengxi_huang
        main.cpp
)

target_link_libraries(CS764_lichengxi_huang parser)

#set_target_properties(CS764_lichengxi_huang
#        PROPERTIES
#        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(parser)