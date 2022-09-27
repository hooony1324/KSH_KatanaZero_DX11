#include "TransformHeader.fx"
#include "CustomRenderOption.fx"

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
    float4 PivotPos;
};

cbuffer MaskData : register(b2)
{
    float2 MaskTextureFramePos;
    float2 MaskTextureFrameSize;
    float4 MaskPivotPos;
}

Output TextureMask_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    NewOutPut.Pos = mul(_Input.Pos + PivotPos, WorldViewProjection);
    NewOutPut.PosLocal = _Input.Pos;
    
    NewOutPut.Tex0.x = (_Input.Tex.x * TextureFrameSize.x) + TextureFramePos.x;
    NewOutPut.Tex0.y = (_Input.Tex.y * TextureFrameSize.y) + TextureFramePos.y;
    
    NewOutPut.Tex1.x = (_Input.Tex.x * MaskTextureFrameSize.x) + MaskTextureFramePos.x;
    NewOutPut.Tex1.y = (_Input.Tex.y * MaskTextureFrameSize.y) + MaskTextureFramePos.y;

    
    return NewOutPut;
}

cbuffer PixelData : register(b0)
{
    float4 MulColor;
    float4 PlusColor;
    float4 Slice;
}


Texture2D Tex : register(t0);
Texture2D Mask : register(t1);
SamplerState Smp : register(s0);
float4 TextureMask_PS(Output _Input) : SV_Target0
{
    // 마스킹
    float4 MaskColor = Mask.Sample(Smp, _Input.Tex1.xy);
    if ( 1 == MaskColor.r && 1 == IsMask)
    {
        clip(-1);
    }
    
    if (_Input.Tex0.x < Slice.x)
    {
        clip(-1);
    }
    
 
    
    float4 RenderColor = (Tex.Sample(Smp, _Input.Tex0.xy) * MulColor) + PlusColor;
    return RenderColor;

}
