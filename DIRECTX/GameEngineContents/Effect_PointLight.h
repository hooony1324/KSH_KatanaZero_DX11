#pragma once
#include "CustomPostEffect.h"

struct PointLightData
{
	float4 LightWorldPos;
	float4 LightScreenPos;
};

// ���� :
class Effect_PointLight : public CustomPostEffect
{
public:
	static Effect_PointLight* Inst;
	static Effect_PointLight* GetInst()
	{
		if (nullptr == Inst)
		{
			MsgBoxAssert("�������� ���� ����Ʈ�� ����Ϸ��� �߽��ϴ�");
		}

		return Inst;
	}

	TransformData TData; // ī�޶� MVP��� �ֱ� �뵵
	GameEngineCamera* MainCam;

	PointLightData LightData;

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

};

