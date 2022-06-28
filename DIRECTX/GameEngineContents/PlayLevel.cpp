#include "PreCompile.h"
#include "PlayLevel.h"
#include <GameEngineCore/CoreMinimal.h>

PlayLevel::PlayLevel() 
{
}

PlayLevel::~PlayLevel() 
{
}

void PlayLevel::Start()
{
	GameEngineCameraActor* Cam = CreateActor<GameEngineCameraActor>();
	Cam->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);

	Cam->GetTransform().SetLocalPosition({ 0, 0, -1000 });
}

void PlayLevel::Update(float _DeltaTime)
{
}

void PlayLevel::End()
{
}
