cmake_minimum_required(VERSION 3.28)
project(Tapr)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 17)

set(exec Tapr)
include(FetchContent)

# miniz
FetchContent_Declare(
    miniz
    GIT_REPOSITORY https://github.com/richgel999/miniz.git
    GIT_TAG master
)

# raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)

# raygui
FetchContent_Declare(
    raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG master
)

FetchContent_MakeAvailable(miniz)
FetchContent_MakeAvailable(raylib)
FetchContent_MakeAvailable(raygui)

set(src
    src/main.cpp
    src/objects.cpp

    src/systems/game_system.cpp
    src/systems/event_system.cpp
    src/systems/game_renderer_system.cpp
    src/systems/updater_system.cpp
    src/systems/app_system.cpp

    src/systems/utils/zip_util.cpp
    src/systems/utils/raygui_impl.cpp
)

add_executable(${exec} ${src})

target_include_directories(${exec} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${raygui_SOURCE_DIR}/src
)

target_link_libraries(${exec} PRIVATE
    raylib
    miniz
)

target_compile_options(${exec} PRIVATE
    -fpermissive
)
