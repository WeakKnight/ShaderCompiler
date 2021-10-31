# ShaderCompiler

ShaderCompiler is a wrapper of [Slang](https://github.com/shader-slang/slang).

## Features
* Slang To HLSL
* Slang To DXIL
* Reflection API

## Examples
### Slang
```
cbuffer CB
{
    int frameIndex;
    int bufferSize;
}

StructuredBuffer<float> buffer0;
StructuredBuffer<float> buffer1;
RWStructuredBuffer<float> result;
StructuredBuffer<float4> bufferArray[_BUFFER_SIZE];

[numthreads(16,16,1)]
void main(uint3 threadId : SV_DispatchThreadID)
{
    uint index = threadId.x;
    result[index] = buffer0[index] + buffer1[index];
}
```
### C++
```
auto compiler = shc::Compiler();
compiler.AddSearchPath("include/");

shc::DefineList defines;
defines.AddDefine("_BUFFER_SIZE", "42");

std::shared_ptr<shc::Compiler::Result> result = compiler.Compile("shader.hlsl", "main", "cs_6_5", defines, shc::Compiler::Target::DXIL);

// Reflection Information
assert(result->variables["bufferArray"].count == 42);

// Binary
assert(result->data != nullptr);
assert(result->size > 0);
```
