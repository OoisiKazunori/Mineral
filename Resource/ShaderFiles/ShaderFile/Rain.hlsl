#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"
#include"../ShaderHeader/Ease.hlsli"

struct ParticleUpdate
{
    float3 pos;
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
    uint appearFlag;
};

RWStructuredBuffer<ParticleUpdate> particleBuffer : register(u0);
RWStructuredBuffer<uint> shaderTable : register(u1);
AppendStructuredBuffer<OutputData> drawData : register(u1);

[numthreads(1024, 1, 1)]
void InitMain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    particleBuffer[index].pos.x = RandVec3(shaderTable[index],800.0f * 2.0f,-800.0f * 2.0f).x;
    particleBuffer[index].pos.y = 100.0f;
    particleBuffer[index].pos.z = RandVec3(shaderTable[index],800.0f * 2.0f,-800.0f * 2.0f).z;
}

[numthreads(1024, 1, 1)]
void UpdateMain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    ParticleUpdate updateData = particleBuffer[index];
    //通常時挙動--------------------------------
    float4 color = float4(1,1,1,1);
    if(!appearFlag)
    {
        color.a = 0.0f;
    }
    //上下移動
    if(updateData.pos.y <= 0.0f)
    {
        updateData.pos.y = 1000.0f;
    }
    else
    {
        updateData.pos.y -= 1.0f;
    }

    particleBuffer[index] = updateData;
    matrix worldMat = CalucurateWorldMat(particleBuffer[index].pos,float3(1.0f,100.0f,1.0f),float3(0,0,0),billboard);
    
    OutputData outputMat;
    outputMat.mat = mul(viewProjectionMat,worldMat);
    outputMat.color = color;
    drawData.Append(outputMat);
}
