#include "PreCompile.h"
#include "LandCloud.h"
#include <GameEngineCore/CoreMinimal.h>

LandCloud::LandCloud() 
{
}

LandCloud::~LandCloud() 
{
}

void LandCloud::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetPivot(PIVOTMODE::BOT);
	Renderer->CreateFrameAnimationFolder("player_landcloud", FrameAnimation_DESC{ "player_landcloud", 0.08f });
	Renderer->SetScaleModeImage();
	Renderer->SetScaleRatio(2.0f);
	Renderer->ChangeFrameAnimation("player_landcloud");

	Renderer->AnimationBindEnd("player_landcloud", [=](const FrameAnimation_DESC& _Info)
		{
			Death();
		});
}

void LandCloud::Update(float _DeltaTime)
{
}

void LandCloud::End()
{
}
