#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class BossProjectile : public GameEngineActor
{
public:
	void Spawn(float4 _SpawnPos, float4 _PlayerPos)
	{
		float4 SpawnPos = _SpawnPos;
		SpawnPos.z = 0;
		float4 PlayerPos = _PlayerPos;
		PlayerPos.z = 0;
		GetTransform().SetWorldPosition(_SpawnPos);

		Dir = (PlayerPos - SpawnPos).NormalizeReturn();

		StateManager.ChangeState("Idle");
	}

public:
	// constrcuter destructer
	BossProjectile();
	~BossProjectile();

	// delete Function
	BossProjectile(const BossProjectile& _Other) = delete;
	BossProjectile(BossProjectile&& _Other) noexcept = delete;
	BossProjectile& operator=(const BossProjectile& _Other) = delete;
	BossProjectile& operator=(BossProjectile&& _Other) noexcept = delete;

protected:
	void OnEvent() override {}
	void OffEvent() override {}
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineStateManager StateManager;

	void IdleStart(const StateInfo& _Info);
	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void ShootStart(const StateInfo& _Info);
	void ShootUpdate(float _DeltaTime, const StateInfo& _Info);

	void DieStart(const StateInfo& _Info);
	void DieUpdate(float _DeltaTime, const StateInfo& _Info);

	bool WallCheck();
	bool CollisionCheck();

private:
	class GameEngineTextureRenderer* Renderer;
	class GameEngineCollision* Collision;

	float4 Dir;

};

