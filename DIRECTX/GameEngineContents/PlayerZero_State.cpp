#include "PreCompile.h"
#include "PlayerZero.h"
#include "Timer.h"
#include "Cursor.h"

void PlayerZero::AttackStart()
{
	CreateSlash();
	AttackTimer->Activate();
	Renderer_Character->ChangeFrameAnimation("attack");


}

void PlayerZero::AttackUpdate()
{
	MoveSpeed *= 0.99f;

	if (true == Attack_AniEnd)
	{
		MoveSpeed = SPEED_PLAYER;
		MouseDir = float4::ZERO;
		MoveDir = float4::ZERO;
		Attack_AniEnd = false;
		Renderer_Slash->Off();
		ChangeState(STATE_PLAYER::IDLE);
	}
}

void PlayerZero::FallStart()
{
	Renderer_Character->ChangeFrameAnimation("fall");
}

void PlayerZero::FallUpdate()
{

	if (false == IsFloat)
	{
		MoveDir = float4::ZERO;
		ChangeState(STATE_PLAYER::IDLE);
	}
}

void PlayerZero::IdleStart()
{
	IsJump = false;
	Renderer_Character->ChangeFrameAnimation("idle");
}

void PlayerZero::IdleUpdate()
{
	if (IsFloat && !IsJump)
	{
		ChangeState(STATE_PLAYER::FALL);
	}

	if (abs(InputDir.x) > 0)
	{
		ChangeState(STATE_PLAYER::IDLETORUN);
	}

}

void PlayerZero::JumpStart()
{
	IsJump = true;
	MoveDir = InputDir;
	JumpDeltaTime = 0.0f;



	Renderer_Character->ChangeFrameAnimation("jump");
}

void PlayerZero::JumpUpdate()
{
	MoveDir.x = InputDir.x;

	MoveDir.y = MoveDir.y - ( JumpDeltaTime * JumpDeltaTime / 25);

	JumpDeltaTime += DeltaTime;
	//포물선 y 성분 0 이면
	if (MoveDir.y <= 0)
	{
		IsJump = false;
		JumpDeltaTime = 0;
		ChangeState(STATE_PLAYER::FALL);
	}

}

void PlayerZero::RollStart()
{
	RollTimer->Activate();
	Renderer_Character->ChangeFrameAnimation("roll");
	MoveSpeed = SPEED_PLAYER * 1.5f;
}

void PlayerZero::RollUpdate()
{
	if (true == Roll_AniEnd)
	{
		Roll_AniEnd = false;
		MoveSpeed = SPEED_PLAYER;

		if (abs(MoveDir.x) > 0)
		{
			ChangeState(STATE_PLAYER::RUN);
		}
		else
		{
			ChangeState(STATE_PLAYER::IDLE);
		}
	}
}

void PlayerZero::RunStart()
{
	MoveDir.x = InputDir.x;
	MoveSpeed = SPEED_PLAYER;
	Renderer_Character->ChangeFrameAnimation("run");
}

void PlayerZero::RunUpdate()
{

	if (InputDir.CompareInt2D(float4::ZERO))
	{
		MoveDir.x = 0;
		ChangeState(STATE_PLAYER::RUNTOIDLE);
	}

	float4 RollDir = float4{ abs(InputDir.x), InputDir.y };
	if (RollDir.CompareInt2D({1, -1}))
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
	Renderer_Character->ChangeFrameAnimation("precrouch");
}

void PlayerZero::CrouchUpdate()
{

}

void PlayerZero::RunToIdleStart()
{
	Renderer_Character->ChangeFrameAnimation("run_to_idle");
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
	MoveSpeed = 20.0f;
	Renderer_Character->ChangeFrameAnimation("idle_to_run");
}

void PlayerZero::IdleToRunUpdate()
{
	if (true == IdleRun_AniEnd)
	{
		ChangeState(STATE_PLAYER::RUN);
	}
}

void PlayerZero::WallPassCheck()
{
	
}

void PlayerZero::CreateSlash()
{
	float4 PlayerPos = GetTransform().GetWorldPosition();
	PlayerPos.z = 0;

	Renderer_Slash->On();
	Renderer_Slash->CurAnimationReset();
	MousePos = Cursor::GetCursorPosition();
	MousePos.z = 0;

	MouseDir = MousePos - PlayerPos;
	MouseDir.z = 0;
	MouseDir.Normalize();
	InputDir.x = MouseDir.x;
	MoveDir = MouseDir;

	float4 Rot = float4::VectorXYtoDegree(PlayerPos, MousePos);
	Renderer_Slash->GetTransform().SetWorldRotation({ 0, 0, Rot.z });
	

	MoveSpeed = 2000.0f;

	// Sound
	std::string Sound = "sound_player_slash_";
	static int SoundIdx = 1;
	Sound = Sound + std::to_string(SoundIdx) + ".wav";

	GameEngineSoundPlayer SoundPlayer = GameEngineSound::SoundPlayControl(Sound);
	SoundPlayer.Volume(0.5f);

	if (++SoundIdx > 3)
	{
		SoundIdx = 1;
	}
}



void PlayerZero::PrintPlayerDebug()
{
	switch (PlayerState)
	{
	case STATE_PLAYER::NONE:
		GameEngineDebug::OutPutString("NONE");
		break;
	case STATE_PLAYER::ATTACK:
		GameEngineDebug::OutPutString("ATTACK");
		break;
	case STATE_PLAYER::FALL:
		GameEngineDebug::OutPutString("FALL");
		break;
	case STATE_PLAYER::IDLE:
		GameEngineDebug::OutPutString("IDLE");
		break;
	case STATE_PLAYER::JUMP:
		GameEngineDebug::OutPutString("JUMP");
		break;
	case STATE_PLAYER::ROLL:
		GameEngineDebug::OutPutString("ROLL");
		break;
	case STATE_PLAYER::RUN:
		GameEngineDebug::OutPutString("RUN");
		break;
	case STATE_PLAYER::WALLSLIDE:
		GameEngineDebug::OutPutString("WALLSLIDE");
		break;
	case STATE_PLAYER::CROUCH:
		GameEngineDebug::OutPutString("CROUCH");
		break;
	case STATE_PLAYER::RUNTOIDLE:
		GameEngineDebug::OutPutString("RUNTOIDLE");
		break;
	case STATE_PLAYER::IDLETORUN:
		GameEngineDebug::OutPutString("IDLETORUN");
		break;
	default:
		break;
	}
}
