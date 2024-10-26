cmake_minimum_required(VERSION 3.8)
project("HotlineAgony")
set(CMAKE_CXX_STANDARD 17)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_PDB_OUTPUT_DIRECTORY     "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

include(FetchContent)

set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    box2d 
    GIT_REPOSITORY https://github.com/erincatto/box2d.git 
    GIT_TAG v2.4.1
)
FetchContent_MakeAvailable(box2d)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib 
    GIT_REPOSITORY https://github.com/raysan5/raylib.git 
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
    b2drawrayLib 
    GIT_REPOSITORY https://github.com/MetGang/b2DrawRayLib.git
    GIT_TAG main
)
FetchContent_MakeAvailable(b2drawrayLib)

set(B2DRAWRAYLIB_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/b2drawraylib-src)
set(B2DRAWRAYLIB_SOURCES
    ${B2DRAWRAYLIB_INCLUDE_DIR}/b2DrawRayLib.cpp
)


file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/HotlineAgony/src/*.cpp") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/HotlineAgony/include/") # Define PROJECT_INCLUDE to be the path to the include directory of the project


add_executable(${PROJECT_NAME})

add_custom_target(copy_data
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${CMAKE_CURRENT_LIST_DIR}/HotlineAgony/assets/"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/"
)
add_dependencies(${PROJECT_NAME} copy_data)

if (MSVC AND WIN32 AND NOT MSVC_VERSION VERSION_LESS 142)
    target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/Zi>)
endif()

target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES} ${B2DRAWRAYLIB_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE} ${B2DRAWRAYLIB_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE box2d raylib)