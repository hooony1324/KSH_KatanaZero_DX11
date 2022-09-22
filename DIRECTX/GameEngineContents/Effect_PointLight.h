#pragma once
#include "CustomPostEffect.h"

struct LightInfo
{
	float4 Position;
};

// 설명 :
class Effect_PointLight : public CustomPostEffect
{
public:
	static Effect_PointLight* Inst;
	static Effect_PointLight* GetInst()
	{
		if (nullptr == Inst)
		{
			MsgBoxAssert("생성되지 않은 이펙트를 사용하려고 했습니다");
		}

		return Inst;
	}

public:
	// constrcuter destructer
	Effect_PointLight();
	~Effect_PointLight();

	// delete Function
	Effect_PointLight(const Effect_PointLight& _Other) = delete;
	Effect_PointLight(Effect_PointLight&& _Other) noexcept = delete;
	Effect_PointLight& operator=(const Effect_PointLight& _Other) = delete;
	Effect_PointLight& operator=(Effect_PointLight&& _Other) noexcept = delete;

protected:
	void EffectInit() override;
	void Effect(GameEngineRenderTarget* _Target) override;

	LightInfo LightOption;
};

