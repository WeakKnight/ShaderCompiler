#include <filesystem>
#include "compiler.h"

int main(int argc, char** argv) 
{
    std::filesystem::path exePath = argv[0];
    std::filesystem::path shaderPath = exePath.parent_path() / "shaders/test.hlsl";

    auto compiler = shc::Compiler();
    compiler.AddSearchPath(shaderPath.parent_path().string());

    shc::DefineList defines;
    defines.AddDefine("_BUFFER_SIZE", "42");
    defines.AddDefine("_RAY_TRACING", "");

    auto result = compiler.Compile(shaderPath.string().c_str(), "main", "cs_6_5", defines);

    shc::Variable var = result->variables["bufferArray"];
}
