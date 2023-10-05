#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"
#include"../ShaderHeader/Ease.hlsli"

struct LerpData
{
    float3 base;
    float3 lerp;
    float mul;
};

struct ParticleUpdate
{
    LerpData pos;
    LerpData scale;
    uint shrinkFlag;
    float shrinkScale;
    float shirinkBaseScale;
    float4 color;
    float scaleTimer;
    float scaleMaxTime;
    float timer;         //リサージュ曲線用のタイマー
    float2 moveTimer;   //リサージュ曲線用の倍率
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

    particleBuffer[index].pos.base.x = RandVec3(shaderTable[index],800.0f * 2.0f,-800.0f * 2.0f).x;
    particleBuffer[index].pos.base.y = RandVec3(shaderTable[index],100.0f,0.0f).y;
    particleBuffer[index].pos.base.z = RandVec3(shaderTable[index],800.0f * 2.0f,-800.0f * 2.0f).z;

    float scale = RandVec3(shaderTable[index],8.0f,5.0f).z;
    particleBuffer[index].scale.base = float3(scale,scale,scale);
    particleBuffer[index].color = float4(0.97f, 0.9f, 0.0f,1.0f);
    particleBuffer[index].timer = 0;
    particleBuffer[index].moveTimer = RandVec3(shaderTable[index],10.0f,1.0f).xy;
    particleBuffer[index].shrinkScale = 0.0f;
    particleBuffer[index].shrinkFlag = 1;
    particleBuffer[index].scaleMaxTime = RandVec3(shaderTable[index],3.0f,1.0f).z;
    particleBuffer[index].scaleTimer = 0.0f;
}

[numthreads(1024, 1, 1)]
void UpdateMain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    ParticleUpdate updateData = particleBuffer[index];

 

    //通常時挙動--------------------------------

    //拡縮
    const float MAX_TIME = 120;
    if(MAX_TIME <= updateData.scaleTimer)
    {
        updateData.shrinkFlag = !updateData.shrinkFlag;
        updateData.scaleTimer = 0;
    }
    float maxValue = 1.2f;
    //拡大
    if(updateData.shrinkFlag)
    {
        updateData.shrinkScale = 0.5f + Easing_Sine_In(updateData.scaleTimer,MAX_TIME,0.0f,maxValue);
    }
    //縮小
    else
    {
        updateData.shrinkScale = 0.5f + (maxValue - Easing_Sine_In(updateData.scaleTimer,MAX_TIME,0.0f,maxValue));
    }
    updateData.scaleTimer += updateData.scaleMaxTime;

    //登場--------------------------------
    if(appearFlag)
    {
        updateData.shirinkBaseScale = lerp(updateData.shirinkBaseScale,updateData.scale.base,0.01f);
    }
    else
    {
        updateData.shirinkBaseScale = 0.0f;
        updateData.shrinkScale = 0.0f;
    }

    updateData.scale.lerp = lerp(updateData.scale.lerp,updateData.shirinkBaseScale * float3(updateData.shrinkScale,updateData.shrinkScale,0.0f),0.1f);

    //リサージュ曲線
    float radius = 1.0f;
    updateData.pos.lerp = updateData.pos.base + 
    float3(
        cos(AngleToRadian(updateData.moveTimer.x * updateData.timer)) * radius,
        sin(AngleToRadian(updateData.moveTimer.y * updateData.timer)) * radius,
        0.0f
    );
    ++updateData.timer;

    if(updateData.pos.lerp.y <= -10.0f)
    {
        updateData.pos.lerp.x = -111111150.0f;
    }
    //通常時挙動--------------------------------
 
    particleBuffer[index] = updateData;
    matrix worldMat = CalucurateWorldMat(particleBuffer[index].pos.lerp,particleBuffer[index].scale.lerp,float3(0,0,0),billboard);
    float4 color = particleBuffer[index].color;
    OutputData outputMat;
    outputMat.mat = mul(viewProjectionMat,worldMat);
    outputMat.color = color;
    drawData.Append(outputMat);
}
