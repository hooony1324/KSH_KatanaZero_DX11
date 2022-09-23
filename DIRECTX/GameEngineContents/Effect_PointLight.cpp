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

	if (true == EffectSet.ShaderResources.IsConstantBuffer("TRANSFORMDATA"))
	{
		EffectSet.ShaderResources.SetConstantBufferLink("TRANSFORMDATA", &TData, sizeof(TData));
	}

	if (true == EffectSet.ShaderResources.IsConstantBuffer("PointLightData"))
	{
		EffectSet.ShaderResources.SetConstantBufferLink("PointLightData", &LightData, sizeof(LightData));
	}

	float4 LightWorldPos = { 400, -600 , GetDepth(ACTOR_DEPTH::BACKGROUND_4), 1.0f };
	LightData.LightWorldPos = LightWorldPos;
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

	// PointLight움직임
	{
	}

	// PointLight위치 계산
	{
		float4 Pos = LightData.LightWorldPos;
		Pos *= MainCam->GetView();
		Pos *= MainCam->GetProjectionMatrix();

		float4x4 ViewPort;
		float4 Size = GameEngineWindow::GetInst()->GetScale();
		ViewPort.ViewPort(Size.x, Size.y, 0.0f, 0.0f, 0.0f, 1.0f);

		Pos *= ViewPort;
		LightData.LightScreenPos = Pos;
		TData.WorldViewProjectionMatrix = MainCam->GetTransform().GetWorldViewProjection();
	}




	EffectSet.ShaderResources.SetTexture("Tex", CopyTarget->GetRenderTargetTexture(0));

	_Target->Clear();
	_Target->Setting();
	_Target->Effect(EffectSet);

}
