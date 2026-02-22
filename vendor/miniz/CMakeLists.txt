add_library(miniz
    src/miniz.c
)

target_include_directories(miniz PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# set_target_properties(miniz PROPERTIES LINKER_LANGUAGE C)
