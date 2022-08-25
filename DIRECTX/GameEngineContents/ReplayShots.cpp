#include "PreCompile.h"
#include "ReplayShots.h"
#include <GameEngineCore/CoreMinimal.h>

void ReplayShots::AddScreenShot(GameEngineTexture* _Texture)
{
	Shots.push_back(_Texture);
}

void ReplayShots::SetScreenIndex(int _Index)
{
	ScreenRenderer->SetTexture(Shots[_Index]);
}

void ReplayShots::PlayReverse(float _PlaySpeed)
{
	ReplayStart = true;
	PlayDir = -1;
	CurIndex = Shots.size() - 1;
	PlaySpeed = _PlaySpeed;
}

ReplayShots::ReplayShots()
	: PlaySpeed(1.0f)
{
}

ReplayShots::~ReplayShots() 
{
}

void ReplayShots::Start()
{
	BaseRenderer = CreateComponent<GameEngineTextureRenderer>();
	BaseRenderer->SetTexture("None.png");
	BaseRenderer->GetTransform().SetLocalScale({ 1280, 720, 1 });
	BaseRenderer->GetTransform().SetLocalPosition({ -640, 360, 1 });
	

	ScreenRenderer = CreateComponent<GameEngineTextureRenderer>();
	ScreenRenderer->GetTransform().SetLocalScale({ 1280, 720, 1 });
	ScreenRenderer->GetTransform().SetLocalPosition({ -640, 360, 1 });
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

