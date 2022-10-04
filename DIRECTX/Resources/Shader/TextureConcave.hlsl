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

Output TextureWave_VS(Input _Input)
{
    Output NewOutPut = (Output) 0; 
    NewOutPut.Pos = mul(_Input.Pos + PivotPos, WorldViewProjection);
   
    NewOutPut.Tex.x = (_Input.Tex.x * TextureFrameSize.x) + TextureFramePos.x;
    NewOutPut.Tex.y = (_Input.Tex.y * TextureFrameSize.y) + TextureFramePos.y;
    
    return NewOutPut;
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);
float4 TextureWave_PS(Output _Input) : SV_Target0
{   
    float2 TexPos = _Input.Tex.xy;
    
    // 웨이브 ver1
    if (Version == 0)
    {
        float2 uv = (_Input.Pos.x / 1280.0f, _Input.Pos.y / 720.0f); // uv : 0 ~ 1
        uv.x = (sin(uv.y * 10 + SumDeltaTime * 2.0f)) / 100.0f;
        uv.y = fmod(-SumDeltaTime * 0.05f, 1.0f);
        float newTexY = TexPos.y + uv.y;
        if (newTexY < 0.0f)
        {
            newTexY = 1.0f + newTexY;
        }
        TexPos = float2(TexPos.x + uv.x, newTexY);
    }
    // 웨이브 ver2
    else if (Version == 1)
    {
        float POWER = 0.04f; // 좌우 흔들림
        float VERTICAL_SPREAD = 5.0f; // vertically
        float ANIM_SPEED = 0.4f;
    
        float y = (TexPos.y + SumDeltaTime * ANIM_SPEED) * VERTICAL_SPREAD;
    
        TexPos.x += (
        //sin(y)                            // 배경쪽 큰 흔들림
        +sin(y * 10.0f) * 0.2f
        //+ sin(y * 50.0f) * 0.03f          // 잔 주름
        )
        * POWER
        * sin(TexPos.y * 3.14f);
    }


    float4 Color = Tex.Sample(Smp, TexPos);
    return Color;
}

