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
		GameEngineInput::GetInst()->CreateKey("CamLeft", 'A');
		GameEngineInput::GetInst()->CreateKey("CamRight", 'D');
		GameEngineInput::GetInst()->CreateKey("CamUp", 'Q');
		GameEngineInput::GetInst()->CreateKey("CamDown", 'E');
		GameEngineInput::GetInst()->CreateKey("CamForward", 'W');
		GameEngineInput::GetInst()->CreateKey("CamBack", 'S');

		GameEngineInput::GetInst()->CreateKey("CamRotY+", 'R');
		GameEngineInput::GetInst()->CreateKey("CamRotY-", 'T');

	}

	{
		GameEngineCameraActor* actor = CreateActor<GameEngineCameraActor>();
		actor->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::PersPective);
		actor->GetTransform().SetLocalPosition({ 0.0f, 0.0f, -500.0f });

	}
	
	{
		GameEngineActor* actor = CreateActor<Cube>();
	}
}

void TestLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst()->IsPress("CamLeft"))
	{
		GetMainCameraActorTransform().SetLocalMove(-GetMainCameraActorTransform().GetRightVector() * 100 * _DeltaTime);
	}

	if (true == GameEngineInput::GetInst()->IsPress("CamRight"))
	{
		GetMainCameraActorTransform().SetLocalMove(GetMainCameraActorTransform().GetRightVector() * 100 * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("CamUp"))
	{
		GetMainCameraActorTransform().SetLocalMove(GetMainCameraActorTransform().GetUpVector() * 100 * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("CamDown"))
	{
		GetMainCameraActorTransform().SetLocalMove(-GetMainCameraActorTransform().GetUpVector() * 100 * _DeltaTime);
	}

	if (true == GameEngineInput::GetInst()->IsPress("CamForward"))
	{
		GetMainCameraActorTransform().SetLocalMove(GetMainCameraActorTransform().GetForwardVector() * 100 * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("CamBack"))
	{
		GetMainCameraActorTransform().SetLocalMove(-GetMainCameraActorTransform().GetForwardVector() * 100 * _DeltaTime);
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
}

void TestLevel::End()
{
}
