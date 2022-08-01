#include "PreCompile.h"
#include "PlayerZero.h"

#include "Timer.h"
#include "Cursor.h"

const float4 Gravity = { 0, -9.8f, 0 };

PlayerZero::PlayerZero()
	: AttackAble(true)
	, RollAble(true)
	, InputDir(float4::ZERO)
	, MouseDir(float4::ZERO)
{
	InitSpeed = SPEED_PLAYER;
	MoveSpeed = InitSpeed;
}

PlayerZero::~PlayerZero()
{
}

void PlayerZero::Start()
{
	// 렌더러
	Renderer_Character = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Slash = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Slash->GetTransform().SetLocalPosition({ 0, 0, -5 });
	CreateAllAnimation();

	Renderer_Character->SetTexture("spr_idle_0.png");
	Renderer_Character->ScaleToTexture();
	Renderer_Slash->SetTexture("spr_slash_0.png");
	Renderer_Slash->ScaleToTexture();

	Renderer_Character->ChangeFrameAnimation("idle");
	Renderer_Slash->ChangeFrameAnimation("slash");
	GetTransform().SetLocalScale({ 2, 2, 1 });

	// 쿨타임 설정
	AttackTimer = CreateComponent<Timer>();
	AttackTimer->Init(0.36f);
	RollTimer = CreateComponent<Timer>();
	RollTimer->Init(0.4f);

	

	// PlayerStateManager
	PlayerStateManager.CreateStateMember("Idle", this, &PlayerZero::IdleUpdate, &PlayerZero::IdleStart);
	PlayerStateManager.CreateStateMember("Jump", this, &PlayerZero::JumpUpdate, &PlayerZero::JumpStart);
	PlayerStateManager.CreateStateMember("Fall", this, &PlayerZero::FallUpdate, &PlayerZero::FallStart);
	PlayerStateManager.CreateStateMember("Attack", this, &PlayerZero::AttackUpdate, &PlayerZero::AttackStart);
	PlayerStateManager.CreateStateMember("Roll", this, &PlayerZero::RollUpdate, &PlayerZero::RollStart);
	PlayerStateManager.CreateStateMember("Run", this, &PlayerZero::RunUpdate, &PlayerZero::RunStart);
	PlayerStateManager.CreateStateMember("RunToIdle", this, &PlayerZero::RunToIdleUpdate, &PlayerZero::RunToIdleStart);
	PlayerStateManager.CreateStateMember("IdleToRun", this, &PlayerZero::IdleToRunUpdate, &PlayerZero::IdleToRunStart);
	PlayerStateManager.CreateStateMember("WallGrab", this, &PlayerZero::WallGrabUpdate, &PlayerZero::WallGrabStart);
	PlayerStateManager.CreateStateMember("WallSlide", this, &PlayerZero::WallSlideUpdate, &PlayerZero::WallSlideStart);

	PlayerStateManager.ChangeState("Idle");
}

void PlayerZero::Update(float _DeltaTime)
{
	if (_DeltaTime >= 0.1f)
	{
		_DeltaTime = 0.1f;
	}

	// 프리카메라 모드면 움직임 X
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	PixelCheck();
	WallCheck();
	InputCheck();	
	PlayerStateManager.Update(_DeltaTime);
	FloatTimeCheck(_DeltaTime);

	// 이동
	LookCheck(InputDir.x);
	PlayerMove(_DeltaTime);

	CoolTimeCheck();
	PrintPlayerDebug();
}

void PlayerZero::End()
{
}

void PlayerZero::InputCheck()
{
	InputDir = float4::ZERO;
	// CLICK
	if (GameEngineInput::GetInst()->IsDown("MouseLeft") && AttackAble)
	{
		PlayerStateManager.ChangeState("Attack");
	}

	// WASD
	if (GameEngineInput::GetInst()->IsPress("w"))
	{
		InputDir[1] += 1;
	}
	if (GameEngineInput::GetInst()->IsPress("a"))
	{
		InputDir[0] += -1;
	}
	if (GameEngineInput::GetInst()->IsPress("s"))
	{
		InputDir[1] += -1;
	}
	if (GameEngineInput::GetInst()->IsPress("d"))
	{
		InputDir[0] += 1;
	}
}

