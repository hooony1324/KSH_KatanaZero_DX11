#include "PreCompile.h"
#include "Portal.h"
#include <GameEngineCore/CoreMinimal.h>

Portal::Portal() 
{
}

Portal::~Portal() 
{
}


void Portal::Start()
{
	Renderer_Base = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Base->SetSamplingModePoint();
	Renderer_Base->SetTexture("spr_portal_loop_back_0.png");
	Renderer_Base->ScaleToTexture();
	Renderer_Base->CreateFrameAnimationFolder("open", FrameAnimation_DESC{"portal_open", 0.065f, false});
	Renderer_Base->CreateFrameAnimationFolder("loop", FrameAnimation_DESC{"portal_loopback", 0.065f });
	Renderer_Base->CreateFrameAnimationFolder("close", FrameAnimation_DESC{"portal_close", 0.065f });
	Renderer_Base->Off();

	Renderer_OutLine = CreateComponent<GameEngineTextureRenderer>();
	Renderer_OutLine->SetSamplingModePoint();
	Renderer_OutLine->SetTexture("spr_portal_loop_front_0.png");
	Renderer_OutLine->ScaleToTexture();
	Renderer_OutLine->SetPivot(PIVOTMODE::LEFT);
	Renderer_OutLine->CreateFrameAnimationFolder("loop", FrameAnimation_DESC{ "portal_loopfront", 0.065f });
	Renderer_OutLine->Off();

	AllAnimationBind();

	GetTransform().SetLocalScale({ 2, 2, 1 });
	GetTransform().SetLocalRotate({ 0, 0, 180 });
	Renderer_OutLine->GetTransform().SetWorldMove({ 0, 0, -200});

	LoopTime = 3.0f;
}


void Portal::OnEvent()
{
	Renderer_Base->On();
	Renderer_Base->ChangeFrameAnimation("open");
	IsClosing = false;
}

void Portal::OffEvent()
{
	Renderer_Base->Off();
	Renderer_OutLine->Off();
	ReSetAccTime();
}

void Portal::Update(float _DeltaTime)
{
	if (GetAccTime() >= LoopTime && false == IsClosing)
	{
		IsClosing = true;
		Renderer_Base->ChangeFrameAnimation("close");
		Renderer_OutLine->Off();
	}
}

void Portal::End()
{
}

void Portal::AllAnimationBind()
{
	Renderer_Base->AnimationBindEnd("open",
		[=](const FrameAnimation_DESC& _Info)
		{ 
			Renderer_Base->ChangeFrameAnimation("loop");
			Renderer_Base->CurAnimationReset();
			Renderer_OutLine->On();
			Renderer_OutLine->ChangeFrameAnimation("loop");
			Renderer_OutLine->CurAnimationReset();
		}
	);

	// close() {}
	//	Renderer_Base->ChangeFrameAnimation("close");
	//	Renderer_OutLine->Off();

	Renderer_Base->AnimationBindEnd("close",
		[=](const FrameAnimation_DESC& _Info)
		{
			Renderer_Base->Off();
			Off();
		}
	);

}
