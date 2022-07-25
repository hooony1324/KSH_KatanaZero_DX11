#include "PreCompile.h"
#include "Room_Boss.h"

Room_Boss::Room_Boss() 
{
}

Room_Boss::~Room_Boss() 
{
}

void Room_Boss::Start()
{
	Background = CreateComponent<GameEngineTextureRenderer>();
	Background->SetTexture("spr_psychboss_background_0.png");
	Background->GetTransform().SetLocalScale({ 1280, 720 });
	Background->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(DEPTH_ACTOR::BACKGROUND_0) });
	Background->Off();

	InitCameraClampArea();
}

void Room_Boss::Update(float _DeltaTime)
{
}

void Room_Boss::End()
{
}

void Room_Boss::OnEvent()
{
}

void Room_Boss::OffEvent()
{
}
