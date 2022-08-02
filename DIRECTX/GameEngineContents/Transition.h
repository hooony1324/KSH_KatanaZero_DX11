#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineUIRenderer;
class Transition : public GameEngineActor
{
public:
	void Activate();

public:
	// constrcuter destructer
	Transition();
	~Transition();

	// delete Function
	Transition(const Transition& _Other) = delete;
	Transition(Transition&& _Other) noexcept = delete;
	Transition& operator=(const Transition& _Other) = delete;
	Transition& operator=(Transition&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	enum class STATE
	{
		NONE,
		FADEIN,
		FADEOUT,
	};
	STATE State;

	void UpdateState(float _DeltaTime);
	void ChangeState(STATE _State);

	void FadeInStart();
	void FadeInUpdate(float _DeltaTime);
	void FadeOutStart();
	void FadeOutUpdate(float _DeltaTime);

private:
	std::vector<std::vector<GameEngineUIRenderer*>> TransitionParticles;
	int YNUM;
	int XNUM;
	int XIndex;
	float SumDeltaTime;
};

