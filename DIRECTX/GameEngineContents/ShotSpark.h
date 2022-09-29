#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class ShotSpark : public GameEngineActor
{
public:
	// constrcuter destructer
	ShotSpark();
	~ShotSpark();

	// delete Function
	ShotSpark(const ShotSpark& _Other) = delete;
	ShotSpark(ShotSpark&& _Other) noexcept = delete;
	ShotSpark& operator=(const ShotSpark& _Other) = delete;
	ShotSpark& operator=(ShotSpark&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineTextureRenderer* RendererSpark;
	GameEngineTextureRenderer* RendererSmoke;
};

