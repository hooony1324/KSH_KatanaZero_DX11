#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>
#include <GameEngineCore/GameEngineRenderSet.h>

// Ό³Έν :
class Effect_DistortionGlitch : public GameEnginePostEffect
{
public:
	// constrcuter destructer
	Effect_DistortionGlitch();
	~Effect_DistortionGlitch();

	// delete Function
	Effect_DistortionGlitch(const Effect_DistortionGlitch& _Other) = delete;
	Effect_DistortionGlitch(Effect_DistortionGlitch&& _Other) noexcept = delete;
	Effect_DistortionGlitch& operator=(const Effect_DistortionGlitch& _Other) = delete;
	Effect_DistortionGlitch& operator=(Effect_DistortionGlitch&& _Other) noexcept = delete;

	void EffectInit() override;
	void Effect(GameEngineRenderTarget* _Target) override;

protected:
	GameEngineRenderTarget* CopyTarget;
	GameEngineRenderSet EffectSet;

private:
	RenderOption Option;
	float SumDeltaTime;
};
