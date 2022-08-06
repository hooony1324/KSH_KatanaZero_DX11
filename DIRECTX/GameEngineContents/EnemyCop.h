#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "EnemyActor.h"

// Ό³Έν :
class EnemyCop : public EnemyActor
{
public:
	// constrcuter destructer
	EnemyCop();
	~EnemyCop();

	// delete Function
	EnemyCop(const EnemyCop& _Other) = delete;
	EnemyCop(EnemyCop&& _Other) noexcept = delete;
	EnemyCop& operator=(const EnemyCop& _Other) = delete;
	EnemyCop& operator=(EnemyCop&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

protected:

};

