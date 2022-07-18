#include "PreCompile.h"
#include "BossLevel.h"
#include <GameEngineCore/GameEngineCameraActor.h>

BossLevel::BossLevel() 
{
}

BossLevel::~BossLevel() 
{
}

void BossLevel::Start()
{
	GameEngineCameraActor* Cam = CreateActor<GameEngineCameraActor>(ACTORGROUP::CAMERA);
	Cam->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
	Cam->GetTransform().SetLocalPosition({ 0, 0, 0 });
}

void BossLevel::Update(float _DeltaTime)
{
}

void BossLevel::End()
{
}
