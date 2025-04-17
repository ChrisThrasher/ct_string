cmake_minimum_required(VERSION 3.28)
project(test_ct_string LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
    find_package(ct_string 0.1.0 EXACT CONFIG REQUIRED)
endif()

add_executable(main main.cpp)
target_link_libraries(main PRIVATE CT::String)
if(MSVC)
    target_compile_options(main PRIVATE /utf-8)
endif()
