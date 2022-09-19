#include "PreCompile.h"
#include "TestLevel.h"
#include <GameEngineCore/CoreMinimal.h>
#include "ControlGUI.h"

#include "TestActor.h"
#include "Portal.h"
#include "Effect_Wave.h"
#include "Effect_Distortion.h"
#include "Effect_DistortionGlitch.h"

#include "TestMonster.h"

TestLevel::TestLevel() 
{
}

TestLevel::~TestLevel() 
{
}

Portal* portal;
void TestLevel::Start()
{
	TestActor* actor = CreateActor<TestActor>();

	portal = CreateActor<Portal>();
	portal->GetTransform().SetWorldMove(float4::RIGHT * 100);
	portal->Off();

	TestMonster* monster = CreateActor<TestMonster>();
}


void TestLevel::LevelStartEvent()
{
	if (false == ControlGUI::GetInst()->IsUpdate())
	{
		ControlGUI::GetInst()->On();
	}


	GetMainCamera()->GetCameraRenderTarget()->AddEffect<Effect_Wave>();
	GetMainCamera()->GetCameraRenderTarget()->AddEffect<Effect_DistortionGlitch>();
	GetMainCamera()->GetCameraRenderTarget()->AddEffect<Effect_Distortion>();

}

void TestLevel::Update(float _DeltaTime)
{

	if (true == GameEngineInput::GetInst()->IsPress("Shift"))
	{
		portal->On();
	}

	if (true == GameEngineInput::GetInst()->IsDown("NumPad0"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
	}

}


void TestLevel::End()
{
}
