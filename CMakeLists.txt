cmake_minimum_required(VERSION 3.20)

set(namespace "osv")
project(OSVision VERSION 0.1.0 DESCRIPTION "Open source security camera library")

if(NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
    message(STATUS "This project is a top-level one")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE src/osvision.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE

    # where the library itself will look for its internal headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC

    # where top-level project will look for the library's public headers
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>

    # where external projects will look for the library's public headers
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Listing of `include` directory
get_property(inclds TARGET ${PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES)

message(STATUS "Listing include directories for ${PROJECT_NAME}...")

foreach(pth ${inclds})
    message(STATUS "  ${pth}")
endforeach()

# actually evaluated generator expressions printed to file on generation
string(JOIN "\n" includeDirectories ${inclds})
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/include-directories.txt" CONTENT ${includeDirectories})

# Without this, public headers won't get installed
set(public_headers include/osvision.hpp)

# not for MSVC
if(CMAKE_COMPILER_IS_GNUCXX)
    # compile options for this target only
    # treat warnings as errors
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
endif()

# where to find our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# message(STATUS "CMake module path: ${CMAKE_MODULE_PATH}")
include(Installing)
find_package(OpenCV REQUIRED)

# ## TESTING
option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ## DISTRIBUTION
include(InstallRequiredSystemLibraries)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)