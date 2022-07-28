#include "PreCompile.h"
#include "TitleFence.h"
#include <GameEngineCore/CoreMinimal.h>

const float LERPIN_DIST = 400.0f;

TitleFence::TitleFence() 
{
}

TitleFence::~TitleFence() 
{
}

void TitleFence::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->GetTransform().SetLocalScale({ 1280, 1440, 1 });
	Renderer->SetTexture("spr_title_fence_0.png");

	float Depth = static_cast<float>(TITLE_DEPTH::FENCE);
	LerpStart({ 0, -100, Depth }, { 0, 300, Depth }, 0.9f);
}

void TitleFence::Update(float _DeltaTime)
{
	LerpUpdate(_DeltaTime);
}

void TitleFence::End()
{
}
