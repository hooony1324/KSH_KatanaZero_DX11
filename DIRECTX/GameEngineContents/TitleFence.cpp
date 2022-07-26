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
	Renderer->GetTransform().SetWorldPosition({ 0, -100, static_cast<float>(TITLE_DEPTH::FENCE)});
	Renderer->SetTexture("spr_title_fence_0.png");

	LerpStart({ 0, -100 }, { 0, 300 }, 0.9f);
}

void TitleFence::Update(float _DeltaTime)
{
	LerpUpdate(_DeltaTime);
}

void TitleFence::End()
{
}
