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

Output DistortionGlitch_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    NewOutPut.Pos = _Input.Pos;
    NewOutPut.Tex = _Input.Tex;
    return NewOutPut;
}





// 참고 : https://www.shadertoy.com/view/ldjGzV
Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float Noise(Output _Input, float2 p)
{
    float2 SmpPos = _Input.Tex.xy;
    SmpPos.x = 2 * cos(SumDeltaTime) * SumDeltaTime * 8.0f + p * 1.0f;
    
    float s = Tex.Sample(Smp, SmpPos);
    
    return s;
}

float CheckOnOff(float a, float b, float c)
{
    return step(c, sin(SumDeltaTime + a * cos(SumDeltaTime * b)));
}

float Ramp(float y, float start, float end)
{
    float inside = step(start, y) - step(end, y);
    float fact = (y - start) / (end - start) * inside;
    return (1.0f - fact) * inside;

}

float Stripe(Output _Input, float2 uv)
{
    float noi = Noise(_Input, uv * float2(0.5f, 1.0f) + float2(1.0f, 3.0f));
    return Ramp(fmod(uv.y * 5.0f + SumDeltaTime / 2.0f + sin(SumDeltaTime + sin(SumDeltaTime * 0.63f)), 1.0f), 0.5f, 0.6f) * noi;

}

float3 GetVideo(Output _Input, float2 uv)
{
    float2 look = uv;
    
    float window = 1.0f / (1.0f + 20.0f * (look.y - fmod(SumDeltaTime / 4.0f, 1.0f)) * (look.y - fmod(SumDeltaTime / 4.0f, 1.0f)));
    look.x = look.x + sin(look.y * 10.0f + SumDeltaTime) / 50.0f * CheckOnOff(4.0f, 4.0f, 0.3f) * (1.0f + cos(SumDeltaTime * 80.0f)) * window;
    float vShift = 0.4f * CheckOnOff(2.0f, 3.0f, 0.9f) * (sin(SumDeltaTime) * sin(SumDeltaTime * 20.0f) +
    (0.5f + 0.1f * sin(SumDeltaTime * 200.0f) * cos(SumDeltaTime)));
    
    look.y = fmod(look.y + vShift, 1.0f);
    
    float3 video = Tex.Sample(Smp, look);
    return video;
}

float2 ScreenDistort(float2 uv)
{
    uv -= float2(0.5f, 0.5f);
    uv = uv * 1.2f * (1.0f / 1.2f + 2.0f * uv.x * uv.x * uv.y * uv.y);
    uv += float2(0.5f, 0.5f);
    
    return uv;
}

float4 DistortionGlitch_PS(Output _Input) : SV_Target0
{
    // Option00 -> OnOff플래그
    if (OnOff == 0)
    {
        return Tex.Sample(Smp, _Input.Tex.xy);
    }

    float2 TexPos = _Input.Tex.xy;
    float2 SmpPos;
    
    SmpPos = ScreenDistort(TexPos);
    float3 video = GetVideo(_Input, SmpPos);
    float vigAmt = 3.0f + 0.3f * sin(SumDeltaTime + 5.0f * cos(SumDeltaTime * 5.0f));
    float vignette = (1.0f - vigAmt * (SmpPos.y - 0.5f) * (SmpPos.y - 0.5f)) * (1.0f - vigAmt * (SmpPos.x - 0.5f) * (SmpPos.x - 0.5f));
    
    video += Stripe(_Input, SmpPos);
    video += Noise(_Input, SmpPos * 2.0f) / 2.0f;
    //video *= vignette;
    //video *= (12.0f + fmod(SmpPos.y * 30.0f + SumDeltaTime, 1.0f)) / 13.0f;
    
    float4 Color = float4(video.r, video.g, video.b, 1.0f);
    
    
    return Color;
}

