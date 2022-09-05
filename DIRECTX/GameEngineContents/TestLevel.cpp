#include "PreCompile.h"
#include "TestLevel.h"
#include <GameEngineCore/CoreMinimal.h>
#include "ControlGUI.h"

#include "TestActor.h"
#include "Portal.h"
#include "Effect_DistortionGlitch.h"
#include "Effect_Distortion.h"
#include "Effect_Wave.h"

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
}


void TestLevel::LevelStartEvent()
{
	if (false == ControlGUI::GetInst()->IsUpdate())
	{
		ControlGUI::GetInst()->On();
	}


	GetMainCamera()->GetCameraRenderTarget()->AddEffect<Effect_Wave>();
}

void TestLevel::Update(float _DeltaTime)
{

	if (true == GameEngineInput::GetInst()->IsPress("Shift"))
	{
		portal->On();
	}

}


void TestLevel::End()
{
}
