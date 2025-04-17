cmake_minimum_required(VERSION 3.28)
project(ct_string VERSION 0.1.0 LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(CT_DEVELOPER_BUILD "Enable developer build" OFF)
if(CT_DEVELOPER_BUILD)
    if(MSVC)
        add_compile_options(/WX /W4 /permissive-)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
        add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast)
    endif()
endif()

add_library(ct_string src/String.cpp)
add_library(CT::String ALIAS ct_string)
target_sources(ct_string PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/CT/String.hpp)
target_compile_features(ct_string PUBLIC cxx_std_20)
set_target_properties(ct_string PROPERTIES EXPORT_NAME String)

install(TARGETS ct_string EXPORT ct_string-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/ct_string-${PROJECT_VERSION}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/ct_string-${PROJECT_VERSION}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ct_string-${PROJECT_VERSION}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ct_string-${PROJECT_VERSION})
install(EXPORT ct_string-targets
        NAMESPACE CT::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/ct_string-${PROJECT_VERSION})
write_basic_package_version_file(ct_string-config-version.cmake COMPATIBILITY SameMajorVersion)
install(FILES ${PROJECT_SOURCE_DIR}/cmake/ct_string-config.cmake
              ${PROJECT_BINARY_DIR}/ct_string-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_DATADIR}/ct_string-${PROJECT_VERSION})

if(NOT CT_DEVELOPER_BUILD)
    return()
endif()

enable_testing()
add_subdirectory(tests)

add_custom_target(format
    COMMAND clang-format -i `git ls-files *.hpp *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(tidy
    COMMAND run-clang-tidy -p ${CMAKE_BINARY_DIR} `git ls-files *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
