#pragma once
#include "CustomPostEffect.h"

// 설명 :
class Effect_Distortion : public CustomPostEffect
{
public:
	static Effect_Distortion* Inst;
	static Effect_Distortion* GetInst()
	{
		if (nullptr == Inst)
		{
			MsgBoxAssert("생성되지 않은 이펙트를 사용하려고 했습니다");
		}

		return Inst;
	}

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

private:
	GameEngineSoundPlayer EffectSoundPlayer;
};

