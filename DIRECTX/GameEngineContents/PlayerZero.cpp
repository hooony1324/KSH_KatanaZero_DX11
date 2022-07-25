#include "PreCompile.h"
#include "PlayerZero.h"

#include "Timer.h"
#include "Cursor.h"

PlayerZero::PlayerZero()
	: Renderer_Player(nullptr)
	, AttackAble(true)
	, RollAble(true)
	, PlayerSpeed(SPEED_PLAYER)
	, InputDir(float4::ZERO)
	, MouseDir(float4::ZERO)
	, MoveDir(float4::ZERO)
	, MoveForce(0.0f)
{
}

PlayerZero::~PlayerZero()
{
}

void PlayerZero::Start()
{

	Renderer_Player = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Slash = CreateComponent<GameEngineTextureRenderer>();

	CreateAllAnimation();

	Renderer_Player->SetTexture("spr_idle_0.png");
	Renderer_Player->ScaleToTexture();
	Renderer_Player->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(DEPTH_ACTOR::PLAYER) });
	Renderer_Slash->SetTexture("spr_slash_0.png");
	Renderer_Slash->ScaleToTexture();
	Renderer_Slash->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(DEPTH_ACTOR::FX) });

	Renderer_Player->ChangeFrameAnimation("idle");
	Renderer_Slash->ChangeFrameAnimation("slash");

	// ÄðÅ¸ÀÓ ¼³Á¤
	AttackTimer = CreateComponent<Timer>();
	AttackTimer->Init(0.35f);
	RollTimer = CreateComponent<Timer>();
	RollTimer->Init(0.6f);

	GetTransform().SetLocalScale({ 1.5f, 1.5f, 1 });
	ChangeState(STATE_PLAYER::IDLE);

}

void PlayerZero::Update(float _DeltaTime)
{
	InputCheck();
	// FallCheck(); ->ChangeState::FALL
	UpdateState();
	CoolTimeCheck();

	GetTransform().SetWorldMove(MoveDir * _DeltaTime * PlayerSpeed);
}

void PlayerZero::End()
{
}

void PlayerZero::InputCheck()
{
	MoveDir = float4::ZERO;
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
		Renderer_Player->GetTransform().PixLocalNegativeX();
		InputDir[0] += -1;
	}
	if (GameEngineInput::GetInst()->IsPress("s"))
	{
		InputDir[1] += -1;
	}
	if (GameEngineInput::GetInst()->IsPress("d"))
	{
		Renderer_Player->GetTransform().PixLocalPositiveX();
		InputDir[0] += 1;
	}

	// FALL Check
	//if (fall)
	/*{
	* asdfasdfasdf
		return;
	}*/
	

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
	Renderer_Player->CreateFrameAnimationFolder("attack", FrameAnimation_DESC{ "attack", 0.05f , false});
	Renderer_Player->CreateFrameAnimationFolder("fall", FrameAnimation_DESC{ "fall", 0.1125f });
	Renderer_Player->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "idle", 0.1125f });
	Renderer_Player->CreateFrameAnimationFolder("idle_to_run", FrameAnimation_DESC{ "idle_to_run", 0.05f , false});
	Renderer_Player->CreateFrameAnimationFolder("jump", FrameAnimation_DESC{ "jump", 0.1125f });
	Renderer_Player->CreateFrameAnimationFolder("postcrouch", FrameAnimation_DESC{ "postcrouch", 0.08f , false});
	Renderer_Player->CreateFrameAnimationFolder("precrouch", FrameAnimation_DESC{ "precrouch", 0.08f , false});
	Renderer_Player->CreateFrameAnimationFolder("roll", FrameAnimation_DESC{ "roll", 0.05f, false });
	Renderer_Player->CreateFrameAnimationFolder("run", FrameAnimation_DESC{ "run", 0.08f });
	Renderer_Player->CreateFrameAnimationFolder("run_to_idle", FrameAnimation_DESC{ "run_to_idle", 0.05f , false});
	Renderer_Player->CreateFrameAnimationFolder("wallgrab", FrameAnimation_DESC{ "wallgrab", 0.08f });
	Renderer_Player->CreateFrameAnimationFolder("wallslide", FrameAnimation_DESC{ "wallslide", 0.08f });

	// Player - ANIMATION BLEND
	Renderer_Player->AnimationBindStart("idle_to_run", [this](const FrameAnimation_DESC&) { IdleRun_AniEnd = false; });
	Renderer_Player->AnimationBindFrame("idle_to_run", [this](const FrameAnimation_DESC&) {  if (InputDir.CompareInt2D({0, 0})) { ChangeState(STATE_PLAYER::IDLE); }});
	Renderer_Player->AnimationBindEnd("idle_to_run", [this](const FrameAnimation_DESC&) { IdleRun_AniEnd = true; });
	Renderer_Player->AnimationBindStart("run_to_idle", [this](const FrameAnimation_DESC&) { RunIdle_AniEnd = false; });
	Renderer_Player->AnimationBindEnd("run_to_idle", [this](const FrameAnimation_DESC&) { RunIdle_AniEnd = true; });
	Renderer_Player->AnimationBindStart("roll", [this](const FrameAnimation_DESC&) { Roll_AniEnd = false; });
	Renderer_Player->AnimationBindEnd("roll", [this](const FrameAnimation_DESC&) { Roll_AniEnd = true; });
	Renderer_Player->AnimationBindStart("attack", [this](const FrameAnimation_DESC&) { Attack_AniEnd = false; });
	Renderer_Player->AnimationBindEnd("attack", [this](const FrameAnimation_DESC&) { Attack_AniEnd = true; });

	// Slash
	Renderer_Slash->CreateFrameAnimationFolder("slash", FrameAnimation_DESC{ "player_slash", 0.09f, false });
	Renderer_Slash->Off();
}

void PlayerZero::CreateSlash()
{
	float4 PlayerPos = GetTransform().GetWorldPosition();

	Renderer_Slash->On();
	Renderer_Slash->CurAnimationReset();
	MousePos = Cursor::GetCursorPosition();
	MousePos.z = 0;

	MouseDir = MousePos - PlayerPos;
	float4 Rot = float4::VectorXYtoDegree(PlayerPos, MousePos);
	Renderer_Slash->GetTransform().SetWorldRotation({0, 0, Rot.z });
}