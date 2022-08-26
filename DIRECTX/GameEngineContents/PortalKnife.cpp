#include "PreCompile.h"
#include "PortalKnife.h"
#include <GameEngineCore/CoreMinimal.h>

const float SpawnDistance = 117;
const float DrawDistance = 80;

PortalKnife::PortalKnife() 
{
}

PortalKnife::~PortalKnife() 
{
}

void PortalKnife::Start()
{

	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_psychboss_attack_knife_1.png");
	Renderer->ScaleToTexture();

	StateManager.CreateStateMember("Spawn"
		, std::bind(&PortalKnife::SpawnUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PortalKnife::SpawnStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Draw"
		, std::bind(&PortalKnife::DrawUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PortalKnife::DrawStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Shoot"
		, std::bind(&PortalKnife::ShootUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PortalKnife::ShootStart, this, std::placeholders::_1));

	MoveSpeed = 100;

	Death(6);
}

void PortalKnife::Update(float _DeltaTime)
{
	//StateManager.Update(_DeltaTime);
}

void PortalKnife::End()
{

}

float CurSpawnDis;
void PortalKnife::SpawnStart(const StateInfo& _Info)
{
	CurSpawnDis = 0;
	MoveSpeed = 100;
}

void PortalKnife::SpawnUpdate(float _DeltaTime, const StateInfo& _Info)
{
	CurSpawnDis += _DeltaTime * MoveSpeed;


}

float CurDrawDis;
void PortalKnife::DrawStart(const StateInfo& _Info)
{
	CurDrawDis = 0;
}

void PortalKnife::DrawUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

void PortalKnife::ShootStart(const StateInfo& _Info)
{

}

void PortalKnife::ShootUpdate(float _DeltaTime, const StateInfo& _Info)
{

}
