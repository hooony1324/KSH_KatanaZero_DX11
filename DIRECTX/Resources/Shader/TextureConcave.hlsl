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
float4 TextureConcave_PS(Output _Input) : SV_Target0
{   
    float2 TexPos = _Input.Tex.xy;
    
    if(ConcaveByPlayerPos.y >= TexPos.y && ConcaveByPlayerPos.y > -1.0f)
    {
        clip(-1);
    }
  

    float4 Color = Tex.Sample(Smp, TexPos);
    return Color;
}

