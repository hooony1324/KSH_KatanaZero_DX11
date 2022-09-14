#pragma once
#include "CustomPostEffect.h"

// 설명 :
class Effect_DistortionGlitch : public CustomPostEffect
{
public:
	static Effect_DistortionGlitch* Inst;
	static Effect_DistortionGlitch* GetInst()
	{
		if (nullptr == Inst)
		{
			MsgBoxAssert("생성되지 않은 이펙트를 사용하려고 했습니다");
		}

		return Inst;
	}

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

};
