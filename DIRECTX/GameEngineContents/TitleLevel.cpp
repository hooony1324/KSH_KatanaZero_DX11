#include "PreCompile.h"
#include "TitleLevel.h"
#include <GameEngineCore/CoreMinimal.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GEngine.h>
#include "ControlGUI.h"

#include "TitleUI.h"
#include "TitlePlate.h"
#include "TitleFence.h"
#include "TitleGraphic.h"
#include "TitleGrass.h"
#include "TitlePlants.h"
#include "TitleButtons.h"

const float LERP_DIST = 720;

TitleLevel::TitleLevel() 
{
}

TitleLevel::~TitleLevel() 
{
}
GameEngineActor* Graphic;
void TitleLevel::Start()
{
	// 카메라
	//GameEngineCameraActor* MainCam = CreateActor<GameEngineCameraActor>();
	//MainCam->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
	//MainCam->GetTransform().SetWorldPosition({ 0, 0, -1000 });

	// 배경
	GameEngineActor* Plate = CreateActor<TitlePlate>();
	GameEngineActor* Fence = CreateActor<TitleFence>();
	Graphic = CreateActor<TitleGraphic>();
	GameEngineActor* Grass = CreateActor<TitleGrass>();
	GameEngineActor* Plants = CreateActor<TitlePlants>();
	GameEngineActor* Buttons = CreateActor<TitleButtons>();
	AllActors.push_back(Plate);
	AllActors.push_back(Fence);
	AllActors.push_back(Graphic);
	AllActors.push_back(Grass);
	AllActors.push_back(Plants);
	AllActors.push_back(Buttons);

}

void TitleLevel::LevelStartEvent()
{
	if (false == ControlGUI::GetInst()->IsUpdate())
	{
		ControlGUI::GetInst()->On();
	}

	SoundPlayer_BGM = GameEngineSound::SoundPlayControl("song_rainonbrick.ogg");
	SoundPlayer_BGM.Volume(0.1f);
}

void TitleLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst()->IsDown("Numpad0"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
	}



	PlayLevelChangeCheck();

	if (false == PlayLevelChange)
	{
		return;
	}

	// Level Change
	if (false == ChangeEnd)
	{
		if (false == Changing)
		{
			SoundPlayer_Transition = GameEngineSound::SoundPlayControl("sound_ui_transition_drone_01.wav");
			SoundPlayer_Transition.Volume(0.1f);
			Changing = true;
			
			for (auto Val : AllActors)
			{
				TitleUI* PtrUI = dynamic_cast<TitleUI*>(Val);
				PtrUI->LerpStart(PtrUI->GetTransformData().WorldPosition + float4{0, 1000}, 0.001f);

			}

		}

		SumDeltaTime += _DeltaTime;

		if (1.8f < SumDeltaTime)
		{
			ChangeEnd = true;
		}
	}
	else
	{
		SoundPlayer_Transition.Stop();
		GEngine::ChangeLevel("PlayLevel");
	}
}

void TitleLevel::LevelEndEvent()
{
	SoundPlayer_BGM.Stop();
}

void TitleLevel::End()
{
}

void TitleLevel::PlayLevelChangeCheck()
{
	TitleButtons* Buttons = dynamic_cast<TitleButtons*>(AllActors.back());
	PlayLevelChange = Buttons->CheckPlayLevelChange();
}
