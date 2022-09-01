#include "PreCompile.h"
#include "TentacleBoss.h"
#include <GameEngineCore/CoreMinimal.h>

const float SpawnUpDis = 120.0f;

void TentacleBoss::Spawn()
{
	On();
	StateManager.ChangeState("Spawn");
}

TentacleBoss::TentacleBoss()
	: Hurt(false)
{
}

TentacleBoss::~TentacleBoss() 
{
}

void TentacleBoss::OnEvent()
{
	Renderer->On();
	Collision->On();
	StateManager.ChangeState("Idle");
}

void TentacleBoss::OffEvent()
{
	Renderer->Off();
	Collision->Off();
	StateManager.ChangeState("Idle");
}

void TentacleBoss::Start()
{
	Renderer = CreateComponent<GameContentsCustomRenderer>();
	Renderer->GetTransform().SetLocalScale({ 60, 160, GetDepth(ACTOR_DEPTH::COLLISION) });
	Renderer->SetPivot(PIVOTMODE::TOP);
	Renderer->CreateFrameAnimationFolder("idle", CustomFrameAnimation_DESC{ "tentacleboss_idle", 0.12f });
	Renderer->CreateFrameAnimationFolder("hurt", CustomFrameAnimation_DESC{ "tentacleboss_hurt", 0.5f, false });
	Renderer->CreateFrameAnimationFolder("attack", CustomFrameAnimation_DESC{ "tentacleboss_attack", 0.12f, false });
	Renderer->CreateMaskAnimationFolder("portal", CustomFrameAnimation_DESC{ "portal_cutoutY", 0.065f, true });
	Renderer->ChangeMaskAnimation("portal");
	Renderer->Option.IsMask = 1;
	RendererScaleY = Renderer->GetTransform().GetLocalScale().y;
	Renderer->ChangeFrameAnimation("idle");
	Renderer->GetTransform().SetLocalPosition({ 10, 0, 0 });
	Renderer->Off();
	Renderer->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));

	Collision = CreateComponent<GameEngineCollision>();
	Collision->GetTransform().SetLocalScale({ 60, 160, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision->GetTransform().SetLocalPosition({ 0, -107, 0 });
	Collision->ChangeOrder(COLLISIONGROUP::ENEMY);
	Collision->SetDebugSetting(CollisionType::CT_AABB2D, { 1, 0, 0, 0.2f });
	Collision->Off();

	StateManager.CreateStateMember("Idle"
		, std::bind(&TentacleBoss::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2));

	StateManager.CreateStateMember("Spawn"
		, std::bind(&TentacleBoss::SpawnUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&TentacleBoss::SpawnStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Hurt"
		, std::bind(&TentacleBoss::HurtUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&TentacleBoss::HurtStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("PortalIn"
		, std::bind(&TentacleBoss::PortalInUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&TentacleBoss::PortalInStart, this, std::placeholders::_1));

	StateManager.ChangeState("Idle");
	Off();
}

void TentacleBoss::Update(float _DeltaTime)
{
	float GroupDeltaScale = GameEngineTime::GetInst()->GetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP));
	StateManager.Update(_DeltaTime * GroupDeltaScale);
}

void TentacleBoss::End()
{
}

void TentacleBoss::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// Off상태
}


void TentacleBoss::SpawnStart(const StateInfo& _Info)
{
	Renderer->ChangeFrameAnimation("idle");
	StartPos = GetTransform().GetWorldPosition();
	DestPos = StartPos + float4{0, SpawnUpDis, 0};
}

void TentacleBoss::SpawnUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float4 CurPos = GetTransform().GetWorldPosition();
	float4 MovePos = float4::LerpLimit(StartPos, DestPos, _Info.StateTime * 2.0f);

	// 마스크렌더러 위치조정
	if (Renderer->GetMaskData().MaskFrameData.y <= -1.0f)
	{
		Renderer->GetMaskData().MaskFrameData.y = -1.0f;
	}
	else
	{
		float Move = (MovePos - CurPos).Length();
		Renderer->GetMaskData().MaskFrameData.y -= Move / RendererScaleY;
	}
	GetTransform().SetWorldPosition(MovePos);

	// Hurt
	if (true == Collision->IsCollision(CollisionType::CT_OBB2D, COLLISIONGROUP::PLAYER_ATTACK, CollisionType::CT_OBB2D))
	{
		Hurt = true;
		StateManager.ChangeState("Hurt");
		return;
	}
	else
	{
		Hurt = false;
	}


	if (_Info.StateTime > 3.0f)
	{
		StateManager.ChangeState("PortalIn");
		return;
	}
}

void TentacleBoss::HurtStart(const StateInfo& _Info)
{
	Renderer->ChangeFrameAnimation("hurt");
	Renderer->GetPixelData().MulColor = float4::RED;
}

void TentacleBoss::HurtUpdate(float _DeltaTime, const StateInfo& _Info)
{


	if (_Info.StateTime > 1.1f)
	{
		StateManager.ChangeState("PortalIn");
	}
}

void TentacleBoss::PortalInStart(const StateInfo& _Info)
{
	float4 Temp = DestPos;
	DestPos = StartPos;
	StartPos = Temp;

	Renderer->ChangeFrameAnimation("idle");
	Renderer->GetPixelData().MulColor = float4::ONE;
}

void TentacleBoss::PortalInUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float4 CurPos = GetTransform().GetWorldPosition();
	float4 MovePos = float4::LerpLimit(StartPos, DestPos, _Info.StateTime * 2.0f);

	// 마스크렌더러 위치조정
	if (Renderer->GetMaskData().MaskFrameData.y >= 0.0f)
	{
		Renderer->GetMaskData().MaskFrameData.y = 0.0f;
	}
	else
	{
		float Move = (MovePos - CurPos).Length();
		Renderer->GetMaskData().MaskFrameData.y += Move / RendererScaleY;
	}

	GetTransform().SetWorldPosition(MovePos);
	
	if (_Info.StateTime > 0.6f)
	{
		StateManager.ChangeState("Idle");
		Off();
	}
}
