#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineTextureRenderer;
class Fan : public GameEngineActor
{
public:
	// constrcuter destructer
	Fan();
	~Fan();

	// delete Function
	Fan(const Fan& _Other) = delete;
	Fan(Fan&& _Other) noexcept = delete;
	Fan& operator=(const Fan& _Other) = delete;
	Fan& operator=(Fan&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

	void OnEvent() override;

private:
	GameEngineTextureRenderer* Renderer;
	class GameEngineCollision* Collision;
};

