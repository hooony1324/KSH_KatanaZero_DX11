#include "PreCompile.h"
#include "PlayerZero.h"

#include "Timer.h"

PlayerZero::PlayerZero()
	: Renderer(nullptr)
	, AttackAble(true)
	, RollAble(true)
{
}

PlayerZero::~PlayerZero() 
{
}

void PlayerZero::CreateKey()
{
	if (GameEngineInput::GetInst()->IsKey("W"))
	{
		return;
	}

	GameEngineInput::GetInst()->CreateKey("W", 'W');
	GameEngineInput::GetInst()->CreateKey("A", 'A');
	GameEngineInput::GetInst()->CreateKey("S", 'S');
	GameEngineInput::GetInst()->CreateKey("D", 'D');
	GameEngineInput::GetInst()->CreateKey("SpaceBar", VK_SPACE);
}

void PlayerZero::Start()
{
	CreateKey();

	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->GetTransform().SetLocalScale({ 100, 100, 0 });
	Renderer->SetTexture("spr_walk_0.png");

	CreateAllAnimation();
	Renderer->ChangeFrameAnimation("idle");

	AttackTimer = CreateComponent<Timer>();
	AttackTimer->Init(0.5f);
	RollTimer = CreateComponent<Timer>();
	RollTimer->Init(0.8f);

	ChangeState(STATE_PLAYER::IDLE);
}

void PlayerZero::Update(float _DeltaTime)
{
 	InputCheck();
	CoolTimeCheck();
	UpdateState();

	GetTransform().SetWorldMove(MoveDir);
}

void PlayerZero::End()
{
}

void PlayerZero::InputCheck()
{
	Input = PLAYERINPUT::NONE;
	
	// CLICK
	if (GameEngineInput::GetInst()->IsPress("SpaceBar") && true == AttackAble)
	{
		Input = PLAYERINPUT::CLICK;
		ChangeState(STATE_PLAYER::ATTACK);
	}

	// WASD
	int InputDir[2] = {0, 0};
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

	if (InputDir[0] == 0 && InputDir[1] == 1)
	{
		Input = PLAYERINPUT::UP;
	}
	if (InputDir[0] == 1 && InputDir[1] == 1)
	{
		Input = PLAYERINPUT::UPRIGHT;
	}
	if (InputDir[0] == 1 && InputDir[1] == 0)
	{
		Input = PLAYERINPUT::RIGHT;
	}
	if (InputDir[0] == 1 && InputDir[1] == -1)
	{
		Input = PLAYERINPUT::RIGHTDOWN;
	}
	if (InputDir[0] == 0 && InputDir[1] == -1)
	{
		Input = PLAYERINPUT::DOWN;
	}
	if (InputDir[0] == -1 && InputDir[1] == -1)
	{
		Input = PLAYERINPUT::LEFTDOWN;
	}
	if (InputDir[0] == -1 && InputDir[1] == 0)
	{
		Input = PLAYERINPUT::LEFT;
	}
	if (InputDir[0] == -1 && InputDir[1] == 1)
	{
		Input = PLAYERINPUT::UPLEFT;
	}



}

void PlayerZero::CoolTimeCheck()
{
	AttackAble = AttackTimer->IsEnd();
	RollAble = RollTimer->IsEnd();
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
			AttackStart();
			break;
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
	Renderer->CreateFrameAnimationFolder("attack", FrameAnimation_DESC{ "attack", 0.05f , false});
	Renderer->CreateFrameAnimationFolder("fall", FrameAnimation_DESC{ "fall", 0.1125f });
	Renderer->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "idle", 0.1125f });
	Renderer->CreateFrameAnimationFolder("idle_to_run", FrameAnimation_DESC{ "idle_to_run", 0.05f , false});
	Renderer->CreateFrameAnimationFolder("jump", FrameAnimation_DESC{ "jump", 0.1125f });
	Renderer->CreateFrameAnimationFolder("postcrouch", FrameAnimation_DESC{ "postcrouch", 0.08f , false});
	Renderer->CreateFrameAnimationFolder("precrouch", FrameAnimation_DESC{ "precrouch", 0.08f , false});
	Renderer->CreateFrameAnimationFolder("roll", FrameAnimation_DESC{ "roll", 0.05f , false});
	Renderer->CreateFrameAnimationFolder("run", FrameAnimation_DESC{ "run", 0.08f });
	Renderer->CreateFrameAnimationFolder("run_to_idle", FrameAnimation_DESC{ "run_to_idle", 0.05f , false});
	Renderer->CreateFrameAnimationFolder("wallgrab", FrameAnimation_DESC{ "wallgrab", 0.08f });
	Renderer->CreateFrameAnimationFolder("wallslide", FrameAnimation_DESC{ "wallslide", 0.08f });
	
	// ANIMATION BLEND
	Renderer->AnimationBindStart("idle_to_run", [this](const FrameAnimation_DESC&) { IdleRun_AniEnd = false; });
	Renderer->AnimationBindFrame("idle_to_run", [this](const FrameAnimation_DESC&) {  if (Input == PLAYERINPUT::NONE) { ChangeState(STATE_PLAYER::IDLE); }} );
	Renderer->AnimationBindEnd("idle_to_run", [this](const FrameAnimation_DESC&) { IdleRun_AniEnd = true; });
	Renderer->AnimationBindStart("run_to_idle", [this](const FrameAnimation_DESC&) { RunIdle_AniEnd = false; });
	Renderer->AnimationBindEnd("run_to_idle", [this](const FrameAnimation_DESC&) { RunIdle_AniEnd = true; });
	Renderer->AnimationBindStart("roll", [this](const FrameAnimation_DESC&) { Roll_AniEnd = false; });
	Renderer->AnimationBindEnd("roll", [this](const FrameAnimation_DESC&) { Roll_AniEnd = true; });
	Renderer->AnimationBindStart("attack", [this](const FrameAnimation_DESC&) { Attack_AniEnd = false; });
	Renderer->AnimationBindEnd("attack", [this](const FrameAnimation_DESC&) { Attack_AniEnd = true; });

}

