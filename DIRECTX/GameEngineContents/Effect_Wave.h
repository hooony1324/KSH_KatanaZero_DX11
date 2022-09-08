#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>
#include <GameEngineCore/GameEngineRenderSet.h>


// Ό³Έν :
class Effect_Wave : public GameEnginePostEffect
{
public:
	static void OnOffSwitch()
	{
		OnOffOption = !OnOffOption;
	}

	static void WaveOff()
	{
		OnOffOption = 0;
	}

	static void WaveOn()
	{
		OnOffOption = 1;
	}

public:
	// constrcuter destructer
	Effect_Wave();
	~Effect_Wave();

	// delete Function
	Effect_Wave(const Effect_Wave& _Other) = delete;
	Effect_Wave(Effect_Wave&& _Other) noexcept = delete;
	Effect_Wave& operator=(const Effect_Wave& _Other) = delete;
	Effect_Wave& operator=(Effect_Wave&& _Other) noexcept = delete;

	void EffectInit() override;
	void Effect(GameEngineRenderTarget* _Target) override;

protected:
	GameEngineRenderTarget* CopyTarget;
	GameEngineRenderSet EffectSet;

private:
	static bool OnOffOption;
	CustomRenderOption Option;
	float SumDeltaTime;
};

