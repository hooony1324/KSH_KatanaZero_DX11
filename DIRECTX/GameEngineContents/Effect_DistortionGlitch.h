#pragma once
#include "CustomPostEffect.h"

// ���� :
class Effect_DistortionGlitch : public CustomPostEffect
{
public:
	static Effect_DistortionGlitch* Inst;
	static Effect_DistortionGlitch* GetInst()
	{
		if (nullptr == Inst)
		{
			MsgBoxAssert("�������� ���� ����Ʈ�� ����Ϸ��� �߽��ϴ�");
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
