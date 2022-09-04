#include "PreCompile.h"
#include "TestActor.h"
#include <GameEngineCore/CoreMinimal.h>

#include "GameContentsCustomRenderer.h"
#include <GameEngineCore/GameEngineRenderSet.h>
#include <GameEngineBase/GameEngineDebug.h>

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

	// 마스크 애니메이션
	TestRenderer = CreateComponent<GameContentsCustomRenderer>();
	TestRenderer->SetSamplingModePoint();
	TestRenderer->SetTexture("spr_psychboss_attack_knife_1.png");
	TestRenderer->ScaleToTexture();
	TestRenderer->CreateFrameAnimationFolder("portal", CustomFrameAnimation_DESC{ "portal_cutout", 0.05f, true });
	TestRenderer->ChangeFrameAnimation("portal");
	TestRenderer->SetPivot(PIVOTMODE::CENTER);

	// 주사기
	KnifeRenderer = CreateComponent<GameContentsCustomRenderer>();
	KnifeRenderer->SetTexture("spr_psychboss_attack_knife_1.png");
	KnifeRenderer->ScaleToTexture();
	
	// atlas data : float2(pos pos), floa2(size size)



	// 주사기 + 마스크
	// 주사기가 점점 바깥으로 나옴
	MaskedRenderer = CreateComponent<GameContentsCustomRenderer>();
	MaskedRenderer->SetTexture("spr_psychboss_tentacle_idle_0.png");
	MaskedRenderer->GetTransform().SetLocalScale({ 60, 160, GetDepth(ACTOR_DEPTH::COLLISION) });
	MaskedRenderer->CreateFrameAnimationFolder("idle", CustomFrameAnimation_DESC{ "tentacleboss_idle", 0.12f });
	MaskedRenderer->CreateFrameAnimationFolder("hurt", CustomFrameAnimation_DESC{ "tentacleboss_hurt", 0.5f, false });
	MaskedRenderer->CreateFrameAnimationFolder("attack", CustomFrameAnimation_DESC{ "tentacleboss_attack", 0.12f, false });
	MaskedRenderer->CreateMaskAnimationFolder("portal", CustomFrameAnimation_DESC{ "portal_cutoutY", 0.065f, true });
	MaskedRenderer->ChangeMaskAnimation("portal");
	MaskedRenderer->Option.IsMask = 1;
	MaskedRenderer->ChangeFrameAnimation("idle");
	

	GetTransform().SetWorldScale({ 2, 2, 1 });


	TestRenderer->GetTransform().SetWorldPosition({ 300, 250 });
	KnifeRenderer->GetTransform().SetWorldPosition({ -300, 250 });
	MaskedRenderer->GetTransform().SetWorldPosition({ -300, 0 });
	KnifeRenderer->GetPixelData().Slice.x = 0;
	TestRenderer->GetPixelData().Slice.x = 0;

}

void TestActor::Update(float _DeltaTime)
{
	if (GameEngineInput::GetInst()->IsPress("W"))
	{
		// EffectSet.ShaderResources.SetTexture("Tex", CopyTarget->GetRenderTargetTexture(0));
		// GetLevel()->GetMainCamera()->GetCameraRenderTarget()->GetRenderTargetTexture(0)
	}

	if (GameEngineInput::GetInst()->IsPress("S"))
	{
		KnifeRenderer->GetAtlasData().FrameData.x -= 0.1f;
	}

	if (GameEngineInput::GetInst()->IsPress("D"))
	{
		if (MaskedRenderer->GetMaskData().MaskFrameData.x >= 1.0f )
		{
			MaskedRenderer->GetMaskData().MaskFrameData.x = 1.0f;
			return;
		}
		float ScaleX = MaskedRenderer->GetTransform().GetLocalScale().x;
		float MoveX = 100 * _DeltaTime;
		MaskedRenderer->GetTransform().SetWorldMove(float4{ MoveX, 0 ,0, 0});
		MaskedRenderer->GetMaskData().MaskFrameData.x += MoveX/2 / ScaleX;
	}

	if (GameEngineInput::GetInst()->IsPress("A"))
	{
		if (MaskedRenderer->GetMaskData().MaskFrameData.x <= 0.0f)
		{
			MaskedRenderer->GetMaskData().MaskFrameData.x = 0.0f;
			return;
		}
		float ScaleX = MaskedRenderer->GetTransform().GetLocalScale().x;
		float MoveX = 100 * _DeltaTime;
		MaskedRenderer->GetTransform().SetWorldMove(float4{ -MoveX, 0 ,0, 0 });
		MaskedRenderer->GetMaskData().MaskFrameData.x -= MoveX/2 / ScaleX;
	}

}

void TestActor::End()
{
}
