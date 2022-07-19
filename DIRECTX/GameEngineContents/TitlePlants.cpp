#include "PreCompile.h"
#include "TitlePlants.h"
#include <GameEngineCore/CoreMinimal.h>

const float LERP_DIST = 1000.0f;

TitlePlants::TitlePlants() 
{
}

TitlePlants::~TitlePlants() 
{
}

void TitlePlants::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_title_grass_0.png");
	Renderer->GetTransform().SetLocalScale({ 1280, 720 });
	Renderer->CreateFrameAnimationFolder("title_plants", FrameAnimation_DESC{ "title_plants", 0.1f });
	Renderer->ChangeFrameAnimation("title_plants");

	GetTransform().SetWorldPosition({ 0, -1000, 0 });

	// Lerp
	float4 Pos = GetTransform().GetTransformData().WorldPosition;
	Lerp_Start = Pos;
	Lerp_Dest = Pos + float4{ 0, LERP_DIST, 0 };
}

void TitlePlants::Update(float _DeltaTime)
{
	float4 Pos = float4::LerpLimit(Lerp_Start, Lerp_Dest, SumDeltaTime);
	GetTransform().SetWorldPosition(Pos);

	// 부드러운 Lerp
	CurPos = GetTransform().GetTransformData().WorldPosition;
	float LerpSpeed = (Lerp_Dest - CurPos).Length() / 350;
	SumDeltaTime += _DeltaTime * LerpSpeed;
}

void TitlePlants::End()
{
}
