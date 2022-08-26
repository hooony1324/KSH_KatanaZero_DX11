#include "PreCompile.h"
#include "PortalKnife.h"
#include <GameEngineCore/CoreMinimal.h>

const float SpawnOutDis = 150;
const float SpawnSpeed = 8.0f;
const float DrawInDis = 100;
const float DrawInSpeed = 6.0f;

PortalKnife::PortalKnife() 
{
}

PortalKnife::~PortalKnife() 
{
}

void PortalKnife::OnEvent()
{
	
}

void PortalKnife::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_psychboss_attack_knife_1.png");
	Renderer->SetPivot(PIVOTMODE::RIGHT);
	Renderer->ScaleToTexture();
	Renderer->Off();

	StateManager.CreateStateMember("Idle"
		, std::bind(&PortalKnife::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2));

	StateManager.CreateStateMember("Spawn"
		, std::bind(&PortalKnife::SpawnUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PortalKnife::SpawnStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Draw"
		, std::bind(&PortalKnife::DrawUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PortalKnife::DrawStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Shoot"
		, std::bind(&PortalKnife::ShootUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PortalKnife::ShootStart, this, std::placeholders::_1));

	MoveSpeed = 1000;
	StateManager.ChangeState("Idle");
}

void PortalKnife::Update(float _DeltaTime)
{
	StateManager.Update(_DeltaTime);
}

void PortalKnife::End()
{

}

void PortalKnife::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime >= WakeTime)
	{
		StateManager.ChangeState("Spawn");
	}
}

void PortalKnife::SpawnStart(const StateInfo& _Info)
{
	Renderer->On();
	StartPos = GetTransform().GetWorldPosition();
	//if (Dir.x < 0.02f && Dir.x > 0)
	//{
	//	Dir.x = 0.02f;
	//}
	DestPos = StartPos + Dir * SpawnOutDis;

}

void PortalKnife::SpawnUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float4 CurPos = float4::Lerp(GetTransform().GetWorldPosition(), DestPos, _DeltaTime * 4.2f);
	
	if (abs(CurPos.x - DestPos.x) > 0.5f)
	{
	}
	GetTransform().SetWorldPosition(CurPos);

	if (_Info.StateTime > 2.0f)
	{
		StateManager.ChangeState("Draw");
	}
}

void PortalKnife::DrawStart(const StateInfo& _Info)
{
	StartPos = GetTransform().GetWorldPosition();
	DestPos = StartPos + (-Dir * DrawInDis);
}

void PortalKnife::DrawUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float4 CurPos = float4::LerpLimit(StartPos, DestPos, _Info.StateTime * DrawInSpeed);
	GetTransform().SetWorldPosition(CurPos);

	if (_Info.StateTime > 0.5f)
	{
		StateManager.ChangeState("Shoot");
	}
}

void PortalKnife::ShootStart(const StateInfo& _Info)
{

}

void PortalKnife::ShootUpdate(float _DeltaTime, const StateInfo& _Info)
{
	GetTransform().SetWorldMove({ Dir * MoveSpeed * _DeltaTime });


	if (_Info.StateTime > 3.0f)
	{
		Death();
	}
}
