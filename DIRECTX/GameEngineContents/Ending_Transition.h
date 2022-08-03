#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class Ending_Transition : public GameEngineActor
{
public:
	// constrcuter destructer
	Ending_Transition();
	~Ending_Transition();

	// delete Function
	Ending_Transition(const Ending_Transition& _Other) = delete;
	Ending_Transition(Ending_Transition&& _Other) noexcept = delete;
	Ending_Transition& operator=(const Ending_Transition& _Other) = delete;
	Ending_Transition& operator=(Ending_Transition&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineTextureRenderer* Transition;
};

