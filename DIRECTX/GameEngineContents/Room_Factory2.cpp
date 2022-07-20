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
}

void Room_Factory2::Update(float _DeltaTime)
{
}

void Room_Factory2::End()
{
}
