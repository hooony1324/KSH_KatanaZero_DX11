#pragma once
#include "EnemyActor.h"
#include <GameEngineBase/GameEngineSound.h>

// ���� :
class GameEngineTextureRenderer;
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
	virtual void AttackStart(const StateInfo& _Info) override;
	virtual void AttackUpdate(float _DeltaTime, const StateInfo& _Info) override;
	GameEngineTextureRenderer* Renderer_Slash;
	GameEngineCollision* Collision_Slash;

};

