#pragma once
#include <GameEngineCore/GameEngineTextureRenderer.h>

class GameContentsEffectRenderer : public GameEngineTextureRenderer
{
public:
	void SetWaveEffect(); // 보스름 출렁이는 뒷배경
	void SetConcaveEffect(); // 보스룸 바닥 오목함 연출

public:
	// constrcuter destructer
	GameContentsEffectRenderer();
	~GameContentsEffectRenderer();

	// delete Function
	GameContentsEffectRenderer(const GameContentsEffectRenderer& _Other) = delete;
	GameContentsEffectRenderer(GameContentsEffectRenderer&& _Other) noexcept = delete;
	GameContentsEffectRenderer& operator=(const GameContentsEffectRenderer& _Other) = delete;
	GameContentsEffectRenderer& operator=(GameContentsEffectRenderer&& _Other) noexcept = delete;

	CustomRenderOption& GetCustomOption()
	{
		return CustomOption;
	}

	void SetPivot(PIVOTMODE _Mode);

private:

	CustomRenderOption CustomOption;
	AtlasData AtlasDataInst;
};

