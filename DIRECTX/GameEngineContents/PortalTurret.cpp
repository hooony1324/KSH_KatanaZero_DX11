#include "PreCompile.h"
#include "PortalTurret.h"
#include <GameEngineCore/CoreMinimal.h>

#include "BossProjectile.h"
#include "PlayLevel.h"

void PortalTurret::Spawn(float4 _SpawnPos, float _FloatDis, float _WakeTime)
{
	StartPos = _SpawnPos;
	FloatDis = _FloatDis;
	WakeTime = _WakeTime;
	On();
}

PortalTurret::PortalTurret()
	: Renderer(nullptr)
	, Collision(nullptr)
{
}

PortalTurret::~PortalTurret() 
{
}

void PortalTurret::OnEvent()
{
	StateManager.ChangeState("Idle");
}

void PortalTurret::OffEvent()
{
	Renderer->Off();
	Collision->Off();
}

void PortalTurret::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetSamplingModePoint();
	Renderer->SetScaleModeImage();
	//Renderer->GetTransform().SetLocalScale({ 66, 60, 1 });
	Renderer->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "turret_idle", 0.12f });
	Renderer->CreateFrameAnimationFolder("die", FrameAnimation_DESC{ "turret_die", 0.12f , false});
	Renderer->CreateFrameAnimationFolder("attack", FrameAnimation_DESC{ "turret_attack", 0.12f, false });
	Renderer->ChangeFrameAnimation("idle");
	Renderer->Off();
	Renderer->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));

	Collision = CreateComponent<GameEngineCollision>();
	//Collision->GetTransform().SetLocalScale({ 66, 60, GetDepth(ACTOR_DEPTH::COLLISION)});
	float4 Scale = Renderer->GetTransform().GetLocalScale();
	Collision->GetTransform().SetLocalScale({ Scale.x, Scale.y, GetDepth(ACTOR_DEPTH::COLLISION)});
	Collision->ChangeOrder(COLLISIONGROUP::ENEMY);
	Collision->SetDebugSetting(CollisionType::CT_AABB2D, { 1, 0, 0, 0.2f });
	Collision->Off();

	StateManager.CreateStateMember("Idle"
		, std::bind(&PortalTurret::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2));

	StateManager.CreateStateMember("Spawn"
		, std::bind(&PortalTurret::SpawnUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PortalTurret::SpawnStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Attack"
		, std::bind(&PortalTurret::AttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PortalTurret::AttackStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Die"
		, std::bind(&PortalTurret::DieUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PortalTurret::DieStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("PortalIn"
		, std::bind(&PortalTurret::PortalInUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PortalTurret::PortalInStart, this, std::placeholders::_1));

	StateManager.ChangeState("Idle");

	// Bind
	Renderer->AnimationBindEnd("die",
		[=](const FrameAnimation_DESC& _Info)
		{
			Death();
		});

	GetTransform().SetLocalScale({ 2, 2, 1 });
	Off();
}

void PortalTurret::Update(float _DeltaTime)
{
	float GroupDeltaScale = GameEngineTime::GetInst()->GetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP));
	StateManager.Update(_DeltaTime * GroupDeltaScale);
}

void PortalTurret::End()
{
}

void PortalTurret::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime >= WakeTime)
	{
		StateManager.ChangeState("Spawn");
	}
}

void PortalTurret::SpawnStart(const StateInfo& _Info)
{
	Renderer->On();
	Collision->On();
	Renderer->ChangeFrameAnimation("idle");
	GetTransform().SetWorldPosition(StartPos);
	DestPos = StartPos + float4{0, FloatDis, 0};
}

void PortalTurret::SpawnUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float4 CurPos = float4::Lerp(GetTransform().GetWorldPosition(), DestPos, _DeltaTime * 4.2f);
	GetTransform().SetWorldPosition(CurPos);

	if (true == Collision->IsCollision(CollisionType::CT_OBB2D, COLLISIONGROUP::PLAYER_ATTACK, CollisionType::CT_OBB2D))
	{
		PlayLevel::PlayLevelInst->ShakeRoom();
		StateManager.ChangeState("Die");
		return;
	}

	if (_Info.StateTime > 3.0f)
	{
		StateManager.ChangeState("Attack");
	}
}

void PortalTurret::AttackStart(const StateInfo& _Info)
{
	Renderer->ChangeFrameAnimation("attack");

	BossProjectile* Projectile = GetLevel()->CreateActor<BossProjectile>();

	Projectile->Spawn(this, GetTransform().GetWorldPosition());
}

void PortalTurret::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// ÃÑ¾Ë ¹ß»ç
	

	if (true == Collision->IsCollision(CollisionType::CT_OBB2D, COLLISIONGROUP::PLAYER_ATTACK, CollisionType::CT_OBB2D))
	{
		StateManager.ChangeState("Die");
		return;
	}

	if (_Info.StateTime > 2.0f)
	{
		StateManager.ChangeState("Die");
		//StateManager.ChangeState("PortalIn");
	}
}

void PortalTurret::DieStart(const StateInfo& _Info)
{
	Renderer->ChangeFrameAnimation("die");
	Collision->Off();
}

void PortalTurret::DieUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

void PortalTurret::PortalInStart(const StateInfo& _Info)
{
	float4 Temp = DestPos;
	DestPos = StartPos;
	StartPos = Temp;
}

void PortalTurret::PortalInUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float4 CurPos = float4::LerpLimit(StartPos, DestPos, _Info.StateTime * 2.0f);
	GetTransform().SetWorldPosition(CurPos);

	if (_Info.StateTime > 1.5f)
	{
		Death();
	}
}
