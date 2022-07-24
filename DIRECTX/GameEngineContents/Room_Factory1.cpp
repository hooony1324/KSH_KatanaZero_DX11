#include "PreCompile.h"
#include "Room_Factory1.h"

Room_Factory1::Room_Factory1() 
{
}

Room_Factory1::~Room_Factory1() 
{
}

void Room_Factory1::Start()
{
	Background = CreateComponent<GameEngineTextureRenderer>();
	Background->SetTexture("room_factory_2.png");
	Background->ScaleToTexture();

}

void Room_Factory1::Update(float _DeltaTime)
{
}

void Room_Factory1::End()
{
}
