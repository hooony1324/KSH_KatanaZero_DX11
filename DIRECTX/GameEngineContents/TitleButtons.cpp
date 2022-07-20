#include "PreCompile.h"
#include "TitleButtons.h"

const float LERP_DIST = 800.0f;

const float SELECTOR_DIST = 227 / 5.0f;

TitleButtons::TitleButtons() 
{
}

TitleButtons::~TitleButtons() 
{
}

void TitleButtons::Start()
{
	Renderer_Background = CreateComponent<GameEngineDefaultRenderer>();
	Renderer_Background->GetTransform().SetLocalScale({ 530, 227 });
	Renderer_Background->SetPipeLine("Color");
	Color_Background = float4{ 0, 0, 0, 0.5f };
	Renderer_Background->ShaderResources.SetConstantBufferLink("ResultColor", Color_Background);

	Renderer_Buttons = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Buttons->SetTexture("TitleButtons.png");
	Renderer_Buttons->ScaleToTexture();

	Renderer_Selector = CreateComponent<GameEngineDefaultRenderer>();
	Renderer_Selector->GetTransform().SetLocalScale({ 510, 44 });
	Renderer_Selector->SetPipeLine("Color");
	Color_Selector = float4{ 1, 1, 1, 0.2f };
	Renderer_Selector->ShaderResources.SetConstantBufferLink("ResultColor", Color_Selector);
	Renderer_Selector->GetTransform().SetLocalPosition({ 0, SELECTOR_DIST, 0 });

	GetTransform().SetWorldPosition({ 0, -1000, 0 });
	//GetTransform().SetWorldPosition({ 0, -200, 0 });
	float4 Pos = GetTransform().GetTransformData().WorldPosition;
	Lerp_Start = Pos;
	Lerp_Dest = Pos + float4{ 0, LERP_DIST, 0 };
}

void TitleButtons::Update(float _DeltaTime)
{
	float4 Pos = float4::LerpLimit(Lerp_Start, Lerp_Dest, SumDeltaTime);
	GetTransform().SetWorldPosition(Pos);

	// 부드러운 Lerp
	CurPos = GetTransform().GetTransformData().WorldPosition;
	float LerpSpeed = (Lerp_Dest - CurPos).Length() / 400;
	SumDeltaTime += _DeltaTime * LerpSpeed;
}

void TitleButtons::End()
{
}
