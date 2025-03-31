cmake_minimum_required(VERSION 3.10)

project(motorola_contest)

set(CMAKE_CXX_STANDARD 20)
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

include(FetchContent)
FetchContent_Declare(
	raygui
	GIT_REPOSITORY "https://github.com/raysan5/raygui.git"
	GIT_TAG "master"
	GIT_PROGRESS TRUE
	FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(raygui)

file(GLOB_RECURSE project_sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
add_executable(motorola_contest ${project_sources})
# target_include_directories(motorola_contest PRIVATE "lib/")
target_link_libraries(motorola_contest PRIVATE raylib)
target_include_directories(motorola_contest SYSTEM PRIVATE "${raylib_SOURCE_DIR}/src")
target_include_directories(motorola_contest SYSTEM PRIVATE "${raygui_SOURCE_DIR}/src")


if(MSVC)
	target_compile_options(motorola_contest PRIVATE /W4)
else()
	target_compile_options(motorola_contest PRIVATE -Wall -Wextra -Wpedantic)
endif()

# target_compile_definitions(motorola_contest PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

# Tests
file(GLOB_RECURSE TESTS_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/tests/*.cpp")
set(project_sources_without_main ${project_sources})
file(GLOB_RECURSE main_file CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp")
list(REMOVE_ITEM project_sources_without_main ${main_file})
message("project_sources_without_main=${project_sources_without_main}")
add_executable(all-tests ${TESTS_SOURCES} ${project_sources_without_main})
target_link_libraries(all-tests PRIVATE raylib)
target_include_directories(all-tests SYSTEM PRIVATE "src" "${raylib_SOURCE_DIR}/src")
target_include_directories(all-tests SYSTEM PRIVATE "src" "${raygui_SOURCE_DIR}/src")
add_test(NAME all-tests COMMAND all-tests)
enable_testing()

if(MSVC)
	target_compile_options(all-tests PRIVATE /W4)
else()
	target_compile_options(all-tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

# file(COPY assets DESTINATION "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets)
add_dependencies(motorola_contest copy_assets)

