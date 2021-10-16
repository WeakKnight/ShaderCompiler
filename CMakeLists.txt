cmake_minimum_required(VERSION 3.12)
project(ShaderCompiler VERSION 0.1.0)

include_directories(slang/)

add_library(slang SHARED IMPORTED)
set_property(TARGET slang PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/slang/win64/slang.dll)
set_property(TARGET slang PROPERTY IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/slang/win64/slang.lib)

add_executable(ShaderCompiler main.cpp)
target_link_libraries(ShaderCompiler slang)
