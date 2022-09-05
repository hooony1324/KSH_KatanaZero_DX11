#include "PreCompile.h"
#include "Effect_Wave.h"
#include <GameEngineCore/CoreMinimal.h>

Effect_Wave::Effect_Wave() 
	: CopyTarget(nullptr)
{
}

Effect_Wave::~Effect_Wave() 
{
	if (nullptr != CopyTarget)
	{
		delete CopyTarget;
		CopyTarget = nullptr;
	}
}

void Effect_Wave::EffectInit()
{
	CopyTarget = new GameEngineRenderTarget();
	CopyTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	EffectSet.SetPipeLine("Wave");


	Option.DeltaTime = 0.0f;
	
	if (true == EffectSet.ShaderResources.IsConstantBuffer("RenderOption"))
	{
		EffectSet.ShaderResources.SetConstantBufferLink("RenderOption", &Option, sizeof(Option));

	}
	
}

void Effect_Wave::Effect(GameEngineRenderTarget* _Target)
{
	CopyTarget->Copy(_Target);

	SumDeltaTime += GameEngineTime::GetDeltaTime();
	Option.DeltaTime = SumDeltaTime;

	EffectSet.ShaderResources.SetTexture("Tex", CopyTarget->GetRenderTargetTexture(0));

	_Target->Clear();
	_Target->Setting();
	_Target->Effect(EffectSet);
}
