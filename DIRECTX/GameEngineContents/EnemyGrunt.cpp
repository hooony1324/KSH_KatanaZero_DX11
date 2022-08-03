#include "PreCompile.h"
#include "EnemyGrunt.h"

EnemyGrunt::EnemyGrunt() 
{
}

EnemyGrunt::~EnemyGrunt() 
{
}

void EnemyGrunt::Start()
{
	Renderer_Character = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Character->SetTexture("spr_grunt_idle_0.png");
	Renderer_Character->ScaleToTexture();
	Renderer_Character->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "grunt_idle", 0.1f });
	Renderer_Character->ChangeFrameAnimation("idle");

	GetTransform().SetLocalScale({ 2, 2, 1 });
}

void EnemyGrunt::Update(float _DeltaTime)
{
}

void EnemyGrunt::End()
{
}
