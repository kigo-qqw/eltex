cmake_minimum_required(VERSION 3.26)
project(mod3-7 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(PORT 5000 CACHE STRING "port")

set(USE_ADDRESS_SANITIZER OFF CACHE BOOL "Use AddressSanitizer")
set(USE_MEMORY_SANITIZER OFF CACHE BOOL "Use MemorySanitizer")
set(USE_THREAD_SANITIZER OFF CACHE BOOL "Use ThreadSanitizer")
set(USE_UNDEFINED_BEHAVIOR_SANITIZER OFF CACHE BOOL "Use UndefinedBehaviorSanitizer")

set(COMPILE_FLAGS
    "$<$<CONFIG:DEBUG>:-O0;-g>"
    "$<$<CONFIG:RELEASE>:-O3>"
    -Werror
    -pedantic

    -Wno-declaration-after-statement
    -Wno-unsafe-buffer-usage
    -Wno-reserved-identifier
)

set(LINK_FLAGS)

# No support for Intel and MSVC
if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    list(APPEND COMPILE_FLAGS -Weverything)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    list(APPEND COMPILE_FLAGS -Wall -Wextra)
endif()

if(${USE_ADDRESS_SANITIZER} EQUAL ON) 
    list(APPEND COMPILE_FLAGS "-fsanitize=address")
    list(APPEND LINK_FLAGS "-fsanitize=address")
endif()

if(${USE_MEMORY_SANITIZER} EQUAL ON)
    list(APPEND COMPILE_FLAGS "-fsanitize=memory")
    list(APPEND LINK_FLAGS "-fsanitize=memory")
endif()

if(${USE_THREAD_SANITIZER} EQUAL ON)
    list(APPEND COMPILE_FLAGS "-fsanitize=thread")
    list(APPEND LINK_FLAGS "-fsanitize=thread")
endif()

if(${USE_UNDEFINED_BEHAVIOR_SANITIZER})
    list(APPEND COMPILE_FLAGS "-fsanitize=undefined")
    list(APPEND LINK_FLAGS "-fsanitize=undefined")
endif()


add_executable(server)
target_sources(server PRIVATE ${PROJECT_SOURCE_DIR}/server.c)
target_compile_options(server PRIVATE ${COMPILE_FLAGS})
target_compile_definitions(server PRIVATE _PORT=${PORT})
target_link_options(server PRIVATE ${LINK_FLAGS})

add_executable(client)
target_sources(client PRIVATE ${PROJECT_SOURCE_DIR}/client.c)
target_compile_options(client PRIVATE ${COMPILE_FLAGS})
target_compile_definitions(client PRIVATE _PORT=${PORT})
target_link_options(client PRIVATE ${LINK_FLAGS})