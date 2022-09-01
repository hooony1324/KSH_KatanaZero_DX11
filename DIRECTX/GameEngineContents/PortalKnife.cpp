#include "PreCompile.h"
#include "PortalKnife.h"
#include <GameEngineCore/CoreMinimal.h>

const float SpawnOutDis = 150;
const float SpawnSpeed = 4.2f;
const float DrawInDis = 100;
const float DrawInSpeed = 6.0f;

PortalKnife::PortalKnife() 
	: Renderer(nullptr)
{
}

PortalKnife::~PortalKnife() 
{
}

void PortalKnife::OnEvent()
{
	
}

void PortalKnife::Start()
{
	Renderer = CreateComponent<GameContentsCustomRenderer>();
	Renderer->SetTexture("spr_psychboss_attack_knife_1.png");
	Renderer->SetPivot(PIVOTMODE::RIGHT);
	Renderer->ScaleToTexture();
	Renderer->CreateMaskAnimationFolder("portal", CustomFrameAnimation_DESC{ "portal_cutout", 0.065f, true });
	Renderer->ChangeMaskAnimation("portal");
	Renderer->Option.IsMask = 1;
	RendererScaleX = Renderer->GetTransform().GetLocalScale().x;
	Renderer->Off();
	Renderer->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));

	StateManager.CreateStateMember("Idle"
		, std::bind(&PortalKnife::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2));

	StateManager.CreateStateMember("Spawn"
		, std::bind(&PortalKnife::SpawnUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PortalKnife::SpawnStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Draw"
		, std::bind(&PortalKnife::DrawUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PortalKnife::DrawStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Shoot"
		, std::bind(&PortalKnife::ShootUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PortalKnife::ShootStart, this, std::placeholders::_1));

	MoveSpeed = 1000;
	StateManager.ChangeState("Idle");
}

void PortalKnife::Update(float _DeltaTime)
{
	float GroupDeltaScale = GameEngineTime::GetInst()->GetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP));
	StateManager.Update(_DeltaTime * GroupDeltaScale);
}

void PortalKnife::End()
{

}

void PortalKnife::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime >= WakeTime)
	{
		StateManager.ChangeState("Spawn");
	}
}

void PortalKnife::SpawnStart(const StateInfo& _Info)
{
	Renderer->On();
	StartPos = GetTransform().GetWorldPosition();
	//if (Dir.x < 0.02f && Dir.x > 0)
	//{
	//	Dir.x = 0.02f;
	//}
	DestPos = StartPos + Dir * SpawnOutDis;

}

// 	MaskedRenderer->GetMaskData().MaskFrameData.x -= MoveX/2 / ScaleX;

void PortalKnife::SpawnUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float4 CurPos = GetTransform().GetWorldPosition();
	float4 MovePos = float4::Lerp(CurPos, DestPos, _DeltaTime * 4.2f);

	// 마스크렌더러 위치조정
	if (Renderer->GetMaskData().MaskFrameData.x >= 1.0f)
	{
		Renderer->GetMaskData().MaskFrameData.x = 1.0f;
	}
	else
	{
		float Move = (MovePos - CurPos).Length();
		Renderer->GetMaskData().MaskFrameData.x += Move / RendererScaleX;
	}
	
	GetTransform().SetWorldPosition(MovePos);

	if (_Info.StateTime > 2.0f)
	{
		StateManager.ChangeState("Draw");
	}
}

void PortalKnife::DrawStart(const StateInfo& _Info)
{
	StartPos = GetTransform().GetWorldPosition();
	DestPos = StartPos + (-Dir * DrawInDis);
}

void PortalKnife::DrawUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float4 CurPos = GetTransform().GetWorldPosition();
	float4 MovePos = float4::Lerp(CurPos, DestPos, _DeltaTime * 4.2f);

	// 마스크렌더러 위치조정
	if (Renderer->GetMaskData().MaskFrameData.x <= 0.0f)
	{
		Renderer->GetMaskData().MaskFrameData.x = 0.0f;
	}
	else
	{
		float Move = (MovePos - CurPos).Length();
		Renderer->GetMaskData().MaskFrameData.x -= Move / RendererScaleX;
	}
	
	GetTransform().SetWorldPosition(MovePos);

	if (_Info.StateTime > 0.5f)
	{
		StateManager.ChangeState("Shoot");
	}
}

void PortalKnife::ShootStart(const StateInfo& _Info)
{
}

void PortalKnife::ShootUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float4 CurPos = GetTransform().GetWorldPosition();
	float4 MovePos = CurPos + Dir * MoveSpeed * _DeltaTime;

	if (Renderer->GetMaskData().MaskFrameData.x >= 1.0f)
	{
		Renderer->GetMaskData().MaskFrameData.x = 1.0f;
	}
	else
	{
		float Move = (MovePos - CurPos).Length();
		Renderer->GetMaskData().MaskFrameData.x += Move / RendererScaleX;
	}

	GetTransform().SetWorldPosition(MovePos);


	if (_Info.StateTime > 3.0f)
	{
		Death();
	}
}
