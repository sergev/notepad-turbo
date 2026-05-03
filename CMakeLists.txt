cmake_minimum_required(VERSION 3.21)

project(NotepadNext
    VERSION 0.14
    DESCRIPTION "Cross-platform text editor (TUI)"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Werror)

string(TIMESTAMP CURRENT_YEAR "%Y")
set(APP_COPYRIGHT "Copyright 2019-${CURRENT_YEAR} Justin Dailey")

if(MSVC)
    add_compile_definitions(_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR)
endif()

if(UNIX AND NOT APPLE)
    add_compile_options(-fstack-protector-strong -D_FORTIFY_SOURCE=2)
    add_link_options(-Wl,-z,relro -Wl,-z,now)
endif()

# Turbo Vision TUI library
include(FetchContent)
FetchContent_Declare(
    tvision
    GIT_REPOSITORY https://github.com/magiblot/tvision.git
    GIT_TAG 6141d9e1a434bb0283c8d434f8988589e7c3dead
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(tvision)

add_subdirectory(thirdparty)
add_subdirectory(src)
