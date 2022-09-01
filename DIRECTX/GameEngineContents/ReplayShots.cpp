#include "PreCompile.h"
#include "ReplayShots.h"
#include <GameEngineCore/CoreMinimal.h>

void ReplayShots::AddScreenShot(GameEngineTexture* _Texture)
{
	Shots.push_back(_Texture);
}

void ReplayShots::SetScreenIndex(int _Index)
{
	//	
}

void ReplayShots::PlayReverse(float _PlaySpeed)
{
	ReplayStart = true;
	PlayDir = -1;
	CurIndex = static_cast<int>(Shots.size()) - 1;
	PlaySpeed = _PlaySpeed;
}

ReplayShots::ReplayShots()
	: PlaySpeed(1.0f)
	, BaseRenderer(nullptr)
	, ScreenRenderer(nullptr)
{
}

ReplayShots::~ReplayShots() 
{
}

void ReplayShots::Start()
{
	BaseRenderer = CreateComponent<GameEngineTextureRenderer>();
	BaseRenderer->SetTexture("spr_bunker_elevator_1.png");
	BaseRenderer->GetTransform().SetLocalScale({ 1280, 720, 1 });
	//BaseRenderer->GetTransform().SetLocalPosition({ -640, 360, 1 });
	

	ScreenRenderer = CreateComponent<GameEngineUIRenderer>();
	ScreenRenderer->GetTransform().SetLocalScale({ 1280, 720, 1 });
	ScreenRenderer->GetTransform().SetLocalPosition({ -640, 420, 1 });
}

void ReplayShots::OnEvent()
{
	BaseRenderer->On();
	ScreenRenderer->On();
	ReplayStart = false;
	FrameTime = 0.0f;


}

void ReplayShots::OffEvent()
{
	BaseRenderer->Off();
	ScreenRenderer->Off();
	ReplayStart = false;
}

void ReplayShots::Update(float _DeltaTime)
{
	if (false == ReplayStart)
	{
		return;
	}

	// 犁积 场
	if (CurIndex < 0 || CurIndex >= Shots.size())
	{
		ReplayStart = false;
		return;
	}

	// 开犁积
	FrameTime += _DeltaTime;

	if (FrameTime > 1 / 60.0f * PlaySpeed)
	{
		SetScreenIndex(CurIndex);
		CurIndex = CurIndex + PlayDir;
	}

}

void ReplayShots::End()
{
}

