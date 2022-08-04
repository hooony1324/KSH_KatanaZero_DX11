#include "PreCompile.h"
#include "EndingLevel.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineCameraActor.h>

#include "Ending_Background.h"
#include "Ending_Logo.h"
#include "Ending_Transition.h"

EndingLevel::EndingLevel() 
{
}

EndingLevel::~EndingLevel() 
{
}

void EndingLevel::Start()
{
	Ending_Background* BG = CreateActor<Ending_Background>();

	Logo = CreateActor<Ending_Logo>();
	Lerp_Start = float4{ 0, -100, GetDepth(ACTOR_DEPTH::LOGO) };
	Lerp_Dest = float4{ 0, 50, GetDepth(ACTOR_DEPTH::LOGO) };

	Ending_Transition* Transition = CreateActor<Ending_Transition>();
	Transition->GetTransform().SetWorldPosition({ 0, 0, GetDepth(ACTOR_DEPTH::TRANSITION) });
}

void EndingLevel::OnEvent()
{
	Logo->GetTransform().SetWorldPosition(Lerp_Start);
	SumDeltaTime = 0.0f;

	SoundPlayer = GameEngineSound::SoundPlayControl("song_fullconfession_cut.wav", 1);
	SoundPlayer.Volume(0.2f);
}

void EndingLevel::Update(float _DeltaTime)
{
	if (GetAccTime() < 2.0f)
	{
		return;
	}

	// Lerp
	float4 Pos = float4::LerpLimit(Lerp_Start, Lerp_Dest, SumDeltaTime);
	Pos.z = 0;
	Logo->GetTransform().SetWorldPosition(Pos);
	SumDeltaTime += _DeltaTime;
}

void EndingLevel::OffEvent()
{
	SoundPlayer.Stop();
}

void EndingLevel::End()
{
}
