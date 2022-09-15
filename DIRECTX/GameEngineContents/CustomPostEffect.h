#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>
#include <GameEngineCore/GameEngineRenderSet.h>
// Ό³Έν :
class CustomPostEffect : public GameEnginePostEffect
{
public:
	CustomRenderOption Option;
	void EffectSwitch()
	{
		OnOffOption = !OnOffOption;
	}

	void EffectOff()
	{
		OnOffOption = 0;
	}

	void EffectOn()
	{
		OnOffOption = 1;
	}

public:
	// constrcuter destructer
	CustomPostEffect();
	virtual ~CustomPostEffect();

	// delete Function
	CustomPostEffect(const CustomPostEffect& _Other) = delete;
	CustomPostEffect(CustomPostEffect&& _Other) noexcept = delete;
	CustomPostEffect& operator=(const CustomPostEffect& _Other) = delete;
	CustomPostEffect& operator=(CustomPostEffect&& _Other) noexcept = delete;

protected:
	GameEngineRenderTarget* CopyTarget;
	GameEngineRenderSet EffectSet;

protected:
	bool OnOffOption;
	float SumDeltaTime;
};

