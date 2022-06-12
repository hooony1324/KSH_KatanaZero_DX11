#include "TestLevel.h"
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineBase/GameEngineInput.h>
#include "Cube.h"

TestLevel::TestLevel() 
{
}

TestLevel::~TestLevel() 
{
}

void TestLevel::Start()
{

	if (false == GameEngineInput::GetInst()->IsKey("CamLeft"))
	{
		GameEngineInput::GetInst()->CreateKey("CamLeft", 'a');
		GameEngineInput::GetInst()->CreateKey("CamRight", 'd');
		GameEngineInput::GetInst()->CreateKey("CamUp", 'q');
		GameEngineInput::GetInst()->CreateKey("CamDown", 'e');
		GameEngineInput::GetInst()->CreateKey("CamForward", 'w');
		GameEngineInput::GetInst()->CreateKey("CamBack", 's');

		GameEngineInput::GetInst()->CreateKey("CamRotY+", 'r');
		GameEngineInput::GetInst()->CreateKey("CamRotY-", 't');

	}

	{
		GameEngineCameraActor* actor = CreateActor<GameEngineCameraActor>();
		actor->GetTransform().SetLocalPosition({ 0, 0, -300.0f });
	}
	
	{
		GameEngineActor* actor = CreateActor<Cube>();
		actor->GetTransform().SetLocalPosition({ 0, 0, 0 });
	}
}

void TestLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst()->IsPress("CamLeft"))
	{
		GetMainCameraActorTransform().SetLocalMove(float4::LEFT * 100 * _DeltaTime);
	}

	if (true == GameEngineInput::GetInst()->IsPress("CamRight"))
	{
		GetMainCameraActorTransform().SetLocalMove(float4::RIGHT * 100 * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("CamUp"))
	{
		GetMainCameraActorTransform().SetLocalMove(float4::UP * 100 * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("CamDown"))
	{
		GetMainCameraActorTransform().SetLocalMove(float4::DOWN * 100 * _DeltaTime);
	}

	if (true == GameEngineInput::GetInst()->IsPress("CamForward"))
	{
		GetMainCameraActorTransform().SetLocalMove(float4::FORWARD * 100 * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("CamBack"))
	{
		GetMainCameraActorTransform().SetLocalMove(float4::BACK * 100 * _DeltaTime);
	}

	static float4 Rot = { 0.0f, 0.0f, 0.0f };
	if (true == GameEngineInput::GetInst()->IsPress("CamRotY+"))
	{
		Rot.y += 360.0f * _DeltaTime;
	}
	if (true == GameEngineInput::GetInst()->IsPress("CamRotY-"))
	{
		Rot.y -= 360.0f * _DeltaTime;
	}

	GetMainCameraActorTransform().SetLocalRotation(Rot);

}

void TestLevel::End()
{
}
