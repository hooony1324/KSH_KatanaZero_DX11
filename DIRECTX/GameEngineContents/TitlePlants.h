#pragma once
#include "TitleUI.h"

// Ό³Έν :
class GameEngineTextureRenderer;
class TitlePlants : public TitleUI
{
public:
	// constrcuter destructer
	TitlePlants();
	~TitlePlants();

	// delete Function
	TitlePlants(const TitlePlants& _Other) = delete;
	TitlePlants(TitlePlants&& _Other) noexcept = delete;
	TitlePlants& operator=(const TitlePlants& _Other) = delete;
	TitlePlants& operator=(TitlePlants&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	// 640 x 360 : spr_title_plants_0
	GameEngineTextureRenderer* Renderer;

};

