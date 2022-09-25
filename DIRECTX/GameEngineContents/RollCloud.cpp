#include "PreCompile.h"
#include "RollCloud.h"
#include <GameEngineCore/CoreMinimal.h>

RollCloud::RollCloud() 
{
}

RollCloud::~RollCloud() 
{
}

void RollCloud::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->CreateFrameAnimationFolder("player_dustcloud", FrameAnimation_DESC{ "player_dustcloud", 0.08f });
	Renderer->SetScaleModeImage();
	Renderer->SetScaleRatio(2.0f);
	Renderer->ChangeFrameAnimation("player_dustcloud");

	Renderer->AnimationBindEnd("player_dustcloud", [=](const FrameAnimation_DESC& _Info)
		{
			Death();
		});
}

void RollCloud::Update(float _DeltaTime)
{
	float4 pos = GetTransform().GetWorldPosition();
	GetTransform().SetWorldMove(Dir * Speed * _DeltaTime);
}

void RollCloud::End()
{
}
