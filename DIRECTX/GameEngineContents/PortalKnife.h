#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class PortalKnife : public GameEngineActor
{
public:
	void Spawn(float4 _Pos, float _Rot, float _WakeTime = 0.0f)
	{
		float4 KnifePos = _Pos - float4{0, 0, 10};
		GetTransform().SetWorldPosition(KnifePos);
		GetTransform().SetWorldRotation({ 0,0, _Rot });
		
		WakeTime = _WakeTime;
		Dir = float4::DegreeToDirection2D(_Rot).NormalizeReturn();
		Renderer->Off();
		On();
	}

public:
	// constrcuter destructer
	PortalKnife();
	~PortalKnife();

	// delete Function
	PortalKnife(const PortalKnife& _Other) = delete;
	PortalKnife(PortalKnife&& _Other) noexcept = delete;
	PortalKnife& operator=(const PortalKnife& _Other) = delete;
	PortalKnife& operator=(PortalKnife&& _Other) noexcept = delete;

protected:
	void OnEvent() override;

	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineStateManager StateManager;

	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void SpawnStart(const StateInfo& _Info);
	void SpawnUpdate(float _DeltaTime, const StateInfo& _Info);

	void DrawStart(const StateInfo& _Info);
	void DrawUpdate(float _DeltaTime, const StateInfo& _Info);

	void ShootStart(const StateInfo& _Info);
	void ShootUpdate(float _DeltaTime, const StateInfo& _Info);

private:
	class GameEngineTextureRenderer* Renderer;

	float4 StartPos;
	float4 DestPos;
	float4 Dir;
	float MoveSpeed;
	float WakeTime;
};

