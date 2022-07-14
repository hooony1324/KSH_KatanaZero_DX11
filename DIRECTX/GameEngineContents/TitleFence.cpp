#include "PreCompile.h"
#include "TitleFence.h"
#include <GameEngineCore/CoreMinimal.h>

TitleFence::TitleFence() 
{
}

TitleFence::~TitleFence() 
{
}

void TitleFence::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->GetTransform().SetLocalScale({ 640, 720, 0 });
	Renderer->SetTexture("spr_title_fence_0.png");
}

void TitleFence::Update(float _DeltaTime)
{
}

void TitleFence::End()
{
}
