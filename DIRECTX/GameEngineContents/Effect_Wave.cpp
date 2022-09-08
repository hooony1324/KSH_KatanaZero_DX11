#include "PreCompile.h"
#include "Effect_Wave.h"
#include <GameEngineCore/CoreMinimal.h>

bool Effect_Wave::OnOffOption;

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
	OnOffOption = false;

	Option.DeltaTime = 0.0f;
	
	if (true == EffectSet.ShaderResources.IsConstantBuffer("CustomRenderOption"))
	{
		EffectSet.ShaderResources.SetConstantBufferLink("CustomRenderOption", &Option, sizeof(Option));
		Option.OnOff = static_cast<int>(OnOffOption);
	}
	
}

void Effect_Wave::Effect(GameEngineRenderTarget* _Target)
{
	CopyTarget->Copy(_Target);

	float DeltaTime = GameEngineTime::GetDeltaTime();
	SumDeltaTime += DeltaTime;
	Option.DeltaTime = DeltaTime;
	Option.SumDeltaTime = SumDeltaTime;
	Option.OnOff = static_cast<int>(OnOffOption);
	EffectSet.ShaderResources.SetTexture("Tex", CopyTarget->GetRenderTargetTexture(0));

	_Target->Clear();
	_Target->Setting();
	_Target->Effect(EffectSet);
}
