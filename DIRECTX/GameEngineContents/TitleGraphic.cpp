#include "PreCompile.h"
#include "TitleGraphic.h"
#include <GameEngineCore/CoreMinimal.h>

const float LERP_DIST = 100.0f;

TitleGraphic::TitleGraphic() 
{
}

TitleGraphic::~TitleGraphic() 
{
}

void TitleGraphic::Start()
{
	Renderer_L = CreateComponent<GameEngineTextureRenderer>();
	Renderer_L->SetTexture("spr_titlegraphic_1.png");
	Renderer_L->ScaleToTexture();

	Renderer_R = CreateComponent<GameEngineTextureRenderer>();
	Renderer_R->SetTexture("spr_titlegraphic_2.png");
	Renderer_R->ScaleToTexture();

	Renderer_Logo = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Logo->SetTexture("spr_titlegraphic_big2_0.png");
	Renderer_Logo->ScaleToTexture();
	Renderer_Logo->GetTransform().SetLocalPosition({ 0, 50, 0 });

	GetTransform().SetLocalScale({ 1.3f, 1.3f, 0 });
	GetTransform().SetWorldPosition({ 0, -100, 0 });

	// Lerp
	float4 Pos = GetTransform().GetTransformData().WorldPosition;
	Lerp_Start = Pos;
	Lerp_Dest = Pos + float4{ 0, LERP_DIST, 0 };
}

void TitleGraphic::Update(float _DeltaTime)
{
	float4 Pos = float4::LerpLimit(Lerp_Start, Lerp_Dest, SumDeltaTime);
	GetTransform().SetWorldPosition(Pos);

	// 부드러운 Lerp
	CurPos = GetTransform().GetTransformData().WorldPosition;
	float LerpSpeed = (Lerp_Dest - CurPos).Length() / 100;
	SumDeltaTime += _DeltaTime * LerpSpeed;
}

void TitleGraphic::End()
{
}
