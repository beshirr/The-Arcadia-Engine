cmake_minimum_required(VERSION 3.16)

project(The_Arcadia_Engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Nice-to-have warnings (kept reasonable for student projects)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Engine library (contains the factory functions too)
add_library(arcadia_engine STATIC
        ArcadiaEngine.cpp
)
target_include_directories(arcadia_engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Student test runner executable
add_executable(main_test_students
        main_test_students.cpp
)
target_link_libraries(main_test_students PRIVATE arcadia_engine)