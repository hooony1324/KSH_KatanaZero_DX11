#include "CustomRenderOption.fx"
#include "TransformHeader.fx"

struct Input
{
    float4 Pos : POSITION;
    float4 Tex : TEXCOORD;
    
    uint Index : ROWINDEX;
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
    float LightRadius;
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
#define SpinSpeed 5.0
#define SpinRadius 10.0
#define LightStrength 1.0
float4 PointLight_PS(Output _Input) : SV_Target0
{
    // Option00 -> OnOff�÷���
    if (OnOff == 0)
    {
        return Tex.Sample(Smp, _Input.Tex.xy);
    }
    
    float4 Color;
    float2 TexPos = _Input.Tex.xy;
    float2 Pos = _Input.Pos.xy;
    float2 CenterPos = LightScreenPos.xy;

    
    //// SV_Position : �»�� {0, 0} ���ϴ� {1280, 720} -> ��ũ��
    Color = Tex.Sample(Smp, TexPos);     
    float Ambient = Color.r + Color.g + Color.b; // ��ü���� ���
    
    // ���� ��ο� �ȼ� ���� �ؽ����� �ʷϻ� ������ ��ȭ
    if (Ambient <= 0.5f && Ambient >= 0.05f)
    {
        // ��
        float2 Orbit = float2(sin(SumDeltaTime), cos(SumDeltaTime));
        
        CenterPos += float2(cos(SumDeltaTime * SpinSpeed), sin(SumDeltaTime * SpinSpeed)) * SpinRadius;
        float Distance = length(Pos - CenterPos);
        
        if (Distance <= LightRadius)
        {
            
            float2 Offset = Orbit * 0.01f;
        
            // ��
            float3 LightColor = float3(0.0f, 1.0f, 0.7f);
            float3 Light = LightColor * (1 - (Distance / LightRadius)) * LightStrength;
            float3 Background = Color.xyz + Color.xyz * Light;
        
        
            Color = float4(Background, Color.a);
        }


    }
       
    return Color;
   
}

Output PointLight_VSINST(Input _Input)
{
    Output NewOutPut = (Output) 0;
    NewOutPut.Pos = mul(_Input.Pos, AllInstancingTransformData[_Input.Index].WorldViewProjection);
    NewOutPut.Tex = _Input.Tex;
    

    return NewOutPut;
}

