#include "TransformHeader.fx"
#include "CustomRenderOption.fx"

struct Input
{
    float4 Pos : POSITION;
    float4 Tex : TEXCOORD;
};

struct Output
{
    float4 Pos : SV_POSITION;
    float4 Tex : TEXCOORD;
};

cbuffer AtlasData : register(b1)
{
    float2 TextureFramePos;
    float2 TextureFrameSize;
    float4 PivotPos;
};

cbuffer ConcaveData : register(b2)
{
    float2 ConcaveByPlayerPos;
    float2 ConcaveByBossPos;
    float ConcaveStrength;
}

Output TextureConcave_VS(Input _Input)
{
    Output NewOutPut = (Output) 0; 
    NewOutPut.Pos = mul(_Input.Pos + PivotPos, WorldViewProjection);
   
    NewOutPut.Tex.x = (_Input.Tex.x * TextureFrameSize.x) + TextureFramePos.x;
    NewOutPut.Tex.y = (_Input.Tex.y * TextureFrameSize.y) + TextureFramePos.y;
    
    return NewOutPut;
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);
#define ConcavePlayerRange 0.1
#define ConcaveBossRange 0.05
float4 TextureConcave_PS(Output _Input) : SV_Target0
{   
    float2 TexPos = _Input.Tex.xy;
    
    // 플레이어
    if(ConcaveByPlayerPos.y >= TexPos.y && ConcaveByPlayerPos.y > -1.0f)
    {
        float DistanceRatio = abs(TexPos.x - ConcaveByPlayerPos.x);
        if (DistanceRatio <= ConcavePlayerRange)
        {
            TexPos.y += -ConcavePlayerRange + DistanceRatio;
        }
    }
    
    // 보스 주사기
    if (ConcaveByBossPos.y >= TexPos.y && ConcaveByBossPos.y > -1.0f)
    {
        float DistanceRatio = abs(TexPos.x - ConcaveByBossPos.x);
        if (DistanceRatio <= ConcaveBossRange)
        {
            float Diff = (1 / ConcaveBossRange) * pow(TexPos.x - ConcaveByBossPos.x, 2); // -> 0 ~ 0.1
            TexPos.y += -(ConcaveBossRange * ConcaveStrength) + Diff * ConcaveStrength;
        }
    }
    

  

    float4 Color = Tex.Sample(Smp, TexPos);
    
    if (Color.r <= 0.05f)
    {
        Color.r = 0.2f;
        Color.g = 0.1f;
        Color.b = 0.1f;
        Color.a = 0.7f;
    }
    
    return Color;
}

