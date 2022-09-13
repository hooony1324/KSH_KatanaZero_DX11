#include "PreCompile.h"
#include "Effect_DistortionGlitch.h"
#include <GameEngineCore/CoreMinimal.h>

Effect_DistortionGlitch* Effect_DistortionGlitch::Inst = nullptr;

Effect_DistortionGlitch::Effect_DistortionGlitch() 
{
}

Effect_DistortionGlitch::~Effect_DistortionGlitch() 
{

}

void Effect_DistortionGlitch::EffectInit()
{
	Inst = this;

	CopyTarget = new GameEngineRenderTarget();
	CopyTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	EffectSet.SetPipeLine("DistortionGlitch");

	if (true == EffectSet.ShaderResources.IsConstantBuffer("CustomRenderOption"))
	{
		Option.DeltaTime = 0.0f;
		EffectSet.ShaderResources.SetConstantBufferLink("CustomRenderOption", &Option, sizeof(Option));
	}
	
}

void Effect_DistortionGlitch::Effect(GameEngineRenderTarget* _Target)
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
