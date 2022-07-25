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
	PlayerSpeed *= 0.99f;

	if (true == Attack_AniEnd)
	{
		PlayerSpeed = SPEED_PLAYER;
		MouseDir = float4::ZERO;
		MoveDir = float4::ZERO;
		Attack_AniEnd = false;
		Renderer_Slash->Off();
		ChangeState(STATE_PLAYER::IDLE);
	}
}

void PlayerZero::FallStart()
{

}

void PlayerZero::FallUpdate()
{
	MoveDir = { 0, -1 };
}

void PlayerZero::IdleStart()
{
	Renderer_Character->ChangeFrameAnimation("idle");
}

void PlayerZero::IdleUpdate()
{

	if (abs(InputDir.x) > 0)
	{
		MoveDir.x = InputDir.x;
		ChangeState(STATE_PLAYER::IDLETORUN);
	}


}

void PlayerZero::JumpStart()
{
	Renderer_Character->ChangeFrameAnimation("jump");
}

void PlayerZero::JumpUpdate()
{

}

void PlayerZero::RollStart()
{
	RollTimer->Activate();
	Renderer_Character->ChangeFrameAnimation("roll");
	PlayerSpeed = SPEED_PLAYER * 2.0f;
}

void PlayerZero::RollUpdate()
{
	if (true == Roll_AniEnd)
	{
		Roll_AniEnd = false;
		PlayerSpeed = SPEED_PLAYER;

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
	PlayerSpeed = SPEED_PLAYER;
	Renderer_Character->ChangeFrameAnimation("run");
}

void PlayerZero::RunUpdate()
{
	if (InputDir.CompareInt2D(float4::ZERO))
	{
		MoveDir = float4::ZERO;
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
	PlayerSpeed = 20.0f;
	Renderer_Character->ChangeFrameAnimation("idle_to_run");
}

void PlayerZero::IdleToRunUpdate()
{
	if (true == IdleRun_AniEnd)
	{
		ChangeState(STATE_PLAYER::RUN);
	}
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
	MoveDir = MouseDir;

	float4 Rot = float4::VectorXYtoDegree(PlayerPos, MousePos);
	Renderer_Slash->GetTransform().SetWorldRotation({ 0, 0, Rot.z });
	LookDir.x = MoveDir.x;

	PlayerSpeed = 2000.0f;

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


