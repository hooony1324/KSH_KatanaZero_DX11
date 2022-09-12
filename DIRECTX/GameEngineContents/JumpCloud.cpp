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
	Renderer->GetTransform().SetLocalScale({ 48, 67, 1 });
	Renderer->SetPivot(PIVOTMODE::BOT);
	Renderer->CreateFrameAnimationFolder("player_jumpcloud", FrameAnimation_DESC{ "player_jumpcloud", 0.1f });
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
