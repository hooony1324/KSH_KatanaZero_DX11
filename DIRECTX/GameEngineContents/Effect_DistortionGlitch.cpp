#include "PreCompile.h"
#include "Effect_DistortionGlitch.h"
#include <GameEngineCore/CoreMinimal.h>

Effect_DistortionGlitch::Effect_DistortionGlitch() 
	: CopyTarget(nullptr)
{
}

Effect_DistortionGlitch::~Effect_DistortionGlitch() 
{
	if (nullptr != CopyTarget)
	{
		delete CopyTarget;
		CopyTarget = nullptr;
	}
}

void Effect_DistortionGlitch::EffectInit()
{
	CopyTarget = new GameEngineRenderTarget();
	CopyTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	EffectSet.SetPipeLine("DistortionGlitch");


	Option.DeltaTime = 0.0f;
	EffectSet.ShaderResources.SetConstantBufferLink("RenderOption", &Option, sizeof(Option));
}

void Effect_DistortionGlitch::Effect(GameEngineRenderTarget* _Target)
{
	CopyTarget->Copy(_Target);

	SumDeltaTime += GameEngineTime::GetDeltaTime();
	Option.DeltaTime = SumDeltaTime;


	EffectSet.ShaderResources.SetTexture("Tex", CopyTarget->GetRenderTargetTexture(0));

	_Target->Clear();
	_Target->Setting();
	_Target->Effect(EffectSet);
}
