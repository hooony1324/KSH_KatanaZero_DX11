#include "PreCompile.h"
#include "EnemyBullet.h"


EnemyBullet::EnemyBullet()
{
	MoveSpeed = 100;
}

EnemyBullet::~EnemyBullet() 
{
}

void EnemyBullet::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_bullet_0.png");
	Renderer->ScaleToTexture();

	float4 Scale = Renderer->GetTransform().GetLocalScale();
	Collision = CreateComponent<GameEngineCollision>();
	Collision->GetTransform().SetLocalScale({ Scale.x, Scale.y, GetDepth(ACTOR_DEPTH::FX) });
	Collision->ChangeOrder(COLLISIONGROUP::ENEMY_ATTACK);
	Collision->SetDebugSetting(CollisionType::CT_OBB2D, { 1, 1, 1, 0.5f });

	GetTransform().SetLocalScale({ 2, 2, 1 });
}

void EnemyBullet::Update(float _DeltaTime)
{
	GetTransform().SetWorldMove(Dir * MoveSpeed * _DeltaTime);
	BoundaryCheckAndDestroy();
}

void EnemyBullet::End()
{
}
