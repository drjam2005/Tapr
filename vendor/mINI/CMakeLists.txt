add_library(mini INTERFACE)

target_include_directories(mini INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# set_target_properties(miniz PROPERTIES LINKER_LANGUAGE C)

