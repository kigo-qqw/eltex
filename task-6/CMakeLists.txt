cmake_minimum_required(VERSION 3.25)

project(eltex-task-6 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


add_subdirectory(s_common)
add_subdirectory(s_string)
add_subdirectory(s_utils)
add_subdirectory(s_terminal)
add_subdirectory(s_students)
add_subdirectory(s_main)

#add_executable(${PROJECT_NAME} main.c)
#target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}::students)
