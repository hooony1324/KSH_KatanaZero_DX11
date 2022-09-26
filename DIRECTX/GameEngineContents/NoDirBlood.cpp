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
	Renderer->CreateFrameAnimationFolder("player_dustcloud", FrameAnimation_DESC{ "player_dustcloud", 0.08f });
	Renderer->SetScaleModeImage();
	Renderer->SetScaleRatio(2.0f);
	Renderer->ChangeFrameAnimation("player_dustcloud");

	Renderer->AnimationBindEnd("player_dustcloud", [=](const FrameAnimation_DESC& _Info)
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
