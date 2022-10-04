#pragma once
#include <GameEngineCore/GameEngineTextureRenderer.h>

struct ConcaveData
{
	float4 ConcavePos; // { PlayerX, PlayerY, BossX, BossY } -> TEXCOORD����(0~1)
};

class GameContentsEffectRenderer : public GameEngineTextureRenderer
{
public:
	void SetWaveEffect(); // ������ �ⷷ�̴� �޹��
	void SetConcaveEffect(); // ������ �ٴ� ������ ����

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

	ConcaveData& GetConcaveData()
	{
		return ConcaveDataInst;
	}

	void SetPivot(PIVOTMODE _Mode);

private:

	CustomRenderOption CustomOption;
	AtlasData AtlasDataInst;
	ConcaveData ConcaveDataInst;
};

