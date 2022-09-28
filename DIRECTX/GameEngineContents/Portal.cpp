#include "PreCompile.h"
#include "Portal.h"
#include <GameEngineCore/CoreMinimal.h>

void Portal::OnTimer(float _WakeTime)
{
	WakeTime = _WakeTime;
}

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
	Renderer_Base->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));

	Renderer_OutLine = CreateComponent<GameEngineTextureRenderer>();
	Renderer_OutLine->SetSamplingModePoint();
	Renderer_OutLine->SetTexture("spr_portal_loop_front_0.png");
	Renderer_OutLine->ScaleToTexture();
	Renderer_OutLine->SetPivot(PIVOTMODE::LEFT);
	Renderer_OutLine->CreateFrameAnimationFolder("loop", FrameAnimation_DESC{ "portal_loopfront", 0.065f });
	Renderer_OutLine->Off();
	Renderer_OutLine->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));

	AllAnimationBind();

	GetTransform().SetLocalScale({ 2, 2, 1 });
	Renderer_Base->GetTransform().SetLocalRotate({ 0, 0, 180 });
	Renderer_OutLine->GetTransform().SetLocalRotate({ 0, 0, 180 });
	Renderer_OutLine->GetTransform().SetWorldMove({ 0, 0, -200});

	LoopTime = 2.5f;
}


void Portal::OnEvent()
{
	Renderer_Base->Off();
	Renderer_Base->ReSetAccTime();
	Renderer_OutLine->Off();
	GetTransform().SetWorldRotation({ 0, 0, 0 });

	IsOpend = false;
	WakeTime = 0;

}

void Portal::OffEvent()
{
	Renderer_Base->Off();
	Renderer_Base->ReSetAccTime();
	Renderer_OutLine->Off();
	GetTransform().SetWorldRotation({ 0, 0, 0 });

	IsOpend = false;
	WakeTime = 0;
}

void Portal::Update(float _DeltaTime)
{
	WakeTime -= _DeltaTime;

	if (WakeTime > 0.0f)
	{
		return;
	}

	if (false == IsOpend)
	{
		Renderer_Base->On();
		Renderer_Base->ChangeFrameAnimation("open");
		IsOpend = true;
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

	Renderer_Base->AnimationBindFrame("loop",
		[=](const FrameAnimation_DESC& _Info)
		{
			if (Renderer_Base->GetAccTime() > LoopTime)
			{
				Renderer_Base->ChangeFrameAnimation("close");
				Renderer_OutLine->Off();
			}
		}
	);


	Renderer_Base->AnimationBindEnd("close",
		[=](const FrameAnimation_DESC& _Info)
		{
			Off();
		}
	);

}
