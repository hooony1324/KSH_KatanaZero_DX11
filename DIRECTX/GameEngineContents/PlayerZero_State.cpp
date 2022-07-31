#include "PreCompile.h"
#include "PlayerZero.h"
#include "Timer.h"
#include "Cursor.h"
#include <algorithm>

float JumpAngle;
float4 JumpVector;

void PlayerZero::IdleStart(const StateInfo& _Info)
{
	IsJump = false;
	Renderer_Character->ChangeFrameAnimation("idle");
}

void PlayerZero::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == IsFall)
	{
		PlayerStateManager.ChangeState("Fall");
	}

	// Jump
	if (InputDir.y > 0 && !IsFall)
	{
		PlayerStateManager.ChangeState("Jump");
	}

	// Run
	if (abs(InputDir.x) > 0 && !IsFall)
	{
		PlayerStateManager.ChangeState("IdleToRun");
	}

	// 땅 뚫고 내려가기
	if (InputDir.y < 0 && !IsFall && DoubleDownBlue)
	{
		GetTransform().SetWorldMove({ 0, -2, 0 });
	}

}

void PlayerZero::AttackStart(const StateInfo& _Info)
{
	CreateSlash();
	AttackTimer->Activate();
	Renderer_Character->ChangeFrameAnimation("attack");
	MoveSpeed = 1600.0f;

}

void PlayerZero::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	MoveSpeed *= 0.99f;

	if (true == Attack_AniEnd)
	{
		MoveSpeed = SPEED_PLAYER;
		MouseDir = float4::ZERO;
		MoveDir = float4::ZERO;
		Attack_AniEnd = false;
		Renderer_Slash->Off();
		PlayerStateManager.ChangeState("Fall");
	}
}

void PlayerZero::FallStart(const StateInfo& _Info)
{
	FallDeltaTime = 0;
	MoveDir.y = 0;
	Renderer_Character->ChangeFrameAnimation("fall");
}

void PlayerZero::FallUpdate(float _DeltaTime, const StateInfo& _Info)
{
	MoveDir.x = InputDir.x * 0.3f;
	MoveDir.y = sinf(0)* FallDeltaTime - 0.5f * 9.8f * pow(FallDeltaTime, 2);
	FallDeltaTime += _DeltaTime;


	if (false == IsFall)
	{
		FallDeltaTime = 0;
		MoveDir = float4::ZERO;
		Velocity = float4::ZERO;
		MoveSpeed = SPEED_PLAYER;
		PlayerStateManager.ChangeState("Idle");
	}


}


void PlayerZero::JumpStart(const StateInfo& _Info)
{
	IsJump = true;
	JumpDeltaTime = 0.0f;
	MoveForce = std::clamp(GameEngineInput::GetInst()->GetTime("W") * 100, 0.0f, 1.0f);
	JumpVector = InputDir.NormalizeReturn();
	JumpVector.y *= MoveForce;
	JumpAngle = float4::VectorXYtoDegree(float4::ZERO, JumpVector); // W 입력 시간에 따른 점프강도
	MoveSpeed = 500.0f;
	Renderer_Character->ChangeFrameAnimation("jump");
}

void PlayerZero::JumpUpdate(float _DeltaTime, const StateInfo& _Info)
{
	MoveDir.x = InputDir.x * 0.3;
	JumpDeltaTime += DeltaTime;
	MoveDir.y = sinf(90) - 0.5f * 9.8f * pow(JumpDeltaTime, 2);

	
	if (MoveDir.y <= 0.005f && IsFall)
	{
		MoveSpeed = SPEED_PLAYER;
		JumpDeltaTime = 0;
		MoveForce = 0;
		MoveDir.y = 0;
		IsJump = false;
		PlayerStateManager.ChangeState("Fall");
	}

}

void PlayerZero::RollStart(const StateInfo& _Info)
{
	RollTimer->Activate();
	Renderer_Character->ChangeFrameAnimation("roll");
	MoveSpeed = SPEED_PLAYER * 1.5f;
}

void PlayerZero::RollUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == Roll_AniEnd)
	{
		Roll_AniEnd = false;
		MoveSpeed = SPEED_PLAYER;

		if (abs(MoveDir.x) > 0)
		{
			PlayerStateManager.ChangeState("Run");
		}
		else
		{
			PlayerStateManager.ChangeState("Idle");
		}
	}
}

void PlayerZero::RunStart(const StateInfo& _Info)
{
	MoveDir.x = InputDir.x;
	MoveSpeed = SPEED_PLAYER;
	Renderer_Character->ChangeFrameAnimation("run");
}

void PlayerZero::RunUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (InputDir.y > 0)
	{
		PlayerStateManager.ChangeState("Jump");
	}

	// 중간 방향전환
	if (InputDir.x * MoveDir.x < 0)
	{
		PlayerStateManager.ChangeState("IdleToRun");
	}

	float4 RollDir = float4{ abs(InputDir.x), InputDir.y };
	if (RollDir.CompareInt2D({ 1, -1 }))
	{
		PlayerStateManager.ChangeState("Roll");
	}

	if (InputDir.CompareInt2D(float4::ZERO))
	{
		MoveDir.x = 0;
		PlayerStateManager.ChangeState("RunToIdle");
	}

}

void PlayerZero::WallSlideStart(const StateInfo& _Info)
{
}

void PlayerZero::WallSlideUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

void PlayerZero::CrouchStart()
{
	Renderer_Character->ChangeFrameAnimation("precrouch");
}

void PlayerZero::CrouchUpdate()
{

}

void PlayerZero::RunToIdleStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("run_to_idle");
}

void PlayerZero::RunToIdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == RunIdle_AniEnd)
	{
		PlayerStateManager.ChangeState("Idle");
	}

	if (InputDir.y > 0)
	{
		PlayerStateManager.ChangeState("Jump");
	}
}

void PlayerZero::IdleToRunStart(const StateInfo& _Info)
{
	MoveSpeed = 20.0f;
	Renderer_Character->ChangeFrameAnimation("idle_to_run");
}

void PlayerZero::IdleToRunUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == IdleRun_AniEnd)
	{
		PlayerStateManager.ChangeState("Run");
	}

	if (InputDir.y > 0)
	{
		PlayerStateManager.ChangeState("Jump");
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
	InputDir.x = MouseDir.x;
	MoveDir = MouseDir;

	float4 Rot = float4::VectorXYtoDegree(PlayerPos, MousePos);
	Renderer_Slash->GetTransform().SetWorldRotation({ 0, 0, Rot.z });
	
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
	std::string State = PlayerStateManager.GetPrevState();
	GameEngineDebug::OutPutString(State);

	std::string Output = "Velocity : " + std::to_string(Velocity.x) + "/" + std::to_string(Velocity.y);
	GameEngineDebug::OutPutString(Output);

	std::string Output2 = "MoveDir : " + std::to_string(MoveDir.x) + "/" + std::to_string(MoveDir.y);
	GameEngineDebug::OutPutString(Output2);
}
