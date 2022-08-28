#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class PortalTurret : public GameEngineActor
{
public:
	void Spawn(float4 _SpawnPos, float _FloatDis, float _WakeTime = 0.0f);

public:
	// constrcuter destructer
	PortalTurret();
	~PortalTurret();

	// delete Function
	PortalTurret(const PortalTurret& _Other) = delete;
	PortalTurret(PortalTurret&& _Other) noexcept = delete;
	PortalTurret& operator=(const PortalTurret& _Other) = delete;
	PortalTurret& operator=(PortalTurret&& _Other) noexcept = delete;

protected:
	void OnEvent() override;
	void OffEvent() override;
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineStateManager StateManager;

	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void SpawnStart(const StateInfo& _Info);
	void SpawnUpdate(float _DeltaTime, const StateInfo& _Info);

	void AttackStart(const StateInfo& _Info);
	void AttackUpdate(float _DeltaTime, const StateInfo& _Info);

	void DieStart(const StateInfo& _Info);
	void DieUpdate(float _DeltaTime, const StateInfo& _Info);

	void PortalInStart(const StateInfo& _Info);
	void PortalInUpdate(float _DeltaTime, const StateInfo& _Info);

private:
	class GameEngineTextureRenderer* Renderer;
	class GameEngineCollision* Collision;

	float4 StartPos;
	float4 DestPos;
	float FloatDis;
	float WakeTime;
};

