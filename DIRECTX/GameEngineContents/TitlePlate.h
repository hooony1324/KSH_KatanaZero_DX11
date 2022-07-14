#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineTextureRenderer;
class TitlePlate : public GameEngineActor
{
public:
	// constrcuter destructer
	TitlePlate();
	~TitlePlate();

	// delete Function
	TitlePlate(const TitlePlate& _Other) = delete;
	TitlePlate(TitlePlate&& _Other) noexcept = delete;
	TitlePlate& operator=(const TitlePlate& _Other) = delete;
	TitlePlate& operator=(TitlePlate&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	// 640 x 720 : spr_title_background_0
	GameEngineTextureRenderer* Renderer;
};

