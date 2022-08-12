#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class SlowMotion : public GameEngineActor
{
public:
	void SlowIn();
	void SlowOut();

public:
	// constrcuter destructer
	SlowMotion();
	~SlowMotion();

	// delete Function
	SlowMotion(const SlowMotion& _Other) = delete;
	SlowMotion(SlowMotion&& _Other) noexcept = delete;
	SlowMotion& operator=(const SlowMotion& _Other) = delete;
	SlowMotion& operator=(SlowMotion&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	class GameEngineTextureRenderer* Renderer;
	float4* Color;
	bool Slow; // true : In, false : out
	float TimeRatio;
	float TimeRatio_Fan;
};

