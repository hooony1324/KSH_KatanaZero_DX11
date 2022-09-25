#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "Particle.h"
#include <GameEngineBase/GameEngineRandom.h>
// 설명 :
// 캐릭터 엑터이 붙어있음
class ParticleShooter : public GameEngineActor
{
public:
	template<typename ParticleType>
	void SetOneShot(int _Num, float4 _SpawnPos, float4 _Dir, float _Speed)
	{
		for (int i = 0; i < _Num; i++)
		{
			Particle* Ptr = dynamic_cast<Particle*>(GetLevel()->CreateActor<ParticleType>());
			if (nullptr == Ptr)
			{
				MsgBoxAssert("파티클이 아닌 오브젝트를 생성시도하였습니다");
			}
			float RandomDegree = GameEngineRandom::MainRandom.RandomFloat(-30.0f, 30.0f);

			
			Ptr->GetTransform().SetWorldPosition(_SpawnPos);
			Ptr->Dir = _Dir.RotationToDegreeZ(RandomDegree);
			Ptr->Speed = _Speed;
		}
	}
	
	// SetRandomShot(구르기) 생성 개수, 생성방향, 방향랜덤 정도, 인터타임, 슛 스피드

	// SetSprinklerShot(Blood) 


	void Shooting();

public:
	// constrcuter destructer
	ParticleShooter();
	~ParticleShooter();

	// delete Function
	ParticleShooter(const ParticleShooter& _Other) = delete;
	ParticleShooter(ParticleShooter&& _Other) noexcept = delete;
	ParticleShooter& operator=(const ParticleShooter& _Other) = delete;
	ParticleShooter& operator=(ParticleShooter&& _Other) noexcept = delete;

	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void OneShotStart(const StateInfo& _Info);
	void OneShotUpdate(float _DeltaTime, const StateInfo& _Info);

	//void RandomShotStart(const StateInfo& _Info);
	//void RandomShotUpdate(float _DeltaTime, const StateInfo& _Info);

	void ShotEnd(const StateInfo& _Info);

private:
	GameEngineStateManager StateManager;
	float4 FirstParticleDir;
	bool Shootable;
};

