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
    float4 PosLocal : POSITION;
    float4 Tex : TEXCOORD;
};



Output Wave_VS(Input _Input)
{
    Output NewOutPut = (Output) 0; 
    NewOutPut.Pos = _Input.Pos;
    NewOutPut.Tex = _Input.Tex;
    return NewOutPut;
    

}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);
float4 Wave_PS(Output _Input) : SV_Target0
{   
    // Option00 -> OnOff플래그
    if (OnOff == 0)
    {
        return Tex.Sample(Smp, _Input.Tex.xy);
    }

    
    // 웨이브
    float2 TexPos = _Input.Tex.xy;
    float2 uv = (_Input.Pos.x / 1280.0f, _Input.Pos.y / 720.0f); // uv : 0 ~ 1

    uv.x = (sin(uv.y * 15 + SumDeltaTime * 30.0f)) / 60.0f;
    uv.y = 0;
    
    float2 SmpPos = float2(TexPos.x + uv.x, TexPos.y + uv.y);
    float4 Color = Tex.Sample(Smp, SmpPos);
    
    
    return Color;
}

