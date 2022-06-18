#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineRenderer;
class Cube : public GameEngineActor
{
public:
	// constrcuter destructer
	Cube();
	~Cube();

	// delete Function
	Cube(const Cube& _Other) = delete;
	Cube(Cube&& _Other) noexcept = delete;
	Cube& operator=(const Cube& _Other) = delete;
	Cube& operator=(Cube&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	float Speed;
	GameEngineRenderer* bodyRenderer;
	GameEngineRenderer* childRenderer;

	std::list<class Obstacle*> list_Obstacle;

};

