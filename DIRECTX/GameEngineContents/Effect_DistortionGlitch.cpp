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

	OnOffOption = false;

	if (true == EffectSet.ShaderResources.IsConstantBuffer("CustomRenderOption"))
	{
		Option.DeltaTime = 0.0f;
		EffectSet.ShaderResources.SetConstantBufferLink("CustomRenderOption", &Option, sizeof(Option));
		Option.OnOff = static_cast<int>(OnOffOption);
	}
	
}

void Effect_DistortionGlitch::Effect(GameEngineRenderTarget* _Target)
{
	CopyTarget->Copy(_Target);


	if (false == OnOffOption)
	{
		SumDeltaTime = 0.0f;
		return;
	}

	float DeltaTime = GameEngineTime::GetDeltaTime();
	SumDeltaTime += DeltaTime;
	Option.DeltaTime = DeltaTime;
	Option.SumDeltaTime = SumDeltaTime;
	Option.OnOff = static_cast<int>(OnOffOption);

	EffectSet.ShaderResources.SetTexture("Tex", CopyTarget->GetRenderTargetTexture(0));


	if (false == EffectSoundPlayed && SumDeltaTime > 0.25f)
	{
		EffectSoundPlayed = true;
		EffectSoundPlayer = GameEngineSound::SoundPlayControl("sound_tv_thump.wav");
		EffectSoundPlayer.Volume(0.07f);
	}

	_Target->Clear();
	_Target->Setting();
	_Target->Effect(EffectSet);
}
