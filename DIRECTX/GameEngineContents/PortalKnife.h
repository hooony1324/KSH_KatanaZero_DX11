#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class PortalKnife : public GameEngineActor
{
public:
	void Spawn(float4 _Pos, float _Rot)
	{
		GetTransform().SetWorldPosition(_Pos);

		GetTransform().SetWorldRotation({ 0,0, _Rot });

		

		Dir = float4::DegreeToDirection2D(_Rot).NormalizeReturn();
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
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineStateManager StateManager;

	void SpawnStart(const StateInfo& _Info);
	void SpawnUpdate(float _DeltaTime, const StateInfo& _Info);

	void DrawStart(const StateInfo& _Info);
	void DrawUpdate(float _DeltaTime, const StateInfo& _Info);

	void ShootStart(const StateInfo& _Info);
	void ShootUpdate(float _DeltaTime, const StateInfo& _Info);

private:
	class GameEngineTextureRenderer* Renderer;
	float4 Dir;
	float MoveSpeed;
};

