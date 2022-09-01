#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineRenderTarget.h>
#include <GameEngineCore/GameEngineRenderSet.h>

// Ό³Έν :
class Effect_Distortion : public GameEnginePostEffect
{
public:
	// constrcuter destructer
	Effect_Distortion();
	~Effect_Distortion();

	// delete Function
	Effect_Distortion(const Effect_Distortion& _Other) = delete;
	Effect_Distortion(Effect_Distortion&& _Other) noexcept = delete;
	Effect_Distortion& operator=(const Effect_Distortion& _Other) = delete;
	Effect_Distortion& operator=(Effect_Distortion&& _Other) noexcept = delete;

	void EffectInit() override;
	void Effect(GameEngineRenderTarget* _Target) override;

protected:
	GameEngineRenderTarget* CopyTarget;
	GameEngineRenderSet EffectSet;


};

