#include "PreCompile.h"
#include "TestActor.h"
#include <GameEngineCore/CoreMinimal.h>

TestActor::TestActor() 
{
}

TestActor::~TestActor() 
{
}

void TestActor::Start()
{
	TRenderer = CreateComponent<GameEngineTextureRenderer>();
	//TRenderer->CreateFrameAnimationFolder("FanSpin", FrameAnimation_DESC{ "fanblade_spin", 0.05f, true });
	//TRenderer->ChangeFrameAnimation("FanSpin");
	//TRenderer->GetTransform().SetLocalScale({ 200, 200, 2 });
	//TRenderer->Option.IsMask = 1;





	//TRenderer->Option.IsMask = 1;

	TRenderer->SetPipeLine("UserCustom");
	ColorData ColorData;
	float4 FrameData;
	FrameData.PosX = 0.0f;
	FrameData.PosY = 0.0f;
	FrameData.SizeX = 1.0f;
	FrameData.SizeY = 1.0f;

	TRenderer->ShaderResources.SetConstantBufferLink("AtlasData", FrameData);
	TRenderer->ShaderResources.SetConstantBufferLink("ColorData", ColorData);
	TRenderer->Option.IsMask = 0;

	TRenderer->SetTexture("spr_bunker_elevator_1.png");
	TRenderer->GetTransform().SetLocalScale({ 200, 200, 2 });





	//DRenderer = CreateComponent<GameEngineDefaultRenderer>();


}

void TestActor::Update(float _DeltaTime)
{
}

void TestActor::End()
{
}
