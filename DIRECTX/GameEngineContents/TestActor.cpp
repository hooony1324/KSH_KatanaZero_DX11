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
	GameContentsCustomRenderer* back = CreateComponent<GameContentsCustomRenderer>();
	back->SetTexture("room_factory_2.png");
	//back->ScaleToTexture();


	TRenderer = CreateComponent<GameContentsCustomRenderer>();
	TRenderer->CreateFrameAnimationFolder("FanSpin", FrameAnimation_DESC{ "fanblade_spin", 0.05f, true });
	TRenderer->ChangeFrameAnimation("FanSpin");
	TRenderer->GetTransform().SetLocalScale({ 600, 600, 2 });
	
	TRenderer->Option.IsMask = 1;
	TRenderer->SetMask("spr_lightcone_0.png");

	// 마스크와 기본 텍스쳐 크기, 위치 따로 조정하는 Set함수 필요??
}

void TestActor::Update(float _DeltaTime)
{
}

void TestActor::End()
{
}
