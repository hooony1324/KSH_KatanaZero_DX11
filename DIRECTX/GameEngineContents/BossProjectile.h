#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class BossProjectile : public GameEngineActor
{
public:
	void Spawn(class PortalTurret* _Turret, float4 _SpawnPos)
	{
		GetTransform().SetWorldPosition(_SpawnPos);

		Turret = _Turret;
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
	class PortalTurret* Turret;

	float4 Dir;
	bool Reflected;
};

