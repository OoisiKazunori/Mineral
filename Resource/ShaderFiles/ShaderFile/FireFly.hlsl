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
    float4 color;
    float timer;         //スケールの拡縮
    float2 moveTimer;   //リサージュ曲線用のタイマー
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

    particleBuffer[index].pos.base = RandVec3(shaderTable[index],800.0f,-800.0f);

    float scale = RandVec3(shaderTable[index],10.0f,5.0f).z;
    particleBuffer[index].scale.base = float3(scale,scale,scale);
    particleBuffer[index].color = float4(1.0f,1.0f,1.0f,1.0f);
    particleBuffer[index].timer = 0;
    particleBuffer[index].moveTimer = RandVec3(shaderTable[index],10.0f,1.0f).xy;
    particleBuffer[index].shrinkScale = 0.0f;
    particleBuffer[index].shrinkFlag = 1;
}

[numthreads(1024, 1, 1)]
void UpdateMain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    ParticleUpdate updateData = particleBuffer[index];

 

    //通常時挙動--------------------------------

    //拡縮
    if(updateData.timer % 40 == 0)
    {
        updateData.shrinkFlag = !updateData.shrinkFlag;
    }
    float maxValue = 10.0f;
    if(updateData.shrinkFlag)
    {
        updateData.shrinkScale += 0.1f;
        //updateData.shrinkScale = Easing_Circ_Out(updateData.timer,40,0.0f,maxValue);
    }
    else
    {
        updateData.shrinkScale -= 0.1f;
       // updateData.shrinkScale = maxValue - Easing_Circ_Out(updateData.timer,40,0.0f,maxValue);
    }

    //登場--------------------------------
    float3 scale = float3(0.0f,0.0f,0.0f);
    if(appearFlag)
    {
        scale = updateData.scale.base;
    }
    else
    {
        updateData.shrinkScale = 0.0f;
    }
    updateData.scale.lerp = lerp(updateData.scale.lerp,scale + float3(updateData.shrinkScale,updateData.shrinkScale,0.0f),0.1f);

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
