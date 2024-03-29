#pragma once
#include "CustomPostEffect.h"

struct PointLightData
{
	float4 LightWorldPos;
	float4 LightScreenPos;
	float LightRadius;
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

	TransformData TData; // 카메라 MVP행렬 넣기 용도
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

	// Rim
	GameEngineRenderTarget* RimTarget;
	GameEngineRenderSet RimEffectSet;
	GameEngineDefaultRenderer* RenderertoChromakey;
private:
	float4x4 ViewMatrix;
	float4x4 ProjectionMatrix;
	float4x4 ViewPortMatrix;
};

