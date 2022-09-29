#include "PreCompile.h"
#include "ShotSpark.h"
#include <GameEngineCore/CoreMinimal.h>

ShotSpark::ShotSpark() 
{
}

ShotSpark::~ShotSpark() 
{
}

void ShotSpark::Start()
{
	RendererSpark = CreateComponent<GameEngineTextureRenderer>();
	RendererSpark->CreateFrameAnimationFolder("gunspark", FrameAnimation_DESC{ "enemy_gunspark", 0.06f, false });
	RendererSpark->SetScaleModeImage();
	RendererSpark->ChangeFrameAnimation("gunspark");
	RendererSpark->AnimationBindEnd("gunspark", [=](const FrameAnimation_DESC& _Info) { RendererSpark->Off(); });

	RendererSmoke = CreateComponent<GameEngineTextureRenderer>();
	RendererSmoke->CreateFrameAnimationFolder("gunsmoke", FrameAnimation_DESC{ "enemy_gunsmoke", 0.06f, false });
	RendererSmoke->SetScaleModeImage();
	RendererSmoke->ChangeFrameAnimation("gunsmoke");
	RendererSmoke->AnimationBindEnd("gunsmoke", [=](const FrameAnimation_DESC& _Info)
		{
			RendererSmoke->Off();
			Death();
		});
}

void ShotSpark::Update(float _DeltaTime)
{
}

void ShotSpark::End()
{
}
