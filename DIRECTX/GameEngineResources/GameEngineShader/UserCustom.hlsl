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
    float4 Tex0 : TEXCOORD0; // 애니메이션
    float4 Tex1 : TEXCOORD1; // 마스크
};



cbuffer AtlasData : register(b1)
{
    float2 TextureFramePos;
    float2 TextureFrameSize;
};


Output UserCustom_VS(Input _Input)
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
    
    NewOutPut.Tex0.x = (_Input.Tex.x * TextureFrameSize.x) + TextureFramePos.x;
    NewOutPut.Tex0.y = (_Input.Tex.y * TextureFrameSize.y) + TextureFramePos.y;
    
    NewOutPut.Tex1 = _Input.Tex;

    
    return NewOutPut;
}

cbuffer ColorData : register(b0)
{
    float4 MulColor;
    float4 PlusColor;
}

Texture2D Tex : register(t0);
Texture2D Mask : register(t1);
SamplerState Smp : register(s0);
float4 UserCustom_PS(Output _Input) : SV_Target0
{
    float4 MaskColor = Mask.Sample(Smp, _Input.Tex1.xy);
    
    if (/*1 == MaskColor.r && */1 == IsMask)
    {
        clip(-1);

    }
    
    float4 RenderColor = (Tex.Sample(Smp, _Input.Tex0.xy) * MulColor) + PlusColor;
    
    return RenderColor;

}
