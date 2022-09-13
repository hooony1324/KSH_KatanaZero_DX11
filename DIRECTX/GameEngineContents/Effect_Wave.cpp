#include "PreCompile.h"
#include "Effect_Wave.h"
#include <GameEngineCore/CoreMinimal.h>

Effect_Wave* Effect_Wave::Inst = nullptr;

Effect_Wave::Effect_Wave() 
{
}

Effect_Wave::~Effect_Wave() 
{
	Inst = nullptr;

}

void Effect_Wave::EffectInit()
{
	Inst = this;

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

	if (false == OnOffOption)
	{
		SumDeltaTime = 0.0f;
		return;
	}

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
