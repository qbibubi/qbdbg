cmake_minimum_required(VERSION 3.13)

project(qbdebug VERSION 1.0 LANGUAGES C)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(qbdbg
    src/main.c
    src/debugger.c
    src/os.c
    src/cli.c
)

target_link_libraries(qbdbg PRIVATE
    readline
)

target_compile_options(qbdbg PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wstrict-prototypes
    -Werror=unused-result
    -g
)
