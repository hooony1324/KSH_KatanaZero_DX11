#include "PreCompile.h"
#include "BossProjectile.h"
#include <GameEngineCore/CoreMinimal.h>

BossProjectile::BossProjectile() 
	: Reflected(false)
	, Renderer(nullptr)
	, Collision(nullptr)
{
}

BossProjectile::~BossProjectile() 
{
}

void BossProjectile::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetSamplingModePoint();
	Renderer->GetTransform().SetLocalScale({ 54, 48, 1 });
	Renderer->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "projectile_idle", 0.12f });
	Renderer->CreateFrameAnimationFolder("die", FrameAnimation_DESC{ "projectile_die", 0.12f, false });
	Renderer->ChangeFrameAnimation("idle");
	Renderer->Off();

	Collision = CreateComponent<GameEngineCollision>();
	Collision->GetTransform().SetLocalScale({ 54, 48, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision->ChangeOrder(static_cast<int>(COLLISIONGROUP::ENEMY_ATTACK));
	Collision->Off();


	StateManager.CreateStateMember("Idle"
		, std::bind(&BossProjectile::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossProjectile::IdleStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Shoot"
		, std::bind(&BossProjectile::ShootUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossProjectile::ShootStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Die"
		, std::bind(&BossProjectile::DieUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossProjectile::DieStart, this, std::placeholders::_1));
	StateManager.ChangeState("Idle");


	Renderer->AnimationBindEnd("die",
		[=](const FrameAnimation_DESC& _Info)
		{
			Death();
		});

	GetTransform().SetLocalScale({ 1.5f, 1.5f, 1 });
}

void BossProjectile::Update(float _DeltaTime)
{
	StateManager.Update(_DeltaTime);
}

void BossProjectile::End()
{
}

void BossProjectile::IdleStart(const StateInfo& _Info)
{
	Renderer->On();
	Collision->On();
	Renderer->ChangeFrameAnimation("idle");
}

void BossProjectile::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 1.5f)
	{
		StateManager.ChangeState("Shoot");
	}
}

float ShootSpeed = 200.0f;
void BossProjectile::ShootStart(const StateInfo& _Info)
{

}

void BossProjectile::ShootUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == WallCheck())
	{
		StateManager.ChangeState("Die");
		return;
	}

	if (true == CollisionCheck())
	{
		Dir *= -1;
	}


	GetTransform().SetWorldMove(Dir * _DeltaTime * ShootSpeed);
}

void BossProjectile::DieStart(const StateInfo& _Info)
{
	Renderer->ChangeFrameAnimation("die");
}

void BossProjectile::DieUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

bool BossProjectile::WallCheck()
{
	float4 CurPos = GetTransform().GetWorldPosition();
	float4 Color = GlobalValueManager::ColMap->GetCurTexture()->GetPixelToFloat4(CurPos.ix(), CurPos.iy());

	if (Color.CompareInt3D(float4::GREEN) ||
		CurPos.x > 1280 || CurPos.x < 0 ||
		CurPos.y > 0 || CurPos.y < -880)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BossProjectile::CollisionCheck()
{
	if (true == Reflected)
	{
		return false;
	}

	if (true == Collision->IsCollision(CollisionType::CT_OBB2D, COLLISIONGROUP::PLAYER_ATTACK, CollisionType::CT_OBB2D))
	{
		Reflected = true;
		Collision->ChangeOrder(static_cast<int>(COLLISIONGROUP::PLAYER_BULLET));
		return true;
	}

	return false;

}
