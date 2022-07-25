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
	Background->Off();
	Background->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(DEPTH_ACTOR::BACKGROUND_0) });

	// test
	GameEngineTextureRenderer* Transition = CreateComponent<GameEngineTextureRenderer>();
	Transition->CreateFrameAnimationFolder("transition_in", FrameAnimation_DESC{ "transition_in", 1.0f });
	Transition->GetTransform().SetLocalScale({32, 32, 1 });
	Transition->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(DEPTH_ACTOR::TRANSITION) });
	Transition->ChangeFrameAnimation("transition_in");
}

void Room_Factory1::OnEvent()
{
	Background->On();

	// 지형, 몬스터 소환

}

void Room_Factory1::OffEvent()
{
	Background->Off();
}

void Room_Factory1::Update(float _DeltaTime)
{
}

void Room_Factory1::End()
{
}


