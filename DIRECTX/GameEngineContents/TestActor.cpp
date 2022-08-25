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


	// 마스크(애니메이션) 주사기(점점 바깥으로 나옴)
	GameContentsCustomRenderer* MaskedRenderer = CreateComponent<GameContentsCustomRenderer>();
	MaskedRenderer->SetSamplingModePoint();
	MaskedRenderer->SetTexture("spr_psychboss_attack_knife_1.png");
	MaskedRenderer->ScaleToTexture();
	//MaskedRenderer->CreateMaskAnimationFolder("portal", CustomFrameAnimation_DESC{ "portal_cutout", 0.1f, true });
	//MaskedRenderer->ChangeMaskAnimation("portal");
	//MaskedRenderer->Option.IsMask = 1;


	// 실험실험실험실험실험실험실험실험실험실험실험
		/*case PIVOTMODE::LEFT:
			AtlasDataInst.PivotPos = float4(0.5f, 0.0f, 0.0f, 0.0f);
			break;*/

	float4 Scale = KnifeRenderer->GetTransform().GetLocalScale();
	MaskedRenderer->GetAtlasData().PivotPos = float4(1.2f, 0.0, 0.0f, 0.0f);
	//MaskedRenderer->GetTransform().SetLocalScale({ Scale.x * 0.5f, Scale.y * 1.0f, Scale.z });

	// 실험실험실험실험실험실험실험실험실험실험실험


	GetTransform().SetWorldScale({ 2, 2, 1 });
	TRenderer->GetTransform().SetWorldPosition({ -300, 250 });
	KnifeRenderer->GetTransform().SetWorldPosition({ 300, 250 });
}

void TestActor::Update(float _DeltaTime)
{
	// pos pos size size 
}

void TestActor::End()
{
}
