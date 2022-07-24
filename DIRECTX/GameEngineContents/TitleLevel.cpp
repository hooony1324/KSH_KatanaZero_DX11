#include "PreCompile.h"
#include "TitleLevel.h"
#include <GameEngineCore/CoreMinimal.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GEngine.h>

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
	GameEngineCameraActor* MainCam = CreateActor<GameEngineCameraActor>();
	MainCam->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
	MainCam->GetTransform().SetLocalPosition({ 0, 0, -1000 });

	// 배경
	GameEngineActor* Plate = CreateActor<TitlePlate>();
	Plate->GetTransform().SetWorldMove({ 0, 0, 500 });
	GameEngineActor* Fence = CreateActor<TitleFence>();
	Fence->GetTransform().SetWorldMove({ 0, 0, 400 });
	Graphic = CreateActor<TitleGraphic>();
	Graphic->GetTransform().SetWorldMove({ 0, 0, 300 });
	GameEngineActor* Grass = CreateActor<TitleGrass>();
	Grass->GetTransform().SetWorldMove({ 0, 0, 200 });
	GameEngineActor* Plants = CreateActor<TitlePlants>();
	Plants->GetTransform().SetWorldMove({ 0, 0, 100 });
	GameEngineActor* Buttons = CreateActor<TitleButtons>();
	AllActors.push_back(Plate);
	AllActors.push_back(Fence);
	AllActors.push_back(Graphic);
	AllActors.push_back(Grass);
	AllActors.push_back(Plants);
	AllActors.push_back(Buttons);

}

void TitleLevel::OnEvent()
{
	SoundPlayer_BGM = GameEngineSound::SoundPlayControl("song_rainonbrick.ogg");
	SoundPlayer_BGM.Volume(0.5f);
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

void TitleLevel::OffEvent()
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
