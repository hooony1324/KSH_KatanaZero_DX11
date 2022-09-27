#include "PreCompile.h"
#include "NoDirBlood.h"
#include <GameEngineCore/CoreMinimal.h>

NoDirBlood::NoDirBlood() 
{
}

NoDirBlood::~NoDirBlood() 
{
}

void NoDirBlood::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->CreateFrameAnimationFolder("Idle", FrameAnimation_DESC{ "spr_blood", 0.08f });
	Renderer->SetScaleRatio(3.0f);
	Renderer->SetScaleModeImage();
	Renderer->SetSamplingModePoint();
	Renderer->ChangeFrameAnimation("Idle");

	Renderer->AnimationBindEnd("Idle", [=](const FrameAnimation_DESC& _Info)
		{
			Death();
		});
}

void NoDirBlood::Update(float _DeltaTime)
{
	GetTransform().SetWorldMove(Dir * Speed * _DeltaTime);
}

void NoDirBlood::End()
{
}
