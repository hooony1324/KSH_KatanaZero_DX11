#include "PreCompile.h"
#include "TestLevel.h"
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineBase/GameEngineInput.h>
#include "Cube.h"
#include "Obstacle.h"
#include "GlobalValueManager.h"
#include <GameEngineCore/GameEngineDefaultRenderer.h>

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
		GameEngineInput::GetInst()->CreateKey("CamLeft", VK_NUMPAD4);
		GameEngineInput::GetInst()->CreateKey("CamRight", VK_NUMPAD6);
		GameEngineInput::GetInst()->CreateKey("CamUp", VK_NUMPAD8);
		GameEngineInput::GetInst()->CreateKey("CamDown", VK_NUMPAD2);
		GameEngineInput::GetInst()->CreateKey("CamForward", VK_ADD);
		GameEngineInput::GetInst()->CreateKey("CamBack", VK_SUBTRACT);

		GameEngineInput::GetInst()->CreateKey("CamRotY+", VK_NUMPAD9);
		GameEngineInput::GetInst()->CreateKey("CamRotY-", VK_NUMPAD7);

	}

	{
		GameEngineCameraActor* actor = CreateActor<GameEngineCameraActor>();
		actor->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
		actor->GetTransform().SetLocalPosition({ 0.0f, 0.0f, -500.0f });
	}
	
	{
		player = CreateActor<Cube>(ACTORGROUP::PLAYER);
	}

	//SoundPlayer = GameEngineSound::SoundPlayControl("sound_slash_bullet.wav", 0);
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
