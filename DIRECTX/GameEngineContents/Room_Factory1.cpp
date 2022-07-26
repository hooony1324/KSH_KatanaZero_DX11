#include "PreCompile.h"
#include "Room_Factory1.h"
#include "Door.h"

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
	Background->GetTransform().SetLocalPosition({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_0) });
	Background->Off();

	// test
	//GameEngineTextureRenderer* Transition = CreateComponent<GameEngineTextureRenderer>();
	//Transition->CreateFrameAnimationFolder("transition_in", FrameAnimation_DESC{ "transition_in", 1.0f });
	//Transition->GetTransform().SetLocalScale({32, 32, 1 });
	//Transition->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(DEPTH_ACTOR::TRANSITION) });
	//Transition->ChangeFrameAnimation("transition_in");

	PtrDoor = GetLevel()->CreateActor<Door>();
	PtrDoor->GetTransform().SetWorldPosition({ -330, 72 });
	PtrDoor->Off();
	
	InitCameraClampArea();
}

void Room_Factory1::OnEvent()
{
	Background->On();

	// 지형 소환
	PtrDoor->Init();
	PtrDoor->On();
	

	// 적 소환

}

void Room_Factory1::OffEvent()
{
	Background->Off();

	PtrDoor->Off();
}

void Room_Factory1::Update(float _DeltaTime)
{
	auto val = PtrDoor->GetTransform().GetWorldPosition();
}

void Room_Factory1::End()
{
}


