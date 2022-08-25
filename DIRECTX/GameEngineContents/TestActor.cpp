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
	ShotRenderer = CreateComponent<GameEngineTextureRenderer>();
	ShotRenderer->SetSamplingModePoint();

	TRenderer = CreateComponent<GameContentsCustomRenderer>();
	TRenderer->SetTexture("spr_psychboss_attack_knife_1.png");
	TRenderer->SetSamplingModePoint();
	TRenderer->ScaleToTexture();
	TRenderer->CreateFrameAnimationFolder("portal", CustomFrameAnimation_DESC{ "portal_cutout", 0.05f, true });
	TRenderer->ChangeFrameAnimation("portal");
	TRenderer->SetPivot(PIVOTMODE::CENTER);
	TRenderer->GetAtlasData().FrameData;


	GameContentsCustomRenderer* KnifeRenderer = CreateComponent<GameContentsCustomRenderer>();
	KnifeRenderer->SetTexture("spr_psychboss_attack_knife_1.png");
	KnifeRenderer->ScaleToTexture();


	// 마스크(애니메이션), 주사기가 점점 바깥으로 나옴
	GameContentsCustomRenderer* MaskedRenderer = CreateComponent<GameContentsCustomRenderer>();
	MaskedRenderer->SetSamplingModePoint();
	MaskedRenderer->SetTexture("spr_psychboss_attack_knife_1.png");
	MaskedRenderer->ScaleToTexture();
	MaskedRenderer->CreateMaskAnimationFolder("portal", CustomFrameAnimation_DESC{ "portal_cutout", 0.1f, true });
	MaskedRenderer->ChangeMaskAnimation("portal");
	MaskedRenderer->Option.IsMask = 1;



	GetTransform().SetWorldScale({ 2, 2, 1 });
	TRenderer->GetTransform().SetWorldPosition({ -300, 250 });
	KnifeRenderer->GetTransform().SetWorldPosition({ 300, 250 });
}

void TestActor::Update(float _DeltaTime)
{
}

void TestActor::End()
{
}
