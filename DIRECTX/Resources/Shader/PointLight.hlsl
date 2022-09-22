#include "CustomRenderOption.fx"
#include "TransformHeader.fx"

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

cbuffer LightData : register(b0)
{
    float4 LightPos; // LocalPosition
}

Output PointLight_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    NewOutPut.Pos = _Input.Pos;
    NewOutPut.Tex = _Input.Tex;
    return NewOutPut;
}

// 참고 : https://www.shadertoy.com/view/fttyz2
Texture2D Tex : register(t0);
SamplerState Smp : register(s0);
float4 PointLight_PS(Output _Input) : SV_Target0
{

    // Option00 -> OnOff플래그
    if (OnOff == 0)
    {
        return Tex.Sample(Smp, _Input.Tex.xy);
    }
    
    float2 TexPos = _Input.Tex.xy;
    float2 SVPos = _Input.Pos;
    float4 Color = float4(0, 0, 0, 0);
 
    
    
    //float2 Orbit = float2(sin(SumDeltaTime), cos(SumDeltaTime));
    //Orbit = float2(-0.5f, 0.7f);
    //float Circle = 1.0f - distance(TexPos - 0.5f, Orbit * 0.5f);

    //float LightStrength = 0.75f + sin(SumDeltaTime * 5.0f) * 0.25f;
    //float2 Offset = Orbit * LightStrength * 0.01f;
    //Offset = Orbit * 0.01f;
    
    //float3 Target = float3(0.0f, 1.0f, 0.0f); // 이 색을 크로마키 타겟에서 찾음
    //float3 Light = float3(1.0f, 0.5f, 0.0f); // 빛 색
    //float3 Background = float3(0.0f, 0.0f, 0.1f) + Circle * LightStrength * Light;
    ////float3 Background = Tex.Sample(Smp, _Input.Tex.xy).rgb;
    
    //float Threshold = 0.7f; // 타겟의 컬러범위 정도
    //float Softness = 0.1f;  // 보간 정도
    
    //// 배경
    //float4 Col = Tex.Sample(Smp, TexPos);
    //float Diff = distance(Col.xyz, Target.xyz) - Threshold;
    //float Factor = clamp(Diff / Softness, 0.0f, 1.0f);
    
    //// 림라이트 엣지
    //float4 ColOffset = Tex.Sample(Smp, TexPos + Offset);
    //float DiffOffset = distance(ColOffset.xyz, Target) - Threshold;
    //float LightFactor = 1.0f - clamp(DiffOffset / Softness, 0.0f, 1.0f);

    
    //Col.xyz *= float3(1.0f, 0.7f, 0.9f);
    //Color = float4(lerp(Background, Col.xyz + Light * LightFactor, Factor), Col.a);
    
    
    
    //// SV_Position : 좌상단 {0, 0} 우하단 {1280, 720} -> 스크린
    //Color = Tex.Sample(Smp, TexPos);
    
    //// 원 만들어보기
    //float2 CirclePos = LightPos;
    //float Distance = length(CirclePos - SVPos);
 
    //float Ambient = Color.r + Color.g + Color.b;    // 전체적인 밝기
    //float Radius = 100.0f;                          // 원 크기
    
    //if (Distance <= Radius)
    //{
    //    // 어두운 픽셀중 원래 텍스쳐의 초록색 성향을 강화
    //    if (Ambient <= 0.3f)
    //    {
    //        Color.g *= clamp(Distance / 60.0f, 1.5f, 4.0f);
    //    }
        
    //    // 밝은 픽샐은 초록색으로
    //    if (Ambient >= 1.0f)
    //    {
    //        Color.r *= clamp(Distance / 30.0f, 1.5f, 4.0f);
    //        Color.b *= clamp(Distance / 30.0f, 1.5f, 4.0f);
    //    }

    //}

    return Color;
}

