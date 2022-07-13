#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineDefaultRenderer;
class TitleFence : public GameEngineActor
{
public:
	// constrcuter destructer
	TitleFence();
	~TitleFence();

	// delete Function
	TitleFence(const TitleFence& _Other) = delete;
	TitleFence(TitleFence&& _Other) noexcept = delete;
	TitleFence& operator=(const TitleFence& _Other) = delete;
	TitleFence& operator=(TitleFence&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	// 640 x 720 : spr_title_fence_0
	GameEngineDefaultRenderer* Renderer;
};

