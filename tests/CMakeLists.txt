add_subdirectory(install)

include(FetchContent)

FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(Catch2)
target_compile_features(Catch2 PRIVATE cxx_std_17)
set_target_properties(Catch2 PROPERTIES COMPILE_OPTIONS "")
include(Catch)

add_executable(test_ct_string String.cpp)
target_link_libraries(test_ct_string PRIVATE CT::String Catch2::Catch2WithMain)
target_compile_definitions(test_ct_string PRIVATE CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS CATCH_CONFIG_FALLBACK_STRINGIFIER=DoesNotExist)
if(MSVC)
    target_compile_options(test_ct_string PRIVATE /utf-8)
endif()
catch_discover_tests(test_ct_string)
