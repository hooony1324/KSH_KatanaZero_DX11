#include "PreCompile.h"
#include "DustCloud.h"
#include <GameEngineCore/CoreMinimal.h>

DustCloud::DustCloud() 
{
}

DustCloud::~DustCloud() 
{
}

void DustCloud::Start()
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

void DustCloud::Update(float _DeltaTime)
{
	GetTransform().SetWorldMove(Dir * Speed * _DeltaTime);
}

void DustCloud::End()
{
}
