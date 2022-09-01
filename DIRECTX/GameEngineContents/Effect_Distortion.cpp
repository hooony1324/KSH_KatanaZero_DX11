#include "PreCompile.h"
#include "Effect_Distortion.h"
#include <GameEngineCore/CoreMinimal.h>

Effect_Distortion::Effect_Distortion() 
	: CopyTarget(nullptr)
{
}

Effect_Distortion::~Effect_Distortion() 
{
	if (nullptr != CopyTarget)
	{
		delete CopyTarget;
		CopyTarget = nullptr;
	}
}

void Effect_Distortion::EffectInit()
{
	CopyTarget = new GameEngineRenderTarget();
	CopyTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	EffectSet.SetPipeLine("Distortion");
	EffectSet.ShaderResources.SetTexture("EffectTex", "glitch.png");
}

void Effect_Distortion::Effect(GameEngineRenderTarget* _Target)
{
	CopyTarget->Copy(_Target);

	EffectSet.ShaderResources.SetTexture("Tex", CopyTarget->GetRenderTargetTexture(0));
	_Target->Clear();
	_Target->Setting();
	_Target->Effect(EffectSet);
}
