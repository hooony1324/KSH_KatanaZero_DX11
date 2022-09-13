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


Output Distortion_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    NewOutPut.Pos = _Input.Pos;
    NewOutPut.Tex = _Input.Tex;
    return NewOutPut;
}

float rand(float2 p)
{
    float t = floor(SumDeltaTime * 10.0f) / 30.0f;
    return frac(sin(dot(p, float2(t * 12.9898f, t * 78.233f))) * 43758.5453);
}

float noise(float2 uv, float blockiness)
{
    float2 lv = frac(uv);
    float2 id = floor(uv);
    
    float n1 = rand(id);
    float n2 = rand(id + float2(1, 0));
    float n3 = rand(id + float2(0, 1));
    float n4 = rand(id + float2(1, 1));
    
    float2 u = smoothstep(0.0f, 1.0f + blockiness, lv);
    
    return lerp(lerp(n1, n2, u.x), lerp(n3, n4, u.x), u.y);

}

float fbm(float2 uv, int count, float blockiness, float complexity)
{
    float val = 0.0f;
    float amp = 0.5f;
    
    while (count != 0)
    {
        val += amp * noise(uv, blockiness);
        amp *= 0.5f;
        uv *= complexity;
        count--;
    }

    return val;
}

#define glitchAmplitude 0.9 // increase
#define glitchNarrowness 2.0
#define glitchBlockiness 2.0
#define glitchMinimizer 6.0 // decrease

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);
float4 Distortion_PS(Output _Input) : SV_Target0
{
    // Option00 -> OnOffÇÃ·¡±×
    if (OnOff == 0)
    {
        return Tex.Sample(Smp, _Input.Tex.xy);
    }

    float2 TexPos = _Input.Tex.xy;

    float aspect = _Input.Pos.x / _Input.Pos.y;
    float2 a = float2(TexPos.x * aspect, TexPos.y);
    float2 uv2 = float2(a.x / _Input.Pos.x, exp(a.y));
    
    float shift = glitchAmplitude * pow(fbm(uv2, 4, glitchBlockiness, glitchNarrowness), glitchMinimizer);
    
    float colR = Tex.Sample(Smp, float2(TexPos.x + shift, TexPos.y)).r * (1.0 - shift);
    float colG = Tex.Sample(Smp, float2(TexPos.x - shift, TexPos.y)).g * (1.0 - shift);
    float colB = Tex.Sample(Smp, float2(TexPos.x - shift, TexPos.y)).b * (1.0 - shift);
    
    float4 Result = float4(colR, colG, colB, 1.0f);
    return Result;
}
