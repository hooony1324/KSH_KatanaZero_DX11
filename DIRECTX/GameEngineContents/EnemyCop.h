#pragma once
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
	void AttackStart(const StateInfo& _Info) override;
	void AttackUpdate(float _DeltaTime, const StateInfo& _Info) override;

	// Aim -> Shot
	void AimStart(const StateInfo& _Info);
	void AimUpdate(float _DeltaTime, const StateInfo& _Info);
	void ShootStart(const StateInfo& _Info);
	void ShootUpdate(float _DeltaTime, const StateInfo& _Info);

	GameEngineStateManager AttackStateManager;
	float4 AimDir;
	GameEngineTextureRenderer* Renderer_GunArm;

	bool PointToPlayer();
};

