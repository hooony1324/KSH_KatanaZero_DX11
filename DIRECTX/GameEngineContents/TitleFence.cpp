#include "PreCompile.h"
#include "TitleFence.h"
#include <GameEngineCore/CoreMinimal.h>

const float LERP_DIST = 400.0f;

TitleFence::TitleFence() 
{
}

TitleFence::~TitleFence() 
{
}

void TitleFence::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->GetTransform().SetLocalScale({ 1280, 1440, 0 });
	Renderer->GetTransform().SetWorldPosition({ 0, -100, 0 });
	Renderer->SetTexture("spr_title_fence_0.png");

	// Lerp
	float4 Pos = Renderer->GetTransform().GetTransformData().WorldPosition;
	Lerp_Start = Pos;
	Lerp_Dest = Pos + float4{ 0, LERP_DIST, 0 };
}

void TitleFence::Update(float _DeltaTime)
{
	float4 Pos = float4::LerpLimit(Lerp_Start, Lerp_Dest, SumDeltaTime);
	Renderer->GetTransform().SetWorldPosition(Pos);

	// 부드러운 Lerp
	CurPos = Renderer->GetTransform().GetTransformData().WorldPosition;
	float LerpSpeed = (Lerp_Dest - CurPos).Length() / 150;
	SumDeltaTime += _DeltaTime * LerpSpeed;
}

void TitleFence::End()
{
}
