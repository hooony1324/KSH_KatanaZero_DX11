#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineTextureRenderer;
class EnemyActor : public GameEngineActor
{
public:
	// constrcuter destructer
	EnemyActor();
	virtual ~EnemyActor();

	// delete Function
	EnemyActor(const EnemyActor& _Other) = delete;
	EnemyActor(EnemyActor&& _Other) noexcept = delete;
	EnemyActor& operator=(const EnemyActor& _Other) = delete;
	EnemyActor& operator=(EnemyActor&& _Other) noexcept = delete;

protected:

protected:
	GameEngineTextureRenderer* Renderer_Character;
	float4 Velocity;
	float4 MoveVec;
	float MoveSpeed;

	GameEngineStateManager StateManager;
private:
};

