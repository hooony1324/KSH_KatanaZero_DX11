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
    float4 Tex : TEXCOORD;
};

cbuffer PointLightData
{
    float4 LightWorldPos;
    float4 LightScreenPos;
};


Output PointLight_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    NewOutPut.Pos = mul(_Input.Pos, WorldViewProjection);
    NewOutPut.Tex = _Input.Tex;
    

    return NewOutPut;
}

// ���� : https://www.shadertoy.com/view/fttyz2
Texture2D Tex : register(t0);
SamplerState Smp : register(s0);
float4 PointLight_PS(Output _Input) : SV_Target0
{

    // Option00 -> OnOff�÷���
    if (OnOff == 0)
    {
        return Tex.Sample(Smp, _Input.Tex.xy);
    }
    
    float2 TexPos = _Input.Tex.xy;
    float2 Pos = _Input.Pos.xy;
    float2 CenterPos = LightScreenPos.xy;
    float4 Color;
 
    
    
    //float2 Orbit = float2(sin(SumDeltaTime), cos(SumDeltaTime));
    //Orbit = float2(-0.5f, 0.7f);
    //float Circle = 1.0f - distance(TexPos - 0.5f, Orbit * 0.5f);

    //float LightStrength = 0.75f + sin(SumDeltaTime * 5.0f) * 0.25f;
    //float2 Offset = Orbit * LightStrength * 0.01f;
    //Offset = Orbit * 0.01f;
    
    //float3 Target = float3(0.0f, 1.0f, 0.0f); // �� ���� ũ�θ�Ű Ÿ�ٿ��� ã��
    //float3 Light = float3(1.0f, 0.5f, 0.0f); // �� ��
    //float3 Background = float3(0.0f, 0.0f, 0.1f) + Circle * LightStrength * Light;
    ////float3 Background = Tex.Sample(Smp, _Input.Tex.xy).rgb;
    
    //float Threshold = 0.7f; // Ÿ���� �÷����� ����
    //float Softness = 0.1f;  // ���� ����
    
    //// ���
    //float4 Col = Tex.Sample(Smp, TexPos);
    //float Diff = distance(Col.xyz, Target.xyz) - Threshold;
    //float Factor = clamp(Diff / Softness, 0.0f, 1.0f);
    
    //// ������Ʈ ����
    //float4 ColOffset = Tex.Sample(Smp, TexPos + Offset);
    //float DiffOffset = distance(ColOffset.xyz, Target) - Threshold;
    //float LightFactor = 1.0f - clamp(DiffOffset / Softness, 0.0f, 1.0f);

    
    //Col.xyz *= float3(1.0f, 0.7f, 0.9f);
    //Color = float4(lerp(Background, Col.xyz + Light * LightFactor, Factor), Col.a);
    
    
    
    //// SV_Position : �»�� {0, 0} ���ϴ� {1280, 720} -> ��ũ��
    Color = Tex.Sample(Smp, TexPos);
    
  
    // �� ������
    float Distance = length(Pos - CenterPos);
    
    float Ambient = Color.r + Color.g + Color.b; // ��ü���� ���
    float Radius = 100.0f; // �� ũ��
    
    if (Distance <= Radius)
    {
        // ��ο� �ȼ��� ���� �ؽ����� �ʷϻ� ������ ��ȭ
        if (Ambient <= 0.3f)
        {
            Color.g *= clamp(Distance / 60.0f, 1.5f, 4.0f);
        }
        
        // ���� �Ȼ��� �ʷϻ�����
        if (Ambient >= 1.0f)
        {
            Color.r *= clamp(Distance / 30.0f, 1.5f, 4.0f);
            Color.b *= clamp(Distance / 30.0f, 1.5f, 4.0f);
        }

    }
 
    return Color;
}

