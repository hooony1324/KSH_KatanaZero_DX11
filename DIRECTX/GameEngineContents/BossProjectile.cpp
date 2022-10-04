#include "PreCompile.h"
#include "BossProjectile.h"
#include <GameEngineCore/CoreMinimal.h>

#include "PortalTurret.h"
#include "PlayLevel.h"

float ShootSpeed = 850.0f;

BossProjectile::BossProjectile() 
	: CollisionResult(false)
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
	Renderer->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP_PARTICLE));

	Collision = CreateComponent<GameEngineCollision>();
	Collision->GetTransform().SetLocalScale({ 54, 48, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision->ChangeOrder(static_cast<int>(COLLISIONGROUP::ENEMY_ATTACK));
	Collision->SetDebugSetting(CollisionType::CT_POINT2D, float4{1, 1, 1, 0.2f});
	Collision->SetCollisionMode(CollisionMode::Ex);
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
	float GroupDeltaScale = GameEngineTime::GetInst()->GetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP));
	StateManager.Update(_DeltaTime * GroupDeltaScale);
}

void BossProjectile::End()
{
}

void BossProjectile::IdleStart(const StateInfo& _Info)
{
	Renderer->On();
	Renderer->ChangeFrameAnimation("idle");
	Collision->On();
}

void BossProjectile::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == CollisionCheck())
	{
		int a = 0;
	}

	if (true == Turret->IsDeath())
	{
		StateManager.ChangeState("Die");
		return;
	}

	if (_Info.StateTime > 1.5f)
	{
		StateManager.ChangeState("Shoot");
		return;
	}
}


void BossProjectile::ShootStart(const StateInfo& _Info)
{
	

	// ¹æÇâ
	float4 CurPos = GetTransform().GetWorldPosition();
	CurPos.z = 0;
	float4 PlayerPos = GlobalValueManager::PlayerPos;
	PlayerPos.z = 0;
	Dir = (PlayerPos - CurPos).NormalizeReturn();

	SoundPlayer = GameEngineSound::SoundPlayControl("sound_boss_akiraorb_shot_01.wav");
	SoundPlayer.Volume(0.1f);
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

	if (Color.g > 0.5f)
	{
		int a = 0;
	}

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
	/*if (true == Reflected)
	{
		return false;
	}

	if (true == Collision->IsCollisionEnterBase(CollisionType::CT_OBB2D, static_cast<int>(COLLISIONGROUP::PLAYER_ATTACK), CollisionType::CT_OBB2D))
	{
		PlayLevel::PlayLevelInst->ShakeRoom(true);
		SoundPlayer = GameEngineSound::SoundPlayControl("sound_slash_bullet.wav");
		SoundPlayer.Volume(0.1f);
		Reflected = true;
		Collision->ChangeOrder(static_cast<int>(COLLISIONGROUP::PLAYER_BULLET));
		return true;
	}*/
	if (true == CollisionResult)
	{
		return false;
	}

	Collision->IsCollisionEnterBase(CollisionType::CT_OBB2D, static_cast<int>(COLLISIONGROUP::PLAYER_ATTACK), CollisionType::CT_OBB2D,
		[=](GameEngineCollision* _This, GameEngineCollision* _Other) 
		{ 
			CollisionResult = true;
			return CollisionReturn::Break;
		}, [=](GameEngineCollision* _This, GameEngineCollision* _Other){return CollisionReturn::Break; }
		, [=](GameEngineCollision* _This, GameEngineCollision* _Other) {return CollisionReturn::Break; });

	if (true == CollisionResult)
	{
		PlayLevel::PlayLevelInst->ShakeRoom(true);

		if (0 == StateManager.GetCurStateStateName().compare("Shoot"))
		{
			SoundPlayer = GameEngineSound::SoundPlayControl("sound_slash_bullet.wav");
			SoundPlayer.Volume(0.1f);
		}
		Collision->ChangeOrder(static_cast<int>(COLLISIONGROUP::PLAYER_BULLET));
		return CollisionResult;
	}

	return CollisionResult;

}
