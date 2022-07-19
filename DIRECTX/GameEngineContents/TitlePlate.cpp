#include "PreCompile.h"
#include "TitlePlate.h"
#include <GameEngineCore/CoreMinimal.h>

const float LERP_DIST = 200.0f;

TitlePlate::TitlePlate() 
{
}

TitlePlate::~TitlePlate() 
{
}

void TitlePlate::Start()
{
	Background = CreateComponent<GameEngineDefaultRenderer>();
	Background->GetTransform().SetLocalScale({ 1280, 1440, 0 });
	Background->SetPipeLine("color");
	Background->ShaderResources.SetConstantBufferLink("RESULTCOLOR", Color_Background);
	Color_Background.r = 0;
	Color_Background.g = 0;
	Color_Background.b = 0;
	Color_Background.w = 1.0f;

	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->GetTransform().SetLocalScale({ 1280, 1440, 0 });
	Renderer->GetTransform().SetWorldPosition({ 0, 100, 0 });
	Renderer->SetTexture("spr_title_background_0.png");

	// Lerp
	float4 Pos = Renderer->GetTransform().GetTransformData().WorldPosition;
	Lerp_Start = Pos;
	Lerp_Dest = Pos + float4{ 0, LERP_DIST, 0 };
}

void TitlePlate::Update(float _DeltaTime)
{
	float4 Pos = float4::LerpLimit(Lerp_Start, Lerp_Dest, SumDeltaTime);
	Renderer->GetTransform().SetWorldPosition(Pos);

	// 부드러운 Lerp
	CurPos = Renderer->GetTransform().GetTransformData().WorldPosition;
	float LerpSpeed = (Lerp_Dest - CurPos).Length() / 200;
	SumDeltaTime += _DeltaTime * LerpSpeed;
}

void TitlePlate::End()
{
}
