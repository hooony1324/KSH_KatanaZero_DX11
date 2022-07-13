#pragma once
#include <GameEngineCore/GameEngineActor.h>

#include "InputHandler.h"
// Ό³Έν :
class GameEngineDefaultRenderer;
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
	GameEngineDefaultRenderer* bodyRenderer;
	GameEngineDefaultRenderer* childRenderer;
	GameEngineDefaultRenderer* childRenderer2;

	std::list<class Obstacle*> list_Obstacle;

	float4 Color;

	InputHandler Input;
};

