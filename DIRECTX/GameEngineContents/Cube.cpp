#include "PreCompile.h"
#include "Cube.h"
#include <GameEngineCore/CoreMinimal.h>
#include <GameEngineBase/GameEngineInput.h>
#include "GlobalValueManager.h"

Cube::Cube()
	: Speed(80.0f)
{
}

Cube::~Cube() 
{
}



void Cube::Start()
{
	if (false == GameEngineInput::GetInst()->IsKey("PlayerLeft"))
	{
		GameEngineInput::GetInst()->CreateKey("PlayerLeft", 'A');
		GameEngineInput::GetInst()->CreateKey("PlayerRight", 'D');
		GameEngineInput::GetInst()->CreateKey("PlayerUp", 'W');
		GameEngineInput::GetInst()->CreateKey("PlayerDown", 'S');
		GameEngineInput::GetInst()->CreateKey("PlayerForward", VK_UP);
		GameEngineInput::GetInst()->CreateKey("PlayerBack", VK_DOWN);
		GameEngineInput::GetInst()->CreateKey("Rot+", 'Q');
		GameEngineInput::GetInst()->CreateKey("Rot-", 'E');
	}

	{
		bodyRenderer = CreateComponent<GameEngineRenderer>();
		bodyRenderer->GetTransform().SetLocalScale({ 100, 100, 0 });
	}

	{
		childRenderer = CreateComponent<GameEngineRenderer>();
		childRenderer->GetTransform().SetParent(bodyRenderer->GetTransform());
		childRenderer->GetTransform().SetLocalScale({1, 1, 0});
		childRenderer->GetTransform().SetLocalPosition({ 1,0,0 });
	}

}

void Cube::Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerUp"))
	{
		GetTransform().SetWorldMove(GetTransform().GetUpVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerDown"))
	{
		GetTransform().SetWorldMove(GetTransform().GetDownVector() * Speed * _DeltaTime);
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerForward"))
	{
		GetTransform().SetWorldMove(GetTransform().GetForwardVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerBack"))
	{
		GetTransform().SetWorldMove(GetTransform().GetBackVector() * Speed * _DeltaTime);
	}

	if (true == GameEngineInput::GetInst()->IsPress("Rot+"))
	{
		GetTransform().SetLocalRotate({ 0.0f, 0.0f, 360.0f * _DeltaTime });
	}
	if (true == GameEngineInput::GetInst()->IsPress("Rot-"))
	{
		GetTransform().SetLocalRotate({ 0.0f, 0.0f, -360.0f * _DeltaTime });
	}

	// 충돌체크
	list_Obstacle = GetLevel()->GetConvertToGroup<Obstacle>(ACTORGROUP::OBSTACLE);

	for (auto& Obs : list_Obstacle)
	{
		if (GameEngineTransform::OBBToOBB(childRenderer->GetTransform(), Obs->GetTransform()) )
		{
			int a = 0;
			GameEngineDebug::OutPutString("부딪");
		}
	}
}

void Cube::End()
{
}
