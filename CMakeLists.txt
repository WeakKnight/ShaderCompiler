cmake_minimum_required(VERSION 3.0.0)
project(ShaderCompiler VERSION 0.1.0)

add_executable(ShaderCompiler main.cpp)
target_link_libraries(ShaderCompiler "bin/win64/slang")
