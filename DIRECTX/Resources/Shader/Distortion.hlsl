
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


// 그래픽카드에서 이뤄지는것.
Output Distortion_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    NewOutPut.Pos = _Input.Pos;
    NewOutPut.Tex = _Input.Tex;
    return NewOutPut;
}


Texture2D Tex : register(t0);
Texture2D EffectTex : register(t1);
SamplerState Smp : register(s0);
float4 Distortion_PS(Output _Input) : SV_Target0
{
    float4 Effect = (EffectTex.Sample(Smp, _Input.Tex.xy));
    
    float2 PixelUVSize = float2(1.0f / 1280.0f, 1.0f / 720.0f);
    float2 OriginalPos = _Input.Tex.xy;
    float2 NewPos;
    
    // 효과넣기
    if (Effect.a > 0.0f)
    {
        NewPos = OriginalPos + PixelUVSize * 50;
    }
    
    if (Effect.a <= 0.0f)
    {
        NewPos = OriginalPos + PixelUVSize * -50;
    }
    
    float4 Result = (Tex.Sample(Smp, NewPos));
    return Result;
}
