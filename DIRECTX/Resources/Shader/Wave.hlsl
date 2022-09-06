#include "TransformHeader.fx"
#include "RenderOption.fx"

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
    // Option00 -> OnOffÇÃ·¡±×
    if (Option00 == 0)
    {
        return Tex.Sample(Smp, _Input.Tex.xy);
    }

    float2 TexPos = _Input.Tex.xy;
    float2 uv = (_Input.Pos.x / 1280.0f, _Input.Pos.y / 1440.0f);

    uv.x = sin(uv.y * 10 + DeltaTime * 20.0f) / 10.0f;
    
    float2 SmpPos = float2(_Input.Tex.x + uv.x, _Input.Tex.y);
    float4 Color = Tex.Sample(Smp, SmpPos);

    return Color;
}

