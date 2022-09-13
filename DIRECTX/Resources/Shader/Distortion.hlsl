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

float random2d(float2 n)
{
    return frac(sin(dot(n, float2(12.9898f, 4.1414f))) * 43758.5453f);
}

float randomRange(float2 seed, float min, float max)
{
    return min + random2d(seed) * (max - min);
}

float insideRange(float v, float bottom, float top)
{
    return step(bottom, v) - step(top, v);
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

#define AMT 0.2 //0 ~ 1 glitch amount
#define SPEED 0.3 //0 ~ 1 speed
float4 Distortion_PS(Output _Input) : SV_Target0
{

    
    // Option00 -> OnOff플래그
    if (OnOff == 0)
    {
        return Tex.Sample(Smp, _Input.Tex.xy);
    }

    float time = floor(SumDeltaTime * SPEED * 60.0f);
    float2 TexPos = _Input.Tex.xy;
    float2 uv = (_Input.Pos.x / 1280.0f, _Input.Pos.y / 720.0f); // uv : 0 ~ 1

    // 원본 가져오고
    float3 Color = Tex.Sample(Smp, TexPos).rgb;

    // 랜덤하게 수평 자르기
    float maxOffset = AMT / 2.0f;
    for (float i = 0.0f; i < 10.0f * AMT; i += 1.0f)
    {
        float sliceY = random2d(float2(time, 2345.0f + float(i)));
        float sliceH = random2d(float2(time, 9035.0f + float(i))) * 0.25f;
        float hOffset = randomRange(float2(time, 9625.0f + float(i)), -maxOffset, maxOffset);
        float uvOff = TexPos;
        uvOff.x += hOffset;
        if (insideRange(TexPos.y, sliceY, frac(sliceY + sliceH)) == 1.0f)
        {
            Color = Tex.Sample(Smp, uvOff).rgb;
        }
    }
    
    // 결과 도출
    float maxColorOffset = AMT / 6.0f;
    float rnd = random2d(float2(time, 9545.0f));
    float2 colorOffset = float2(randomRange(float2(time, 9545.0f), -maxColorOffset, maxColorOffset),
    randomRange(float2(time, 7205.0f), -maxColorOffset, maxColorOffset));
    
    if (rnd < 0.33f)
    {
        Color.r = Tex.Sample(Smp, TexPos + colorOffset).r;
    }
    else if (rnd < 0.66f)
    {
        Color.g = Tex.Sample(Smp, TexPos + colorOffset).g;
    }
    else
    {
        Color.b = Tex.Sample(Smp, TexPos + colorOffset).b;
    }
    
    float4 Result = float4(Color.rgb, 1.0f);
    return Result;
}
