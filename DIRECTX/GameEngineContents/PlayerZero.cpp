#include "PreCompile.h"
#include "PlayerZero.h"

#include "Timer.h"

const float FORCE_LIMIT = 200.0f;
const float FORCE_INPUT = 100.0f;

PlayerZero::PlayerZero()
	: Renderer(nullptr)
	, AttackAble(true)
	, RollAble(true)
	, PlayerSpeed(400.0f)
{
}

PlayerZero::~PlayerZero()
{
}

void PlayerZero::Start()
{

	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_idle_0.png");
	Renderer->ScaleToTexture();
	CreateAllAnimation();
	Renderer->ChangeFrameAnimation("idle");
	GetTransform().SetLocalScale({ 2, 2, 1.0f });

	// 쿨타임 설정
	AttackTimer = CreateComponent<Timer>();
	AttackTimer->Init(0.4f);
	RollTimer = CreateComponent<Timer>();
	RollTimer->Init(0.6f);

	ChangeState(STATE_PLAYER::IDLE);
}

void PlayerZero::Update(float _DeltaTime)
{
	InputCheck(_DeltaTime);
	UpdateState();
	CoolTimeCheck();

	GetTransform().SetWorldMove(MoveDir * _DeltaTime * InputForce * PlayerSpeed);
}

void PlayerZero::End()
{
}

void PlayerZero::InputCheck(float _DeltaTime)
{

	// CLICK
	if (GameEngineInput::GetInst()->IsPress("SpaceBar"))
	{
		ChangeState(STATE_PLAYER::ATTACK);
	}

	// WASD
	InputDir = float4::ZERO;
	if (GameEngineInput::GetInst()->IsPress("w"))
	{
		InputDir[1] += 1;
	}
	if (GameEngineInput::GetInst()->IsPress("a"))
	{
		Renderer->GetTransform().PixLocalNegativeX();
		InputDir[0] += -1;
	}
	if (GameEngineInput::GetInst()->IsPress("s"))
	{
		InputDir[1] += -1;
	}
	if (GameEngineInput::GetInst()->IsPress("d"))
	{
		Renderer->GetTransform().PixLocalPositiveX();
		InputDir[0] += 1;
	}

	// Input정도 : 0 ~ 1
	if (abs(InputDir.x) + abs(InputDir.y) <= 0)
	{
		InputForce = 0.0f;
		InputTime = 0.0f;
	}
	else
	{
		InputTime += _DeltaTime * 2;
		InputForce = InputTime * InputTime + 0.2f;
		if (InputForce >= 1.0f)
		{
			InputForce = 1.0f;
		}
	}


	// 벽 체크?

	MoveDir = InputDir;
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
	Renderer->CreateFrameAnimationFolder("attack", FrameAnimation_DESC{ "attack", 0.06f , false});
	Renderer->CreateFrameAnimationFolder("fall", FrameAnimation_DESC{ "fall", 0.1125f });
	Renderer->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "idle", 0.1125f });
	Renderer->CreateFrameAnimationFolder("idle_to_run", FrameAnimation_DESC{ "idle_to_run", 0.05f , false});
	Renderer->CreateFrameAnimationFolder("jump", FrameAnimation_DESC{ "jump", 0.1125f });
	Renderer->CreateFrameAnimationFolder("postcrouch", FrameAnimation_DESC{ "postcrouch", 0.08f , false});
	Renderer->CreateFrameAnimationFolder("precrouch", FrameAnimation_DESC{ "precrouch", 0.08f , false});
	Renderer->CreateFrameAnimationFolder("roll", FrameAnimation_DESC{ "roll", 0.05f, false });
	Renderer->CreateFrameAnimationFolder("run", FrameAnimation_DESC{ "run", 0.08f });
	Renderer->CreateFrameAnimationFolder("run_to_idle", FrameAnimation_DESC{ "run_to_idle", 0.05f , false});
	Renderer->CreateFrameAnimationFolder("wallgrab", FrameAnimation_DESC{ "wallgrab", 0.08f });
	Renderer->CreateFrameAnimationFolder("wallslide", FrameAnimation_DESC{ "wallslide", 0.08f });

	// ANIMATION BLEND
	Renderer->AnimationBindStart("idle_to_run", [this](const FrameAnimation_DESC&) { IdleRun_AniEnd = false; });
	Renderer->AnimationBindFrame("idle_to_run", [this](const FrameAnimation_DESC&) {  if (InputDir.CompareInt2D({0, 0})) { ChangeState(STATE_PLAYER::IDLE); }});
	Renderer->AnimationBindEnd("idle_to_run", [this](const FrameAnimation_DESC&) { IdleRun_AniEnd = true; });
	Renderer->AnimationBindStart("run_to_idle", [this](const FrameAnimation_DESC&) { RunIdle_AniEnd = false; });
	Renderer->AnimationBindEnd("run_to_idle", [this](const FrameAnimation_DESC&) { RunIdle_AniEnd = true; });
	Renderer->AnimationBindStart("roll", [this](const FrameAnimation_DESC&) { Roll_AniEnd = false; });
	Renderer->AnimationBindEnd("roll", [this](const FrameAnimation_DESC&) { Roll_AniEnd = true; });
	Renderer->AnimationBindStart("attack", [this](const FrameAnimation_DESC&) { Attack_AniEnd = false; });
	Renderer->AnimationBindEnd("attack", [this](const FrameAnimation_DESC&) { Attack_AniEnd = true; });

}

