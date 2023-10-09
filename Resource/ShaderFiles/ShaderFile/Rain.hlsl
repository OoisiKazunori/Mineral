#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"
#include"../ShaderHeader/Ease.hlsli"

struct ParticleUpdate
{
    float3 basePos;
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
    float3 playerPos;
    uint appearFlag;
};

RWStructuredBuffer<ParticleUpdate> particleBuffer : register(u0);
RWStructuredBuffer<uint> shaderTable : register(u1);
RWStructuredBuffer<OutputData> drawData : register(u1);

[numthreads(1024, 1, 1)]
void InitMain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    float range = 400.0f;
    particleBuffer[index].pos.x = RandVec3(shaderTable[index],range,-range).x;
    particleBuffer[index].pos.z = RandVec3(shaderTable[index],range,-range).z;

    int randomTableIndex = 0;
    while(!(550.0f <= particleBuffer[index].pos.y))
    {
        uint randowmIndex = shaderTable[index + randomTableIndex];
        ++randomTableIndex;
        particleBuffer[index].pos.y = RandVec3(randowmIndex,1000.0f,500.0f).y;
    }
    

    particleBuffer[index].basePos = particleBuffer[index].pos;
}

[numthreads(1024, 1, 1)]
void UpdateMain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    if(1024 / 2 <= index)
    {
        return;
    }
    ParticleUpdate updateData = particleBuffer[index];
    //通常時挙動--------------------------------
    float4 color = float4(1,1,1,1);
    if(!appearFlag)
    {
        color.a = 0.0f;
    }
    else
    {
        color.a = 0.1f;
    }
    //上下移動
    if(updateData.pos.y <= -100.0f)
    {
        updateData.pos.x = updateData.basePos.x + playerPos.x;
        updateData.pos.y = updateData.basePos.y;
        updateData.pos.z = updateData.basePos.z + playerPos.z;
    }
    else
    {
        updateData.pos.y -= 10.0f;
    }

    particleBuffer[index] = updateData;
    matrix worldMat = CalucurateWorldMat(particleBuffer[index].pos,float3(1.0f,50.0f,1.0f),float3(0,0,0),billboard);
    
    OutputData outputMat;
    outputMat.mat = mul(viewProjectionMat,worldMat);
    outputMat.color = color;
    drawData[index] = (outputMat);
}
