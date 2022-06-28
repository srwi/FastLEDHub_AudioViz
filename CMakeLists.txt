cmake_minimum_required(VERSION 3.20)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(FastLEDHubAudioViz)

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
add_executable(FastLEDHubAudioViz ${SOURCES})

# BASS
target_link_libraries(FastLEDHubAudioViz bass basswasapi)

# FTXUI
find_package(ftxui CONFIG REQUIRED)
target_link_libraries(FastLEDHubAudioViz ftxui::dom ftxui::screen ftxui::component)

# IXWebsocket
find_package(ixwebsocket CONFIG REQUIRED)
target_link_libraries(FastLEDHubAudioViz ixwebsocket::ixwebsocket)

# CLI11
find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(FastLEDHubAudioViz CLI11::CLI11)

add_custom_command(TARGET FastLEDHubAudioViz POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/lib/dll" $<TARGET_FILE_DIR:FastLEDHubAudioViz>)