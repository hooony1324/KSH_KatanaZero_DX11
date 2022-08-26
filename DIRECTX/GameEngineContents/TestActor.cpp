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
	//ShotRenderer = CreateComponent<GameEngineTextureRenderer>();
	//ShotRenderer->SetSamplingModePoint();

	// ����ũ �ִϸ��̼�
	TestRenderer = CreateComponent<GameContentsCustomRenderer>();
	TestRenderer->SetSamplingModePoint();
	TestRenderer->SetTexture("spr_psychboss_attack_knife_1.png");
	TestRenderer->ScaleToTexture();
	TestRenderer->CreateFrameAnimationFolder("portal", CustomFrameAnimation_DESC{ "portal_cutout", 0.05f, true });
	TestRenderer->ChangeFrameAnimation("portal");
	TestRenderer->SetPivot(PIVOTMODE::CENTER);

	// �ֻ��
	KnifeRenderer = CreateComponent<GameContentsCustomRenderer>();
	KnifeRenderer->SetTexture("spr_psychboss_attack_knife_1.png");
	KnifeRenderer->ScaleToTexture();





	// �ֻ�� + ����ũ
	// �ֻ�Ⱑ ���� �ٱ����� ����
	MaskedRenderer = CreateComponent<GameContentsCustomRenderer>();
	MaskedRenderer->SetSamplingModePoint();
	MaskedRenderer->SetTexture("spr_psychboss_attack_knife_1.png");
	MaskedRenderer->ScaleToTexture();
	MaskedRenderer->CreateMaskAnimationFolder("portal", CustomFrameAnimation_DESC{ "portal_cutout", 0.1f, true });
	MaskedRenderer->ChangeMaskAnimation("portal");
	MaskedRenderer->Option.IsMask = 1;


	GetTransform().SetWorldScale({ 2, 2, 1 });


	TestRenderer->GetTransform().SetWorldPosition({ 300, 250 });
	KnifeRenderer->GetTransform().SetWorldPosition({ -300, 250 });
	KnifeRenderer->GetPixelData().Slice.x = 0;
	TestRenderer->GetPixelData().Slice.x = 0;
}

void TestActor::Update(float _DeltaTime)
{
	
}

void TestActor::End()
{
}
