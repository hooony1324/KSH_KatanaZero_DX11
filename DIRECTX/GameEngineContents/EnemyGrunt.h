#pragma once
#include "EnemyActor.h"
#include <GameEngineBase/GameEngineSound.h>

// Ό³Έν :
class EnemyGrunt : public EnemyActor
{
public:
	// constrcuter destructer
	EnemyGrunt();
	~EnemyGrunt();

	// delete Function
	EnemyGrunt(const EnemyGrunt& _Other) = delete;
	EnemyGrunt(EnemyGrunt&& _Other) noexcept = delete;
	EnemyGrunt& operator=(const EnemyGrunt& _Other) = delete;
	EnemyGrunt& operator=(EnemyGrunt&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

protected:
	bool PlayerAttackCheck(GameEngineCollision* _This, GameEngineCollision* _Other) override;

private:



	void IdleStart(const StateInfo& _Info);
	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void DeathStart(const StateInfo& _Info);
	void DeathUpdate(float _DeltaTime, const StateInfo& _Info);


	//TODO:: Patrol, Chase, Attack 

	//GameEngineStateManager MoveStateManager;
private:

};

