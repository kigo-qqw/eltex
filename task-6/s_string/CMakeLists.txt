project(s_string LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(${PROJECT_NAME}
        ./src/s_string.c
)
add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}::s_common)

set(DEBUG_COMPILE_OPTIONS -Wall -Wextra -pedantic -Werror -g -O0)
set(RELEASE_COMPILE_OPTIONS -Wall -Wextra -pedantic -Werror -O3)

set(DEBUG_LINK_OPTIONS -g)
set(RELEASE_LINK_OPTIONS -s -O3)

target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_link_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_LINK_OPTIONS}>")
target_link_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_LINK_OPTIONS}>")
