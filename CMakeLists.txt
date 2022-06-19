cmake_minimum_required(VERSION 3.20)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(FastLEDHubSpectrum)

set(CMAKE_CXX_STANDARD 17)

link_directories(${CMAKE_SOURCE_DIR}/lib)

include_directories(
    ${CMAKE_SOURCE_DIR}/include/BASS
    ${CMAKE_SOURCE_DIR}/include/BASSWASAPI
)

file(GLOB SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.h"
)
add_executable(FastLEDHubSpectrum ${SOURCES})

# BASS
target_link_libraries(FastLEDHubSpectrum bass basswasapi)

# FTXUI
find_package(ftxui CONFIG REQUIRED)
target_link_libraries(FastLEDHubSpectrum ftxui::dom ftxui::screen ftxui::component)

# IXWebsocket
find_package(ixwebsocket CONFIG REQUIRED)
target_link_libraries(FastLEDHubSpectrum ixwebsocket::ixwebsocket)

# CLI11
find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(FastLEDHubSpectrum CLI11::CLI11)