cmake_minimum_required(VERSION 3.28)
project(Tapr)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(exec exec)
set(src
	src/main.cpp
	src/objects.cpp
)

add_executable(${exec} ${src})

target_include_directories(${exec} PRIVATE
	${CMAKE_SOURCE_DIR}/include
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
		m
	)
endif()
