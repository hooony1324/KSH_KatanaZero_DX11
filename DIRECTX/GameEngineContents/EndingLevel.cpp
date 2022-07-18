#include "PreCompile.h"
#include "EndingLevel.h"
#include <GameEngineCore/GameEngineCameraActor.h>

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
	Cam->GetTransform().SetLocalPosition({ 0, 0, 0 });
}

void EndingLevel::Update(float _DeltaTime)
{
}

void EndingLevel::End()
{
}
