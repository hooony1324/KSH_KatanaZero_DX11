#include "PreCompile.h"
#include "TitleGrass.h"
#include <GameEngineCore/CoreMinimal.h>

const float LERP_DIST = 450.0f;

TitleGrass::TitleGrass() 
{
}

TitleGrass::~TitleGrass() 
{
}

void TitleGrass::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_title_grass_0.png");
	Renderer->ScaleToTexture();
	Renderer->GetTransform().SetWorldPosition({ 0, -720, 0 });

	GetTransform().SetWorldScale(float4{ 1.0f, 1.0f, 1.0f } * 1.5f);

	// Lerp
	float4 Pos = Renderer->GetTransform().GetTransformData().WorldPosition;
	Lerp_Start = Pos;
	Lerp_Dest = Pos + float4{ 0, LERP_DIST, 0 };
}

void TitleGrass::Update(float _DeltaTime)
{
	float4 Pos = float4::LerpLimit(Lerp_Start, Lerp_Dest, SumDeltaTime);
	Renderer->GetTransform().SetWorldPosition(Pos);

	// 부드러운 Lerp
	CurPos = Renderer->GetTransform().GetTransformData().WorldPosition;
	float LerpSpeed = (Lerp_Dest - CurPos).Length() / 200;
	SumDeltaTime += _DeltaTime * LerpSpeed;
}

void TitleGrass::End()
{
}
