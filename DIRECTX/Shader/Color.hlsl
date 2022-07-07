// #include "myshader.hlsli"

// SV_POSITION �ø�ƽ
// �׷���ī�忡�� �̳༮�� �̷� �η��ϱ� �ϰ� �ڵ����� ó���ϴ� �༮�� ������ �ϰ�.

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
    // ���̴��� ��쿡�� ��κ��� ��Ȳ���� ����ȯ�� �����ϴ�.
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
    
    // �� ���
    //float4 ScreenMiddle = { 640.0f, 360.0f, 0, 0 };
    //float4 InputVec = { _Input.Pos.x - ScreenMiddle.x, _Input.Pos.y - ScreenMiddle.y, 0, 0 };
    //float InputDis = length(InputVec);
    //if (InputDis >= 180.0f)
    //{
    //    clip(-1);
    //}
    
    // Ÿ�� ���
    //float4 InputVec = { _Input.Pos2.x - 0, _Input.Pos2.y - 0, 0, 0 };
    //float InputDis = length(InputVec);
    //if (InputDis >= 0.5f)
    //{
    //    clip(-1);
    //}
 
    // ������ ���
    //float Xpos = _Input.Pos2.x;
    //float Ypos = _Input.Pos2.y;
    //if (abs(Xpos - 0) / 0.5f + abs(Ypos - 0) / 0.5f > 1.0f)
    //{
    //    clip(-1);
    //}
    
    return _Input.Color /* * MultyplyColor + PlusColor */;
}
