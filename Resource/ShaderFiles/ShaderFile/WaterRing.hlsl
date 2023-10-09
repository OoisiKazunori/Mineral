#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"
#include"../ShaderHeader/Ease.hlsli"

struct ParticleUpdate
{
    float3 pos;
    float3 scale;
    int timer;
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
    float3 playerPos;
    uint appearFlag;
};

RWStructuredBuffer<ParticleUpdate> particleBuffer : register(u0);
RWStructuredBuffer<uint> shaderTable : register(u1);
RWStructuredBuffer<OutputData> drawData : register(u2);

static const float range = 400.0f;
[numthreads(1024, 1, 1)]
void InitMain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    particleBuffer[index].pos.x = RandVec3(shaderTable[index],range,-range).x;
    particleBuffer[index].pos.z = RandVec3(shaderTable[index],range,-range).z;
    particleBuffer[index].pos.y = 5.0f;
    particleBuffer[index].timer = 0;
    particleBuffer[index].scale = float3(0.0f,0.0f,0.0f);
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
    else
    {
        color.a = 1.0f;
    }
    ++updateData.timer;
    //拡大終了
    if(60 <= updateData.timer)
    {
        updateData.pos.x = RandVec3(shaderTable[index],range,-range).x;
        updateData.pos.z = RandVec3(shaderTable[index],range,-range).z;
        updateData.scale = float3(0.0f,0.0f,0.0f);
        updateData.timer = 0;        
    }
    //拡大
    else
    {
        updateData.scale.x = updateData.timer * 6.0f;
        updateData.scale.y = updateData.timer * 6.0f;
        updateData.scale.z = updateData.timer * 6.0f;
    }

    particleBuffer[index] = updateData;
    matrix worldMat = CalucurateWorldMat(particleBuffer[index].pos,float3(10.0f,10.0f,1.0f),float3(0,90,0),MatrixIdentity());
    
    OutputData outputMat;
    outputMat.mat = mul(viewProjectionMat,worldMat);
    outputMat.color = color;
    drawData[index] = (outputMat);
}
