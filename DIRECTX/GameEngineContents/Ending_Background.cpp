#include "PreCompile.h"
#include "Ending_Background.h"
#include <GameEngineCore/GameEngineTextureRenderer.h>

Ending_Background::Ending_Background() 
{
}

Ending_Background::~Ending_Background() 
{
}

void Ending_Background::Start()
{
	GameEngineTextureRenderer* BackGround = CreateComponent<GameEngineTextureRenderer>();
	BackGround->GetTransform().SetLocalScale({ 1280, 720, 1 });
	BackGround->SetTexture("Rooftop.png");
	BackGround->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "Ending_Rooftop", 0.1f });
	BackGround->ChangeFrameAnimation("idle");
	BackGround->GetTransform().SetWorldPosition({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_0) });
}

void Ending_Background::Update(float _DeltaTime)
{
}

void Ending_Background::End()
{
}
