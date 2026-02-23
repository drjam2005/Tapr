cmake_minimum_required(VERSION 3.28)
project(Tapr)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(exec Tapr)
set(src
	src/main.cpp
	src/objects.cpp

	src/systems/game_system.cpp
	src/systems/event_system.cpp
	src/systems/game_renderer_system.cpp
	src/systems/updater_system.cpp
)

add_executable(${exec} ${src})
add_subdirectory(vendor/miniz)

target_include_directories(${exec} PRIVATE
	${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${exec} PRIVATE
	miniz
)

if(UNIX)
	target_include_directories(${exec} PRIVATE
		${CMAKE_SOURCE_DIR}/vendor/raylib/unix/include
	)
	target_link_directories(${exec} PRIVATE
		${CMAKE_SOURCE_DIR}/vendor/raylib/unix/lib
	)
	target_link_libraries(${exec} PRIVATE
		raylib
		m
	)
elseif(WIN32)
	target_include_directories(${exec} PRIVATE
		${CMAKE_SOURCE_DIR}/vendor/raylib/windows/include
	)
	target_link_directories(${exec} PRIVATE
		${CMAKE_SOURCE_DIR}/vendor/raylib/windows/lib
	)
	target_link_libraries(${exec} PRIVATE
		raylib
		gdi32
		winmm
	)
endif()