void PlayerZero::PlayerMove(float _DeltaTime)
{	
	Velocity = MoveVec * MoveSpeed * _DeltaTime;

	// 벽 막기
	switch (WallState)
	{
	case LiveActor::STATE_WALL::NONE:
		break;
	case LiveActor::STATE_WALL::RIGHT:
	{
		Velocity.x = -1;
		if (IsFall && InputDir.x > 0)
		{
			WallGrabCheck();
		}
		break;
	}
	case LiveActor::STATE_WALL::LEFT:
	{
		Velocity.x = 1;
		if (IsFall && InputDir.x > 0)
		{
			WallGrabCheck();
		}
		break;
	}		
	case LiveActor::STATE_WALL::UP:
		Velocity.y = -1;
		break;
	case LiveActor::STATE_WALL::DOWN:
		break;
	case LiveActor::STATE_WALL::UNDERGROUND:
		GetTransform().SetWorldMove({ 0, 1.0f, 0 });
		break;
	case LiveActor::STATE_WALL::RIGHTSLOPE:
	{
		if (Velocity.y > 0)
		{
			break;
		}
		// 우상향
		if (Velocity.x > 0 && CurLookDir > 0)
		{
			GetTransform().SetWorldMove({ 0.3f, 0.3f, 0 });
			return;
		}
		// 좌하향
		else if(Velocity.x < 0 && CurLookDir < 0)
		{
			GetTransform().SetWorldMove({ -0.3f, -0.3f, 0 });
			return;
		}
		break;
	}
	case LiveActor::STATE_WALL::LEFTSLOPE:
	{
		if (Velocity.y > 0)
		{
			break;
		}
		// 우하향
		if (Velocity.x > 0 && CurLookDir > 0)
		{
			GetTransform().SetWorldMove({ 0.3f, -0.3f, 0 });
			return;
		}
		// 좌상향
		else if(Velocity.x < 0 && CurLookDir < 0)
		{
			GetTransform().SetWorldMove({ -0.3f, 0.3f, 0 });
			return;
		}
		break;
	}
	default:
		break;
	}


	GetTransform().SetWorldMove(Velocity);


}

void PlayerZero::CoolTimeCheck()
{
	AttackAble = !AttackTimer->IsCoolTime();
	RollAble = !RollTimer->IsCoolTime();
}


void PlayerZero::CreateAllAnimation()
{
	// Player
	Renderer_Character->CreateFrameAnimationFolder("attack", FrameAnimation_DESC{ "attack", 0.05f , false});
	Renderer_Character->CreateFrameAnimationFolder("fall", FrameAnimation_DESC{ "fall", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "idle", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("idle_to_run", FrameAnimation_DESC{ "idle_to_run", 0.05f , false});
	Renderer_Character->CreateFrameAnimationFolder("jump", FrameAnimation_DESC{ "jump", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("postcrouch", FrameAnimation_DESC{ "postcrouch", 0.08f , false});
	Renderer_Character->CreateFrameAnimationFolder("precrouch", FrameAnimation_DESC{ "precrouch", 0.08f , false});
	Renderer_Character->CreateFrameAnimationFolder("roll", FrameAnimation_DESC{ "roll", 0.05f, false });
	Renderer_Character->CreateFrameAnimationFolder("run", FrameAnimation_DESC{ "run", 0.08f });
	Renderer_Character->CreateFrameAnimationFolder("run_to_idle", FrameAnimation_DESC{ "run_to_idle", 0.05f , false});
	Renderer_Character->CreateFrameAnimationFolder("wallgrab", FrameAnimation_DESC{ "wallgrab", 0.08f, false });
	Renderer_Character->CreateFrameAnimationFolder("wallslide", FrameAnimation_DESC{ "wallslide", 0.08f, false });

	// Player - ANIMATION BLEND
	Renderer_Character->AnimationBindStart("idle_to_run", &PlayerZero::IdleRunStart, this);
	Renderer_Character->AnimationBindFrame("idle_to_run", &PlayerZero::StopIdleToRun, this);
	Renderer_Character->AnimationBindEnd("idle_to_run", &PlayerZero::IdleRunEnd, this);
	Renderer_Character->AnimationBindStart("run_to_idle", &PlayerZero::RunidleStart, this);
	Renderer_Character->AnimationBindEnd("run_to_idle", &PlayerZero::RunidleEnd, this);
	Renderer_Character->AnimationBindStart("roll", &PlayerZero::RollStart, this);
	Renderer_Character->AnimationBindEnd("roll", &PlayerZero::RollEnd, this);
	Renderer_Character->AnimationBindStart("attack", &PlayerZero::AttackStart, this);
	Renderer_Character->AnimationBindEnd("attack", &PlayerZero::AttackEnd, this);
	//Renderer_Character->AnimationBindStart("idle_to_run", [this](const FrameAnimation_DESC&) { IdleRun_AniEnd = false; });
	//Renderer_Character->AnimationBindFrame("idle_to_run", [this](const FrameAnimation_DESC&) {  if (InputDir.CompareInt2D({0, 0})) { ChangeState(STATE_PLAYER::IDLE); }});
	//Renderer_Character->AnimationBindEnd("idle_to_run", [this](const FrameAnimation_DESC&) { IdleRun_AniEnd = true; });
	//Renderer_Character->AnimationBindStart("run_to_idle", [this](const FrameAnimation_DESC&) { RunIdle_AniEnd = false; });
	//Renderer_Character->AnimationBindEnd("run_to_idle", [this](const FrameAnimation_DESC&) { RunIdle_AniEnd = true; });
	//Renderer_Character->AnimationBindStart("roll", [this](const FrameAnimation_DESC&) { Roll_AniEnd = false; });
	//Renderer_Character->AnimationBindEnd("roll", [this](const FrameAnimation_DESC&) { Roll_AniEnd = true; });
	//Renderer_Character->AnimationBindStart("attack", [this](const FrameAnimation_DESC&) { Attack_AniEnd = false; });
	//Renderer_Character->AnimationBindEnd("attack", [this](const FrameAnimation_DESC&) { Attack_AniEnd = true; });

	// Slash
	Renderer_Slash->CreateFrameAnimationFolder("slash", FrameAnimation_DESC{ "player_slash", 0.07f, false });
	Renderer_Slash->Off();
}

