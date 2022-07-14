#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineTextureRenderer;
class TitleGraphic : public GameEngineActor
{
public:
	// constrcuter destructer
	TitleGraphic();
	~TitleGraphic();

	// delete Function
	TitleGraphic(const TitleGraphic& _Other) = delete;
	TitleGraphic(TitleGraphic&& _Other) noexcept = delete;
	TitleGraphic& operator=(const TitleGraphic& _Other) = delete;
	TitleGraphic& operator=(TitleGraphic&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	// 639 x 360 : spr_titlegraphic_big_0
	GameEngineTextureRenderer* Renderer;
};

