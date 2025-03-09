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

add_executable(motorola_contest src/main.cpp)
target_include_directories(motorola_contest PRIVATE ${PROJECT_INCLUDE})

target_compile_definitions(motorola_contest PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

target_link_libraries(motorola_contest PRIVATE raylib)

file(COPY assets DESTINATION "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

