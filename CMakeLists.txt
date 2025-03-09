cmake_minimum_required(VERSION 3.10)

project(motorola_contest)

include(FetchContent)
FetchContent_Declare(
	raylib
	GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
	GIT_TAG "master"
	GIT_PROGRESS TRUE
	FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(raylib)

set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/sources/")

add_executable(motorola_contest src/main.cpp)
target_include_directories(motorola_contest PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(motorola_contest PRIVATE raylib)

