#include "PreCompile.h"
#include "EndingLevel.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineCameraActor.h>

#include "Ending_Background.h"
#include "Ending_Logo.h"

EndingLevel::EndingLevel() 
{
}

EndingLevel::~EndingLevel() 
{
}

void EndingLevel::Start()
{
	GameEngineCameraActor* Cam = CreateActor<GameEngineCameraActor>(ACTORGROUP::CAMERA);
	Cam->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
	Cam->GetTransform().SetWorldPosition({ 0, 0, -100 });


	Ending_Background* BG = CreateActor<Ending_Background>();
	BG->GetTransform().SetWorldPosition({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_0)});

	Logo = CreateActor<Ending_Logo>();
	Logo->GetTransform().SetWorldMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_1)});
	Lerp_Start = float4{ 0, -100 };
	Lerp_Dest = float4{ 0, 50 };
}

void EndingLevel::OnEvent()
{
	Logo->GetTransform().SetWorldPosition(Lerp_Start);
	SumDeltaTime = 0.0f;

	SoundPlayer = GameEngineSound::SoundPlayControl("song_fullconfession_cut.wav", 1);
	SoundPlayer.Volume(0.5f);
}

void EndingLevel::Update(float _DeltaTime)
{
	// Lerp
	float4 Pos = float4::LerpLimit(Lerp_Start, Lerp_Dest, SumDeltaTime);
	Logo->GetTransform().SetWorldPosition(float4{ Pos.x, Pos.y });
	SumDeltaTime += _DeltaTime;
}

void EndingLevel::OffEvent()
{
	SoundPlayer.Stop();
}

void EndingLevel::End()
{
}
