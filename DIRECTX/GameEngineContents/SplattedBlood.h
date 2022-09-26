#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class SplattedBlood : public GameEngineActor
{
public:
	void SpawnRandomBlood(float _Degree);

public:
	// constrcuter destructer
	SplattedBlood();
	~SplattedBlood();

	// delete Function
	SplattedBlood(const SplattedBlood& _Other) = delete;
	SplattedBlood(SplattedBlood&& _Other) noexcept = delete;
	SplattedBlood& operator=(const SplattedBlood& _Other) = delete;
	SplattedBlood& operator=(SplattedBlood&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineTextureRenderer* Renderer;
};

