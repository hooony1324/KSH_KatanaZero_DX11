#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class TestMonster : public GameEngineActor
{
public:
	// constrcuter destructer
	TestMonster();
	~TestMonster();

	// delete Function
	TestMonster(const TestMonster& _Other) = delete;
	TestMonster(TestMonster&& _Other) noexcept = delete;
	TestMonster& operator=(const TestMonster& _Other) = delete;
	TestMonster& operator=(TestMonster&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineCollision* Collision;

	CollisionReturn BlockedPlayer(GameEngineCollision* _This, GameEngineCollision* _Other);
};

