#include "PreCompile.h"
#include "Effect_Distortion.h"
#include <GameEngineCore/CoreMinimal.h>

Effect_Distortion* Effect_Distortion::Inst = nullptr;

Effect_Distortion::Effect_Distortion() 
{
}

Effect_Distortion::~Effect_Distortion() 
{

}

void Effect_Distortion::EffectInit()
{
	Inst = this;

	CopyTarget = new GameEngineRenderTarget();
	CopyTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	EffectSet.SetPipeLine("Distortion");
	OnOffOption = false;

	if (true == EffectSet.ShaderResources.IsConstantBuffer("CustomRenderOption"))
	{
		Option.DeltaTime = 0.0f;
		EffectSet.ShaderResources.SetConstantBufferLink("CustomRenderOption", &Option, sizeof(Option));
		Option.OnOff = static_cast<int>(OnOffOption);
	}

}

void Effect_Distortion::Effect(GameEngineRenderTarget* _Target)
{
	CopyTarget->Copy(_Target);


	if (false == OnOffOption)
	{
		SumDeltaTime = 0.0f;
		return;
	}


	if (false == EffectSoundPlayed)
	{
		EffectSoundPlayed = true;
		EffectSoundPlayer = GameEngineSound::SoundPlayControl("sound_ui_screen_glitch_02.wav");
		EffectSoundPlayer.Volume(0.07f);
	}

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
