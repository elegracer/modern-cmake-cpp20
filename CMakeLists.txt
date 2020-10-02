cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")
project(proj VERSION 2.2.3)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0120 OLD)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")
set(THIRDPARTY_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/3rdparty")
set(THIRDPARTY_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/deps")

# ccache
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message(STATUS "Found CCache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

set(CMAKE_CXX_STANDARD 20)

# always generate PIC code, no matter static or shared library
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# dependencies

# boost
find_package(Boost REQUIRED COMPONENTS ALL)
message(STATUS "${Boost_LIBRARIES}")
add_library(depends::boost INTERFACE IMPORTED GLOBAL)
target_link_libraries(depends::boost INTERFACE Boost::boost Boost::json Boost::url)
# spdlog
find_package(spdlog REQUIRED)
add_library(depends::spdlog INTERFACE IMPORTED GLOBAL)
target_link_libraries(depends::spdlog INTERFACE spdlog::spdlog_header_only)
# zlib
find_package(ZLIB REQUIRED)
add_library(depends::zlib INTERFACE IMPORTED GLOBAL)
target_link_libraries(depends::zlib INTERFACE ZLIB::ZLIB)
# openssl
if(APPLE AND NOT DEFINED ENV{OPENSSL_ROOT_DIR})
    find_program(HOMEBREW brew)
    if(NOT HOMEBREW STREQUAL "HOMEBREW-NOTFOUND")
        execute_process(
            COMMAND brew --prefix openssl OUTPUT_VARIABLE OPENSSL_ROOT_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()
endif()
find_package(OpenSSL REQUIRED)
add_library(depends::openssl INTERFACE IMPORTED GLOBAL)
target_link_libraries(depends::openssl INTERFACE OpenSSL::SSL OpenSSL::Crypto)
if(WIN32)
    target_link_libraries(depends::openssl INTERFACE ws2_32)
endif()
# threads
find_package(Threads REQUIRED)
add_library(depends::threads INTERFACE IMPORTED GLOBAL)
target_link_libraries(depends::threads INTERFACE Threads::Threads)

# options
option(PROJ_OPTION "..." ON)

configure_file(cmake/version.h.in include/version.h)

add_library(proj-core)
target_sources(
    proj-core
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/boost-ext/di.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/boost-ext/mp.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/boost-ext/sml2
        ${CMAKE_CURRENT_SOURCE_DIR}/src/boost-ext/te.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/boost-ext/ut.hpp
)
target_include_directories(
    proj-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
                   ${CMAKE_CURRENT_BINARY_DIR}/include
)
target_link_libraries(
    proj-core
    PUBLIC depends::boost
           depends::spdlog
           depends::zlib
           depends::openssl
           depends::threads
)

# if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#     target_compile_options(proj-core PUBLIC -stdlib=libc++)
#     target_link_options(proj-core PUBLIC -stdlib=libc++)
# elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#     target_compile_definitions(
#         proj-core PUBLIC BOOST_ALL_NO_LIB _CRT_SECURE_NO_WARNINGS
#                        _WIN32_WINNT=0x0601
#     )
#     target_compile_options(proj-core PUBLIC /bigobj /O2)
# endif()

add_subdirectory(utils)
