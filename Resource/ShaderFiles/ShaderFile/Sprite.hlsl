struct VSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float2 uv : TEXCOORD; //uv�l
};

cbuffer MatBuffer : register(b0)
{
    matrix mat; //3D�ϊ��s��
}

cbuffer ColorBuffer : register(b1)
{
    float4 color; //3D�ϊ��s��
}

VSOutput VSmain(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput op;
	op.svpos = mul(mat, pos);
	op.uv = uv;
	return op;
}

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 PSmain(VSOutput input) : SV_TARGET
{
    float4 output = float4(tex.Sample(smp, input.uv));    
    return output;
}

//アルファ対応
float4 PSAlphaMain(VSOutput input) : SV_TARGET
{
    float4 output = float4(tex.Sample(smp, input.uv));    
    if (output.w * color.w < 0.01f)
    {
        discard;
    }
    return output * color;
}


struct VSOutputColor
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float2 uv : TEXCOORD; //uv�l
    float4 color : COLOR;
};

struct OutputData
{
    matrix mat; //�V�X�e���p���_���W
    float4 color; //uv�l
};
RWStructuredBuffer<OutputData> matBuffer : register(u0);

VSOutputColor InstanceVSMain(float4 pos : POSITION, float2 uv : TEXCOORD,uint id : SV_InstanceID)
{
	VSOutputColor op;
	op.svpos = mul(matBuffer[id].mat, pos);
	op.uv = uv;
    op.color = matBuffer[id].color;
	return op;
}

float4 InstancePSMain(VSOutputColor input) : SV_TARGET
{
    float4 output = float4(tex.Sample(smp, input.uv)) * input.color;    
    return float4(1,1,1,1);
}