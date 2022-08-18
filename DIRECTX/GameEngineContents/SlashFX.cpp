#include "PreCompile.h"
#include "SlashFX.h"
#include <GameEngineCore/CoreMinimal.h>

SlashFX::SlashFX()
	: SlashStart(false)
{
	
}

SlashFX::~SlashFX() 
{
}

void SlashFX::Start()
{
	// 벤 자리에 생성
	Renderer_SlashScar = CreateComponent<GameEngineTextureRenderer>();
	Renderer_SlashScar->CreateFrameAnimationFolder("slashscar", FrameAnimation_DESC{ "slashscar", 0.08f, false });
	Renderer_SlashScar->GetTransform().SetLocalScale({ 80, 80, 1 });
	Renderer_SlashScar->AnimationBindEnd("slashscar", [=](const FrameAnimation_DESC& _Info) { Renderer_SlashScar->Off(); });
	Renderer_SlashScar->ChangeFrameAnimation("slashscar");

	// 화면을 가로지름
	Renderer_SlashLight = CreateComponent<GameEngineTextureRenderer>();
	Renderer_SlashLight->SetTexture("spr_cursor_slash.png");
	Renderer_SlashLight->GetTransform().SetLocalScale({ 3014, 8, 1 });
	Renderer_SlashLight->GetTransform().SetLocalPosition({ 0, 0, 0 });
}

void SlashFX::Update(float _DeltaTime)
{
	if (false == SlashStart)
	{
		return;
	}

	Renderer_SlashLight->GetTransform().SetWorldMove(SlashDir * _DeltaTime * 7000.0f);

	if (GetAccTime() > 1.5f)
	{
		Death();
	}
}

void SlashFX::End()
{
}
