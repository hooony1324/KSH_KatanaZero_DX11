#include "PreCompile.h"
#include "Room_Factory2.h"

Room_Factory2::Room_Factory2() 
{
}

Room_Factory2::~Room_Factory2() 
{
}

void Room_Factory2::Start()
{
	Background = CreateComponent<GameEngineTextureRenderer>();
	Background->SetTexture("room_factory_3_2.png");
	Background->ScaleToTexture();
	Background->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(DEPTH_ACTOR::BACKGROUND_0) });
	Background->Off();

	InitCameraClampArea();
}

void Room_Factory2::Update(float _DeltaTime)
{
}

void Room_Factory2::End()
{
}

void Room_Factory2::OnEvent()
{
}

void Room_Factory2::OffEvent()
{
}
