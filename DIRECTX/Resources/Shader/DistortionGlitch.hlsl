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


// 참고 : https://www.shadertoy.com/view/Md2GDw

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);
float4 Wave_PS(Output _Input) : SV_Target0
{
    // fragCoord => SV_Position
    // iResolution => 화면 크기
    // iTime => playbacktime(재생시간)
    float iTime = DeltaTime;
    
    float2 uv = (_Input.Pos.x / 1280.0f, _Input.Pos.y / 720.0f);
    float2 block = floor(_Input.Pos.xy / float2(16, 16));
    float2 uv_noise = block / float2(64, 64);
    uv_noise += floor(float2(iTime, iTime) * float2(1234.0f, 3543.0f)) / float2(64, 64);
    
    float block_thresh = pow(frac(iTime * 1236.0453f), 2.0f) * 0.2f;
    float line_thresh = pow(frac(iTime * 2236.0453f), 3.0f) * 0.7f;
    
    float2 uv_r = uv, uv_g = uv, uv_b = uv;
    
    // 블럭 Glitch
    if (Tex.Sample(Smp, uv_noise).r < block_thresh ||
        Tex.Sample(Smp, float2(uv_noise.y, 0.0f)).g < line_thresh)
    {
        float2 dist = (frac(uv_noise) - 0.5f) * 0.3f;
        uv_r += dist * 0.1f;
        uv_g += dist * 0.2f;
        uv_b += dist * 0.125f;
    }

    float4 Result = (0, 0, 0, 1.0f);
    Result.r = Tex.Sample(Smp, uv_r).r;
    Result.g = Tex.Sample(Smp, uv_g).g;
    Result.b = Tex.Sample(Smp, uv_b).b;
    
    	// loose luma for some blocks
    if (Tex.Sample(Smp, uv_noise).g < block_thresh)
        Result.rgb = Result.ggg;

	// discolor block lines
    if (Tex.Sample(Smp, float2(uv_noise.y, 0.0f)).b * 3.5f < line_thresh)
    {
        Result.rgb = float3(0.0f, dot(Result.rgb, float3(1.0f, 1.0f, 1.0f)), 0.0f);
    }

	// interleave lines in some blocks
    if (Tex.Sample(Smp, uv_noise).g * 1.5f < block_thresh ||
		Tex.Sample(Smp, float2(uv_noise.y, 0.0f)).g * 2.5f < line_thresh)
    {
        float Line = frac(_Input.Pos.y / 3.0f);
        float3 mask = float3(3.0f, 0.0f, 0.0f);
        if (Line > 0.333)
            mask = float3(0.0f, 3.0f, 0.0f);
        if (Line > 0.666)
            mask = float3(0.0f, 0.0f, 3.0f);
		
        Result.xyz *= mask;
    }
    
    
    
    return Result;
}

