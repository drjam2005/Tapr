cmake_minimum_required(VERSION 4.1.2)
project(Tapr)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(exec exec)
set(src
	src/main.cpp
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
	)
elseif(WIN32)
	target_include_directories(${exec} PRIVATE
		${CMAKE_SOURCE_DIR}/vendor/raylib/windows/include
	)
	target_link_directories(${exec} PRIVATE
		${CMAKE_SOURCE_DIR}/vendor/raylib/windows/lib
	)
endif()
