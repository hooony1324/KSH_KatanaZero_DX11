#include "PreCompile.h"
#include "PlayerZero.h"

#include "Timer.h"
#include "Cursor.h"

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

	Renderer_Character = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Slash = CreateComponent<GameEngineTextureRenderer>();

	// DEPTH
	//Renderer_Character->GetTransform().SetLocalPosition({ 0, 0, GetDepth(ACTOR_DEPTH::PLAYER) });
	Renderer_Slash->GetTransform().SetLocalPosition({ 0, 0, -5 });

	CreateAllAnimation();

	Renderer_Character->SetTexture("spr_idle_0.png");
	Renderer_Character->ScaleToTexture();
	Renderer_Slash->SetTexture("spr_slash_0.png");
	Renderer_Slash->ScaleToTexture();

	Renderer_Character->ChangeFrameAnimation("idle");
	Renderer_Slash->ChangeFrameAnimation("slash");

	// ÄðÅ¸ÀÓ ¼³Á¤
	AttackTimer = CreateComponent<Timer>();
	AttackTimer->Init(0.35f);
	RollTimer = CreateComponent<Timer>();
	RollTimer->Init(0.4f);

	GetTransform().SetLocalScale({ 2, 2, 1 });
	ChangeState(STATE_PLAYER::IDLE);

}

void PlayerZero::Update(float _DeltaTime)
{
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	InputCheck();
	// FallCheck(); ->ChangeState::FALL
	UpdateState();
	CoolTimeCheck();

	PixelCheck();
	VelocityCheck(_DeltaTime);
	
	GetTransform().SetWorldMove(Velocity);


}

void PlayerZero::End()
{
}

void PlayerZero::InputCheck()
{
	InputDir = float4::ZERO;
	// CLICK
	if (GameEngineInput::GetInst()->IsDown("MouseLeft"))
	{
		ChangeState(STATE_PLAYER::ATTACK);
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


	//MoveDir = InputDir;
}

void PlayerZero::CoolTimeCheck()
{
	AttackAble = !AttackTimer->IsCoolTime();
	RollAble = !RollTimer->IsCoolTime();
}

void PlayerZero::UpdateState()
{
	switch (PlayerState)
	{
	case STATE_PLAYER::ATTACK:
		AttackUpdate();
		break;
	case STATE_PLAYER::FALL:
		FallUpdate();
		break;
	case STATE_PLAYER::IDLE:
		IdleUpdate();
		break;
	case STATE_PLAYER::JUMP:
		JumpUpdate();
		break;
	case STATE_PLAYER::ROLL:
		RollUpdate();
		break;
	case STATE_PLAYER::RUN:
		RunUpdate();
		break;
	case STATE_PLAYER::WALLSLIDE:
		WallSlideUpdate();
		break;
	case STATE_PLAYER::CROUCH:
		CrouchUpdate();
		break;
	case STATE_PLAYER::IDLETORUN:
		IdleToRunUpdate();
		break;
	case STATE_PLAYER::RUNTOIDLE:
		RunToIdleUpdate();
		break;
	default:
		break;
	}

}

void PlayerZero::ChangeState(STATE_PLAYER _PlayerState)
{
	if (PlayerState != _PlayerState)
	{
		switch (_PlayerState)
		{
		case STATE_PLAYER::ATTACK:
		{
			if (false == AttackAble)
			{
				return;
			}
			AttackStart();
			break;
		}
		case STATE_PLAYER::FALL:
			FallStart();
			break;
		case STATE_PLAYER::IDLE:
			IdleStart();
			break;
		case STATE_PLAYER::JUMP:
			JumpStart();
			break;
		case STATE_PLAYER::ROLL:
		{
			if (false == RollAble)
			{
				return;
			}
			RollStart();
			break;
		}
		case STATE_PLAYER::RUN:
			RunStart();
			break;
		case STATE_PLAYER::WALLSLIDE:
			WallSlideStart();
			break;
		case STATE_PLAYER::CROUCH:
			CrouchStart();
			break;
		case STATE_PLAYER::IDLETORUN:
			IdleToRunStart();
			break;
		case STATE_PLAYER::RUNTOIDLE:
			RunToIdleStart();
			break;
		default:
			break;
		}
	}

	PlayerState = _PlayerState;
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
	Renderer_Character->CreateFrameAnimationFolder("wallgrab", FrameAnimation_DESC{ "wallgrab", 0.08f });
	Renderer_Character->CreateFrameAnimationFolder("wallslide", FrameAnimation_DESC{ "wallslide", 0.08f });

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
	Renderer_Slash->CreateFrameAnimationFolder("slash", FrameAnimation_DESC{ "player_slash", 0.09f, false });
	Renderer_Slash->Off();
}

