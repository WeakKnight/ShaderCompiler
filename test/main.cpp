#include <iostream>
#include <filesystem>
#include "slang.h"
#include "compiler.h"

int main(int argc, char** argv) 
{
    std::filesystem::path exePath = argv[0];
    std::filesystem::path shaderPath = exePath.parent_path() / "shaders/test.hlsl";

    auto compiler = shc::Compiler();
    auto result = compiler.Compile(shaderPath.string().c_str(), "main", "cs_6_5");
}
