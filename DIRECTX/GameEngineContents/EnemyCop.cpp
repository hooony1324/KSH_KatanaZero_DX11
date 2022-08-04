#include "PreCompile.h"
#include "EnemyCop.h"
#include <GameEngineCore/CoreMinimal.h>

EnemyCop::EnemyCop() 
{
}

EnemyCop::~EnemyCop() 
{
}

void EnemyCop::Start()
{
	Renderer_Character = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Character->SetTexture("spr_cop_aim_0.png");
	Renderer_Character->ScaleToTexture();
	Renderer_Character->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "cop_idle", 0.1f });
	Renderer_Character->ChangeFrameAnimation("idle");

	GetTransform().SetLocalScale({ 2, 2, 1 });
}

void EnemyCop::Update(float _DeltaTime)
{
}

void EnemyCop::End()
{
}
