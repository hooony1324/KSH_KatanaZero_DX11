#include "PreCompile.h"
#include "ReflectFX.h"
#include <GameEngineCore/CoreMinimal.h>
#include <GameEngineBase/GameEngineSound.h>

ReflectFX::ReflectFX() 
{
}

ReflectFX::~ReflectFX() 
{
}

void ReflectFX::Start()
{
	SoundPlayer = GameEngineSound::SoundPlayControl("sound_slash_bullet.wav");
	SoundPlayer.Volume(0.08f);
	SoundPlayer.PlaySpeed(GameEngineTime::GetInst()->GetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP)));

	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->CreateFrameAnimationFolder("player_bulletreflect", FrameAnimation_DESC{ "player_bulletreflect", 0.1f, false });
	Renderer->SetScaleModeImage();
	Renderer->SetScaleRatio(1.0f);
	Renderer->ChangeFrameAnimation("player_bulletreflect");

	Renderer->AnimationBindEnd("player_bulletreflect", [=](const FrameAnimation_DESC& _Info)
		{
			Death();
		});

}

void ReflectFX::Update(float _DeltaTime)
{
}

void ReflectFX::End()
{
}
