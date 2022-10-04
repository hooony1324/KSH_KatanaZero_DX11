#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class BossFloor : public GameEngineActor
{
public:
	// constrcuter destructer
	BossFloor();
	~BossFloor();

	// delete Function
	BossFloor(const BossFloor& _Other) = delete;
	BossFloor(BossFloor&& _Other) noexcept = delete;
	BossFloor& operator=(const BossFloor& _Other) = delete;
	BossFloor& operator=(BossFloor&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameContentsEffectRenderer* Renderer;
	GameEngineCollision* Collision;

	float4 FloorScale;
	float4 FloorLeftTop;
};

