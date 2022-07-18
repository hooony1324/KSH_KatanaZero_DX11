#include "PreCompile.h"
#include "TitleLevel.h"
#include <GameEngineCore/CoreMinimal.h>
#include <GameEngineCore/GameEngineCameraActor.h>

#include "TitlePlate.h"
#include "TitleFence.h"
#include "TitleGraphic.h"
#include "TitleGrass.h"
#include "TitlePlants.h"

TitleLevel::TitleLevel() 
{
}

TitleLevel::~TitleLevel() 
{
}

void TitleLevel::Start()
{
	// 카메라
	GameEngineCameraActor* MainCam = CreateActor<GameEngineCameraActor>();
	MainCam->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
	MainCam->GetTransform().SetLocalPosition({ 0, 0, 0 });

	// 배경
	GameEngineActor* Plate = CreateActor<TitlePlate>();
	GameEngineActor* Fence = CreateActor<TitleFence>();
	GameEngineActor* Graphic = CreateActor<TitleGraphic>();
	GameEngineActor* Grass = CreateActor<TitleGrass>();
	//GameEngineActor* Plants = CreateActor<TitlePlants>();
}

void TitleLevel::Update(float _DeltaTime)
{
}

void TitleLevel::End()
{
}
