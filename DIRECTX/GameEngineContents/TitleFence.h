#pragma once
#include "TitleUI.h"

// Ό³Έν :
class GameEngineTextureRenderer;
class TitleFence : public TitleUI
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
	GameEngineTextureRenderer* Renderer;
};

