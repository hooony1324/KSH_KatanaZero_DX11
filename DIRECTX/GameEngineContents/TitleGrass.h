#pragma once
#include "TitleUI.h"

// Ό³Έν :
class GameEngineTextureRenderer;
class TitleGrass : public TitleUI
{
public:
	// constrcuter destructer
	TitleGrass();
	~TitleGrass();

	// delete Function
	TitleGrass(const TitleGrass& _Other) = delete;
	TitleGrass(TitleGrass&& _Other) noexcept = delete;
	TitleGrass& operator=(const TitleGrass& _Other) = delete;
	TitleGrass& operator=(TitleGrass&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	// 640 x 120 : spr_title_grass_0
	GameEngineTextureRenderer* Renderer;
	float4 Color_Shadow;
};

