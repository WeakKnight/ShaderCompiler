#include <scene.hlsli>

cbuffer CB
{
    int frameIndex;
    int bufferSize;
}

struct Foo
{
    int a;
    int b;
    uint c;
    float d;
};
ConstantBuffer<Foo> cb;

StructuredBuffer<float> buffer0;
StructuredBuffer<float> buffer1;
RWStructuredBuffer<float> result;

StructuredBuffer<float4> bufferArray[_BUFFER_SIZE];

SamplerState materialSampler;
RWTexture2D<float4> outputTexture;

[numthreads(16,16,1)]
void main(uint3 threadId : SV_DispatchThreadID)
{
    uint index = threadId.x;
    result[index] = buffer0[index] + buffer1[index];
}