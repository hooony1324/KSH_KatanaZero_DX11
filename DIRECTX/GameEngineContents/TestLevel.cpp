#include "PreCompile.h"
#include "TestLevel.h"
#include <GameEngineCore/CoreMinimal.h>
#include "ControlGUI.h"

#include "TestActor.h"
#include "Portal.h"
#include "Effect_Wave.h"
#include "Effect_Distortion.h"
#include "Effect_DistortionGlitch.h"
#include "Effect_PointLight.h"

#include "TestMonster.h"
#include "BloodLeaf.h"
#include "BossFloor.h"

#include "Cursor.h"

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

	//TestMonster* monster = CreateActor<TestMonster>();

	Cursor* cursor = CreateActor<Cursor>();


	BossFloor* Floor = CreateActor<BossFloor>();
	Floor->GetTransform().SetWorldPosition({ 0, -200, -1500 });
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
	GetMainCamera()->GetCameraRenderTarget()->AddEffect<Effect_PointLight>();

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
