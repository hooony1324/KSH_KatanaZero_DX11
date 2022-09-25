#include "PreCompile.h"
#include "ParticleShooter.h"
#include <GameEngineCore/CoreMinimal.h>



void ParticleShooter::Shooting()
{
	Shootable = true;
}

ParticleShooter::ParticleShooter()
	: Shootable(false)
{
}

ParticleShooter::~ParticleShooter() 
{
}

void ParticleShooter::Start()
{
	StateManager.CreateStateMember("Idle",
		std::bind(&ParticleShooter::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2));

	StateManager.CreateStateMember("OneShot",
		std::bind(&ParticleShooter::OneShotUpdate, this, std::placeholders::_1, std::placeholders::_2),
		std::bind(&ParticleShooter::OneShotStart, this, std::placeholders::_1),
		std::bind(&ParticleShooter::ShotEnd, this, std::placeholders::_1));

	StateManager.ChangeState("Idle");
}

void ParticleShooter::Update(float _DeltaTime)
{
	if (false == Shootable)
	{
		return;
	}

	StateManager.Update(_DeltaTime);
}

void ParticleShooter::End()
{
}

void ParticleShooter::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

void ParticleShooter::OneShotStart(const StateInfo& _Info)
{

}

void ParticleShooter::OneShotUpdate(float _DeltaTime, const StateInfo& _Info)
{
}



void ParticleShooter::ShotEnd(const StateInfo& _Info)
{
	Shootable = false;
}
