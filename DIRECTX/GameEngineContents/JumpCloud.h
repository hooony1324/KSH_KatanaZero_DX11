#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class JumpCloud : public GameEngineActor
{
public:
	// constrcuter destructer
	JumpCloud();
	~JumpCloud();

	// delete Function
	JumpCloud(const JumpCloud& _Other) = delete;
	JumpCloud(JumpCloud&& _Other) noexcept = delete;
	JumpCloud& operator=(const JumpCloud& _Other) = delete;
	JumpCloud& operator=(JumpCloud&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	class GameEngineTextureRenderer* Renderer;
};

