cmake_minimum_required(VERSION 3.8)
project("HotlineAgony")
set(CMAKE_CXX_STANDARD 17)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


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

# Adding our source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/HotlineAgony/src/*.cpp") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/HotlineAgony/include/") # Define PROJECT_INCLUDE to be the path to the include directory of the project

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES} ${B2DRAWRAYLIB_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE} ${B2DRAWRAYLIB_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE box2d raylib)
