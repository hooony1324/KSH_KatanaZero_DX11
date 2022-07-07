// #include "myshader.hlsli"

// SV_POSITION 시맨틱
// 그래픽카드에게 이녀석은 이런 부류니까 니가 자동으로 처리하는 녀석이 있으면 하고.

// t1

// Create("TransformData");
#include "TransformHeader.hlsli"

struct Input
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
};

struct Output
{
    float4 Pos : SV_POSITION;
    float4 Pos2 : POSITION;
    float4 Color : COLOR;
};

// 1000
// 0100
// 2010
// 0301

// 1020
// 0103
// 0010
// 0001

Output Color_VS(Input _Input)
{
    // 쉐이더의 경우에는 대부분의 상황에서 형변환이 가능하다.
    // 0
    Output NewOutPut = (Output) 0;
    NewOutPut.Pos = _Input.Pos;
    NewOutPut.Pos.w = 1.0f;
    NewOutPut.Pos = mul(NewOutPut.Pos, WorldViewProjection);
    
    NewOutPut.Pos2 = _Input.Pos;
    // NewOutPut.Pos.w = 1.0f;
    NewOutPut.Color = _Input.Color;
    
    return NewOutPut;
}

cbuffer ResultColor : register(b2)
{
    float4 PlusColor;
    float4 MultyplyColor;
}

float4 Color_PS(Output _Input) : SV_Target0
{
    
    // 원 출력
    //float4 ScreenMiddle = { 640.0f, 360.0f, 0, 0 };
    //float4 InputVec = { _Input.Pos.x - ScreenMiddle.x, _Input.Pos.y - ScreenMiddle.y, 0, 0 };
    //float InputDis = length(InputVec);
    //if (InputDis >= 180.0f)
    //{
    //    clip(-1);
    //}
    
    // 타원 출력
    //float4 InputVec = { _Input.Pos2.x - 0, _Input.Pos2.y - 0, 0, 0 };
    //float InputDis = length(InputVec);
    //if (InputDis >= 0.5f)
    //{
    //    clip(-1);
    //}
 
    // 마름모 출력
    //float Xpos = _Input.Pos2.x;
    //float Ypos = _Input.Pos2.y;
    //if (abs(Xpos - 0) / 0.5f + abs(Ypos - 0) / 0.5f > 1.0f)
    //{
    //    clip(-1);
    //}
    
    return _Input.Color /* * MultyplyColor + PlusColor */;
}
