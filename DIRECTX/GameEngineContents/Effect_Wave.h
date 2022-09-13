#pragma once
#include "CustomPostEffect.h"

// 설명 :
class Effect_Wave : public CustomPostEffect
{
public:
	static Effect_Wave* Inst;
	static Effect_Wave* GetInst()
	{
		if (nullptr == Inst)
		{
			MsgBoxAssert("생성되지 않은 이펙트를 사용하려고 했습니다");
		}

		return Inst;
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

};

