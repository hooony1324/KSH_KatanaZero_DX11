#include "PreCompile.h"
#include "TestLevel.h"
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineBase/GameEngineInput.h>
#include "Cube.h"
#include "Obstacle.h"
#include "GlobalValueManager.h"

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
		actor->GetTransform().SetLocalPosition({ 0.0f, 0.0f, -1000.0f });

	}
	
	{
		player = CreateActor<Cube>(ACTORGROUP::PLAYER);

		monster = CreateActor<Obstacle>(ACTORGROUP::MONSTER);
		monster->GetTransform().SetLocalScale({ 5, 5, 0 });
		monster->GetTransform().SetWorldScale({ 20, 20, 0 });
		monster->GetTransform().SetWorldPosition({ 0, 200, 0 });
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


	// 충돌체크
	if (GameEngineTransform::OBBToOBB(player->GetTransform(), monster->GetTransform()))
	{
		int a = 0;
	}

}

void TestLevel::End()
{
}
