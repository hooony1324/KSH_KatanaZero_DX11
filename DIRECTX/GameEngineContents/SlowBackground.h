#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class SlowBackground : public GameEngineActor
{
public:
	void FadeIn();
	void FadeOut();

public:
	// constrcuter destructer
	SlowBackground();
	~SlowBackground();

	// delete Function
	SlowBackground(const SlowBackground& _Other) = delete;
	SlowBackground(SlowBackground&& _Other) noexcept = delete;
	SlowBackground& operator=(const SlowBackground& _Other) = delete;
	SlowBackground& operator=(SlowBackground&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	class GameEngineTextureRenderer* Renderer;
	ColorData Color;
	bool Fade; // true : In, false : out
};

