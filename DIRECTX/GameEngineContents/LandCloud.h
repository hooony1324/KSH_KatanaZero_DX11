#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class LandCloud : public GameEngineActor
{
public:
	// constrcuter destructer
	LandCloud();
	~LandCloud();

	// delete Function
	LandCloud(const LandCloud& _Other) = delete;
	LandCloud(LandCloud&& _Other) noexcept = delete;
	LandCloud& operator=(const LandCloud& _Other) = delete;
	LandCloud& operator=(LandCloud&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineTextureRenderer* Renderer;
};

