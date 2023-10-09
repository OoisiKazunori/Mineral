#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"
#include"../ShaderHeader/Ease.hlsli"

struct ParticleUpdate
{
    float3 pos;
    float3 scale;
    int timer;
    int coolTimer;
    int coolMaxTimer;
    int coolTimeFlag;
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

    particleBuffer[index].coolTimeFlag = 1;
    particleBuffer[index].coolMaxTimer = RandVec3(shaderTable[index],60 * 4,30).x;
    particleBuffer[index].coolTimer = 0;
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

    //一度発生したら間隔をあける
    if(updateData.coolTimeFlag)
    {
        ++updateData.coolTimer;

        if(updateData.coolMaxTimer <= updateData.coolTimer)
        {
            updateData.coolTimeFlag = 0;
            updateData.coolTimer = 0;
        }
    }
    //通常時
    else
    {
        ++updateData.timer;
    }

    const float MAX_TIMER = 30;
    //拡大終了
    if(MAX_TIMER <= updateData.timer)
    {
        updateData.pos.x = RandVec3(shaderTable[index + updateData.pos.x],range,-range).x;
        updateData.pos.z = RandVec3(shaderTable[index + updateData.pos.z],range,-range).z;
        updateData.scale = float3(0.0f,0.0f,0.0f);
        updateData.timer = 0;
        updateData.coolTimeFlag = 1;
    }
    //拡大
    else
    {
        float rate = (float)updateData.timer / MAX_TIMER;
        updateData.scale.x = rate * 10.0f;
        updateData.scale.y = rate * 10.0f;
        updateData.scale.z = rate * 10.0f;
        color.a = 1.0f - rate;
    }

    particleBuffer[index] = updateData;
    matrix worldMat = CalucurateWorldMat(particleBuffer[index].pos, updateData.scale,float3(90,0,0),MatrixIdentity());
    
    OutputData outputMat;
    outputMat.mat = mul(viewProjectionMat,worldMat);
    outputMat.color = color;
    drawData[index] = (outputMat);
}
