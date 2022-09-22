#include "PreCompile.h"
#include "Effect_PointLight.h"
#include <GameEngineCore/CoreMinimal.h>


Effect_PointLight* Effect_PointLight::Inst = nullptr;

Effect_PointLight::Effect_PointLight() 
{
}

Effect_PointLight::~Effect_PointLight() 
{
	Inst = nullptr;

}

void Effect_PointLight::EffectInit()
{
	Inst = this;

	CopyTarget = new GameEngineRenderTarget();
	CopyTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	EffectSet.SetPipeLine("PointLight");
	OnOffOption = false;

	Option.DeltaTime = 0.0f;

	if (true == EffectSet.ShaderResources.IsConstantBuffer("CustomRenderOption"))
	{
		EffectSet.ShaderResources.SetConstantBufferLink("CustomRenderOption", &Option, sizeof(Option));
		Option.OnOff = static_cast<int>(OnOffOption);
	}

	// ºû À§Ä¡ ÁöÁ¤
	if (true == EffectSet.ShaderResources.IsConstantBuffer("LightData"))
	{
		EffectSet.ShaderResources.SetConstantBufferLink("LightData", &LightOption, sizeof(LightOption));
	}

	LightOption.Position = (320.0f, 560.0f);
	//float2 CirclePos = float2(320.0f, 560.0f);

}

void Effect_PointLight::Effect(GameEngineRenderTarget* _Target)
{
	if (false == OnOffOption)
	{
		SumDeltaTime = 0.0f;
		return;
	}

	// 
	CopyTarget->Copy(_Target);

	Option.OnOff = static_cast<int>(OnOffOption);
	float DeltaTime = GameEngineTime::GetDeltaTime();
	SumDeltaTime += DeltaTime;
	Option.DeltaTime = DeltaTime;
	Option.SumDeltaTime = SumDeltaTime;
	EffectSet.ShaderResources.SetTexture("Tex", CopyTarget->GetRenderTargetTexture(0));

	_Target->Clear();
	_Target->Setting();
	_Target->Effect(EffectSet);

}
