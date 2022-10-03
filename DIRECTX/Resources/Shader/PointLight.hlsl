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
#define SpinRadius 20.0
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
    float2 StartPos = LightScreenPos.xy;

    //// SV_Position : �»�� {0, 0} ���ϴ� {1280, 720} -> ��ũ��
    Color = Tex.Sample(Smp, TexPos);
    float Ambient = Color.r + Color.g + Color.b; // ��ü���� ���
    
    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            float2 CenterPos = float2(StartPos.x + x * 300.0f, StartPos.y - y * 300.0f);
            // ���� ��ο� �ȼ� ���� �ؽ����� �ʷϻ� ������ ��ȭ
            if (Ambient <= 0.5f && Ambient >= 0.05f)
            {
                // ��
                float SpinDir = fmod(x, 2) >= 1.0f ? 1.0f : -1.0f;
        
                CenterPos += float2(SpinDir * cos(SumDeltaTime * SpinSpeed), sin(SumDeltaTime * SpinSpeed)) * SpinRadius;
                float Distance = length(Pos - CenterPos);
        
                if (Distance <= LightRadius)
                {
                    
                     // ��
                    float3 LightColor;
                    if (fmod(x, 3) < 0.1f)
                    {
                        LightColor = float3(0.0f, 1.0f, 0.7f);
                    }
                    else if (fmod(x, 3) >= 1.0f && fmod(x, 3) < 2.0f)
                    {
                        LightColor = float3(0.0f, 0.8f, 1.0f);
                    }
                    else
                    {
                        LightColor = float3(0.6f, 0.0f, 0.8f);
                    }
                
                    float3 Light = LightColor * (1 - (Distance / LightRadius)) * LightStrength;
                    float3 Background = Color.xyz + Color.xyz * Light;
        
        
                    Color = float4(Background, Color.a);
                }
            }
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

