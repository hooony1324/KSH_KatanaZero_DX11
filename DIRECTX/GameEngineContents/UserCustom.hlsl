#include "TransformHeader.fx"
#include "RenderOption.fx"

// 0                                                                                                1 
// 0□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□1

struct Input
{
    float4 Pos : POSITION;
    float4 Tex : TEXCOORD;
};

struct Output
{
    float4 Pos : SV_POSITION;
    float4 PosLocal : POSITION;
    float4 Tex : TEXCOORD;
};



cbuffer AtlasData : register(b1)
{
    float2 TextureFramePos;
    float2 TextureFrameSize;
};


Output TextureAtlas_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    NewOutPut.Pos = mul(_Input.Pos, WorldViewProjection);
    NewOutPut.PosLocal = _Input.Pos;
    
    // 버텍스가 몇번째 버텍스 인지 알수가 없다.
    // NewOutPut.Tex
    // 00    10
    
    //// 10    11
    
    //TextureFrameSize.x -= 0.5f;
    //TextureFrameSize.y -= 0.5f;
    //TextureFramePos.x -= 0.5f;
    //TextureFramePos.y -= 0.5f;
    
    NewOutPut.Tex.x = (_Input.Tex.x * TextureFrameSize.x) + TextureFramePos.x;
    NewOutPut.Tex.y = (_Input.Tex.y * TextureFrameSize.y) + TextureFramePos.y;
    
    return NewOutPut;
}

cbuffer ColorData : register(b0)
{
    float4 MulColor;
    float4 PlusColor;
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);
float4 TextureAtlas_PS(Output _Input) : SV_Target0
{
    return (Tex.Sample(Smp, _Input.Tex.xy) * MulColor) + PlusColor;
}
