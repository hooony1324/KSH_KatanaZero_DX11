#pragma once
#include <GameEngineCore/GameEngineActor.h>

// CreateActor후, On/Off 하면 됨
class TentacleBoss : public GameEngineActor
{
public:
	// 소환
	void Spawn();
	bool IsHurt()
	{
		return Hurt;
	}

public:
	// constrcuter destructer
	TentacleBoss();
	~TentacleBoss();

	// delete Function
	TentacleBoss(const TentacleBoss& _Other) = delete;
	TentacleBoss(TentacleBoss&& _Other) noexcept = delete;
	TentacleBoss& operator=(const TentacleBoss& _Other) = delete;
	TentacleBoss& operator=(TentacleBoss&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

	void OnEvent() override;
	void OffEvent() override;

private:
	GameEngineStateManager StateManager;

	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void SpawnStart(const StateInfo& _Info);
	void SpawnUpdate(float _DeltaTime, const StateInfo& _Info);

	void HurtStart(const StateInfo& _Info);
	void HurtUpdate(float _DeltaTime, const StateInfo& _Info);

	void PortalInStart(const StateInfo& _Info);
	void PortalInUpdate(float _DeltaTime, const StateInfo& _Info);


private:
	class GameEngineTextureRenderer* Renderer;
	class GameEngineCollision* Collision;

	float4 StartPos;
	float4 DestPos;

	bool Hurt;

};

