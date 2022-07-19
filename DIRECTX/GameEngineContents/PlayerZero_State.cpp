#include "PreCompile.h"
#include "PlayerZero.h"

#include "Timer.h"

void PlayerZero::AttackStart()
{
	AttackTimer->Activate();
	Renderer->ChangeFrameAnimation("attack");
}

void PlayerZero::AttackUpdate()
{
	if (true == Attack_AniEnd)
	{
		Attack_AniEnd = false;
		AttackTimer;
		ChangeState(STATE_PLAYER::IDLE);
	}
}

void PlayerZero::FallStart()
{

}

void PlayerZero::FallUpdate()
{
}

void PlayerZero::IdleStart()
{
	Renderer->ChangeFrameAnimation("idle");
}

void PlayerZero::IdleUpdate()
{
	// idle -> run
	// idle -> jump
	// idle -> crouch
	// idle -> attack
	switch (Input)
	{
	case PLAYERINPUT::NONE:
		break;
	case PLAYERINPUT::UP:
		break;
	case PLAYERINPUT::UPRIGHT:
		break;
	case PLAYERINPUT::RIGHT:
		ChangeState(STATE_PLAYER::IDLETORUN);
		break;
	case PLAYERINPUT::RIGHTDOWN:
		ChangeState(STATE_PLAYER::ROLL);
		break;
	case PLAYERINPUT::DOWN:
		break;
	case PLAYERINPUT::LEFTDOWN:
		ChangeState(STATE_PLAYER::ROLL);
		break;
	case PLAYERINPUT::LEFT:
		ChangeState(STATE_PLAYER::IDLETORUN);
		break;
	case PLAYERINPUT::UPLEFT:
		break;
	case PLAYERINPUT::CLICK:
		ChangeState(STATE_PLAYER::ATTACK);
		break;
	default:
		break;
	}
}

void PlayerZero::JumpStart()
{
	Renderer->ChangeFrameAnimation("jump");
}

void PlayerZero::JumpUpdate()
{

}

void PlayerZero::RollStart()
{
	RollTimer->Activate();
	Renderer->ChangeFrameAnimation("roll");
}

void PlayerZero::RollUpdate()
{
	if (true == Roll_AniEnd)
	{
		Roll_AniEnd = false;
		ChangeState(STATE_PLAYER::IDLE);
	}
}

void PlayerZero::RunStart()
{
	Renderer->ChangeFrameAnimation("run");
}

void PlayerZero::RunUpdate()
{
	if (PLAYERINPUT::NONE == Input)
	{
		ChangeState(STATE_PLAYER::RUNTOIDLE);
	}

	if (PLAYERINPUT::LEFTDOWN == Input || PLAYERINPUT::RIGHTDOWN == Input)
	{
		ChangeState(STATE_PLAYER::ROLL);
	}

}

void PlayerZero::WallSlideStart()
{
}

void PlayerZero::WallSlideUpdate()
{
}

void PlayerZero::CrouchStart()
{
	Renderer->ChangeFrameAnimation("precrouch");
}

void PlayerZero::CrouchUpdate()
{

}

void PlayerZero::RunToIdleStart()
{
	Renderer->ChangeFrameAnimation("run_to_idle");
}

void PlayerZero::RunToIdleUpdate()
{
	if (true == RunIdle_AniEnd)
	{
		ChangeState(STATE_PLAYER::IDLE);
	}
}

void PlayerZero::IdleToRunStart()
{
	Renderer->ChangeFrameAnimation("idle_to_run");
}

void PlayerZero::IdleToRunUpdate()
{
	if (true == IdleRun_AniEnd)
	{
		ChangeState(STATE_PLAYER::RUN);
	}
}
