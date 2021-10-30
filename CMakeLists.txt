cmake_minimum_required(VERSION 3.12)
project(ShaderCompiler VERSION 0.1.0)

include_directories(include/)
include_directories(thirdparty/slang/)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set Output Directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/bin>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG $<1:${CMAKE_SOURCE_DIR}/bin>)

add_library(slang SHARED IMPORTED)
set_property(TARGET slang PROPERTY IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/thirdparty/slang/win64/slang.lib)

file(GLOB HEADERS "include/*.h")
file(GLOB SOURCES "src/*.cpp")
add_library(ShaderCompiler ${SOURCES} ${HEADERS})
target_link_libraries(ShaderCompiler slang)

add_executable(test test/main.cpp)
target_link_libraries(test ShaderCompiler slang)
