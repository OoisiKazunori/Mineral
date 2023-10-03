#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

struct ParticleUpdate
{
    float3 pos;
    float3 scale;
    float4 color;
    uint timer;
};

struct OutputData
{
    matrix mat;
    float4 color;
};

cbuffer RootConstants : register(b0)
{
    matrix viewProjectionMat;
    matrix billboard;
};

RWStructuredBuffer<ParticleUpdate> particleBuffer : register(u0);
RWStructuredBuffer<uint> shaderTable : register(u1);
AppendStructuredBuffer<OutputData> drawData : register(u1);

[numthreads(1024, 1, 1)]
void InitMain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    particleBuffer[index].pos =   RandVec3(shaderTable[index],500.0f,-500.0f);
    particleBuffer[index].scale = float3(10.0f,10.0f,10.0f);
    particleBuffer[index].color = float4(1.0f,1.0f,1.0f,1.0f);
    particleBuffer[index].timer = 0;
}

[numthreads(1024, 1, 1)]
void UpdateMain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    matrix worldMat = CalucurateWorldMat(particleBuffer[index].pos,particleBuffer[index].scale,float3(0,0,0),billboard);
    float4 color = particleBuffer[index].color;

    OutputData outputMat;
    outputMat.mat = mul(viewProjectionMat,worldMat);
    outputMat.color = color;
    drawData.Append(outputMat);
}
