project(s_main LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
        ./src/main.c
)

target_link_libraries(${PROJECT_NAME}
        ${CMAKE_PROJECT_NAME}::s_students
        ${CMAKE_PROJECT_NAME}::s_terminal
        ${CMAKE_PROJECT_NAME}::s_utils
)

set(DEBUG_COMPILE_OPTIONS -Wall -Wextra -pedantic -Werror -g -O0)
set(RELEASE_COMPILE_OPTIONS -Wall -Wextra -pedantic -Werror -O3)

set(DEBUG_LINK_OPTIONS -g)
set(RELEASE_LINK_OPTIONS -s -O3)

target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_link_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_LINK_OPTIONS}>")
target_link_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_LINK_OPTIONS}>")
