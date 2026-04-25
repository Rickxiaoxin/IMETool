cmake_minimum_required(VERSION 3.15)
project(ImeTool VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(imetool main.cc)

target_link_libraries(imetool PRIVATE user32 imm32)

include(GNUInstallDirs)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/imetool)
install(TARGETS imetool)
