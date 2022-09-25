#include "PreCompile.h"
#include "JumpCloud.h"
#include <GameEngineCore/CoreMinimal.h>

JumpCloud::JumpCloud() 
{
}

JumpCloud::~JumpCloud() 
{
}

void JumpCloud::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetScaleModeImage();
	Renderer->SetScaleRatio(2.0f);
	Renderer->SetPivot(PIVOTMODE::BOT);
	Renderer->CreateFrameAnimationFolder("player_jumpcloud", FrameAnimation_DESC{ "player_jumpcloud", 0.08f });
	Renderer->ChangeFrameAnimation("player_jumpcloud");

	Renderer->AnimationBindEnd("player_jumpcloud", [=](const FrameAnimation_DESC& _Info)
		{
			Death();
		});
}

void JumpCloud::Update(float _DeltaTime)
{
}

void JumpCloud::End()
{
}
