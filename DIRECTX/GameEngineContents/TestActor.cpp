#include "PreCompile.h"
#include "TestActor.h"
#include <GameEngineCore/CoreMinimal.h>

#include "GameContentsCustomRenderer.h"

TestActor::TestActor() 
{
}

TestActor::~TestActor() 
{
}

void TestActor::Start()
{
	TRenderer = CreateComponent<GameContentsCustomRenderer>();
	TRenderer->CreateFrameAnimationFolder("FanSpin", FrameAnimation_DESC{ "fanblade_spin", 0.05f, true });
	TRenderer->ChangeFrameAnimation("FanSpin");
	TRenderer->GetTransform().SetLocalScale({ 200, 200, 2 });
	
	TRenderer->Option.IsMask = 1;
	TRenderer->SetMask("spr_lightcone_0.png");




	//DRenderer = CreateComponent<GameEngineDefaultRenderer>();


}

void TestActor::Update(float _DeltaTime)
{
}

void TestActor::End()
{
}
