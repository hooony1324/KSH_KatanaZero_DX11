#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class ReflectFX : public GameEngineActor
{
public:
	// constrcuter destructer
	ReflectFX();
	~ReflectFX();

	// delete Function
	ReflectFX(const ReflectFX& _Other) = delete;
	ReflectFX(ReflectFX&& _Other) noexcept = delete;
	ReflectFX& operator=(const ReflectFX& _Other) = delete;
	ReflectFX& operator=(ReflectFX&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineTextureRenderer* Renderer;
	GameEngineSoundPlayer SoundPlayer;
};

