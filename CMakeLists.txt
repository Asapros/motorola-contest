cmake_minimum_required(VERSION 3.10)

project(motorola_contest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
	raylib
	GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
	GIT_TAG "master"
	GIT_PROGRESS TRUE
	FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(raylib)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
add_executable(motorola_contest ${PROJECT_SOURCES})
# target_include_directories(motorola_contest PRIVATE "lib/")

# target_compile_definitions(motorola_contest PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

target_link_libraries(motorola_contest PRIVATE raylib)
target_include_directories(motorola_contest PRIVATE "${raylib_SOURCE_DIR}/src")

# file(COPY assets DESTINATION "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets)
add_dependencies(motorola_contest copy_assets)

