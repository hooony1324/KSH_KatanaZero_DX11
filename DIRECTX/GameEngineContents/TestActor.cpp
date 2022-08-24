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
	TRenderer->SetTexture("spr_psychboss_attack_knife_0.png");
	TRenderer->CreateMaskAnimationFolder("portal", CustomFrameAnimation_DESC{ "portal_cutout", 0.05f, true });
	TRenderer->ChangeMaskAnimation("portal");
	TRenderer->SetPivot(PIVOTMODE::LEFT);
	TRenderer->GetTransform().SetLocalScale({ 200, 60, 2 });
	
	TRenderer->Option.IsMask = 1;

	GetTransform().SetWorldScale({ 2, 2, 1 });
}

void TestActor::Update(float _DeltaTime)
{


}

void TestActor::End()
{
}
