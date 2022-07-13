#include "PreCompile.h"
#include "Cube.h"
#include <GameEngineCore/CoreMinimal.h>
#include <GameEngineBase/GameEngineInput.h>
#include "GlobalValueManager.h"

#include "Obstacle.h"

#include <GameEngineCore/GameEngineDefaultRenderer.h>
#include <GameEngineCore/GameEngineRenderingPipeLine.h>
#include <GameEngineCore/GameEngineVertexShader.h>
#include <GameEngineCore/GameEngineConstantBuffer.h>
#include <GameEngineCore/GameEngineDevice.h>


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
		bodyRenderer = CreateComponent<GameEngineDefaultRenderer>();
		bodyRenderer->GetTransform().SetLocalScale({ 100, 100, 100 });
		bodyRenderer->SetPipeLine("Color");
		bodyRenderer->PipeLineHelper.SetConstantBufferLink("ResultColor", Color);

	}

	//{
	//	childRenderer = CreateComponent<GameEngineDefaultRenderer>();
	//	childRenderer->SetParent(bodyRenderer);
	//	childRenderer->GetTransform().SetLocalScale({1, 1, 0});
	//	childRenderer->GetTransform().SetLocalPosition({ 1,0,0 });
	//}

	//{
	//	childRenderer2 = CreateComponent<GameEngineDefaultRenderer>();
	//	childRenderer2->SetParent(this);
	//	childRenderer2->GetTransform().SetLocalScale({ 1, 1, 0 });
	//	childRenderer2->GetTransform().SetLocalPosition({ 1,0,0 });
	//}

	//auto obs = GetLevel()->CreateActor<Obstacle>(ACTORGROUP::OBSTACLE);
	//obs->SetParent(this);
	//obs->GetTransform().SetLocalScale({ 10, 10, 10 });
	//GetTransform().SetWorldScale({ 2, 2, 2 });
	//this;

	//obs->GetTransform().SetWorldPosition({ 10, 10, 0 });


}

void Cube::Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		Color.r += 1.0f * _DeltaTime;
		Color.b += 1.0f * _DeltaTime;
		GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		Color.r -= 1.0f * _DeltaTime;
		GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
	}
	if (STATE_PLAYER::JUMP == Input.HandleInput())
	{
		Color.g += 1.0f * _DeltaTime;
		GetTransform().SetWorldMove(GetTransform().GetUpVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerDown"))
	{
		Color.b -= 1.0f * _DeltaTime;
		Color.g -= 1.0f * _DeltaTime;
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
		bodyRenderer->GetTransform().SetLocalRotate({ 0.0f, 0.0f, 360.0f * _DeltaTime });
	}
	if (true == GameEngineInput::GetInst()->IsPress("Rot-"))
	{
		bodyRenderer->GetTransform().SetLocalRotate({ 0.0f, 0.0f, -360.0f * _DeltaTime });
	}

	//// 충돌체크
	//list_Obstacle = GetLevel()->GetConvertToGroup<Obstacle>(ACTORGROUP::OBSTACLE);

	//for (auto& Obs : list_Obstacle)
	//{
	//	if (GameEngineTransform::OBBToOBB(childRenderer->GetTransform(), Obs->GetTransform()) )
	//	{
	//		int a = 0;
	//		GameEngineDebug::OutPutString("부딪");
	//	}
	//}

	//GameEngineConstantBufferSetter& Data = bodyRenderer->GetPipeLine()->GetVertexShader()->GetConstantBufferSetter("TransformData");

	//const TransformData& DataRef = bodyRenderer->GetTransformData();

	//Data.Buffer->ChangeData(&DataRef, sizeof(TransformData));

	//GameEngineDevice::GetContext()->VSSetConstantBuffers(Data.BindPoint, 1, &Data.Buffer->Buffer);
}

void Cube::End()
{
}
