#include "PreCompile.h"
#include "PlayerZero.h"
#include "Timer.h"
#include "Cursor.h"
#include <algorithm>

float FlyAngle;
float4 FlyVector;
const float AntiGravity = 5.2f;

void PlayerZero::IdleStart(const StateInfo& _Info)
{
	IsJump = false;
	Renderer_Character->ChangeFrameAnimation("idle");
}

void PlayerZero::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{

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
	if (InputDir.y < 0 && DoubleDownBlue)
	{
		GetTransform().SetWorldMove({ 0, -4, 0 });
		PlayerStateManager.ChangeState("Fall");

	}

	if (true == IsFall && !IsJump)
	{
		PlayerStateManager.ChangeState("Fall");
	}

}

void PlayerZero::AttackStart(const StateInfo& _Info)
{
	CreateSlash();
	AttackTimer->Activate();
	Renderer_Character->ChangeFrameAnimation("attack");
	
	// 포물선
	MoveVec = InputDir;
	MoveSpeed = SPEED_PLAYER;
	FlyAngle = float4::VectorXYtoRadian({ 0, 0 }, MoveVec);
	MoveVec.x = static_cast<float>(cosf(FlyAngle));
}

void PlayerZero::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float Dt = _Info.StateTime;
	MoveVec.x += InputDir.x * 0.4f * _DeltaTime;
	MoveVec.y = static_cast<float>(sinf(FlyAngle)) - 9.8f * Dt / AntiGravity - FloatDeltaTime / 2;


	if (true == Attack_AniEnd)
	{
		MoveVec.y = 0.0f;
		Attack_AniEnd = false;
		Renderer_Slash->Off();
		PlayerStateManager.ChangeState("Fall");
	}
}

void PlayerZero::FallStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("fall");

	if (abs(MoveVec.x) <= 0.0005f)
	{
		MoveVec.x = 0.001f * CurLookDir;
	}
	FlyAngle = float4::VectorXYtoRadian({ 0, 0 }, { MoveVec.x, MoveVec.y });
}

void PlayerZero::FallUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float Dt = _Info.StateTime;
	MoveVec.x += InputDir.x * 0.4f * _DeltaTime;
	MoveVec.y = static_cast<float>(sinf(FlyAngle)) - 9.8f * Dt / AntiGravity;


	if (false == IsFall)
	{
		MoveVec = float4::ZERO;
		Velocity = float4::ZERO;
		MoveSpeed = SPEED_PLAYER;
		PlayerStateManager.ChangeState("Idle");
	}


}


void PlayerZero::JumpStart(const StateInfo& _Info)
{
	IsJump = true;
	MoveSpeed = SPEED_PLAYER;
	Renderer_Character->ChangeFrameAnimation("jump");
	MoveVec = InputDir.NormalizeReturn();
	FlyAngle = float4::VectorXYtoRadian({ 0, 0 }, MoveVec);
}

void PlayerZero::JumpUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float Dt = _Info.StateTime;
	MoveVec.y = static_cast<float>(sinf(FlyAngle)) - 9.8f * Dt / AntiGravity;
	MoveVec.x += InputDir.x * 0.4f * _DeltaTime;

	if (MoveVec.y <= 0)
	{
		IsJump = false;
		PlayerStateManager.ChangeState("Fall");
	}

	GameEngineDebug::OutPutString(_Info.PrevState);
}

void PlayerZero::RollStart(const StateInfo& _Info)
{
	RollTimer->Activate();
	Renderer_Character->ChangeFrameAnimation("roll");
	MoveSpeed = SPEED_PLAYER * 1.3f;
}

void PlayerZero::RollUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == Roll_AniEnd)
	{
		Roll_AniEnd = false;
		MoveSpeed = SPEED_PLAYER;

		if (abs(MoveVec.x) > 0)
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
	MoveVec.x = InputDir.x;
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
	if (InputDir.x * MoveVec.x < 0)
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
		MoveVec.x = 0;
		PlayerStateManager.ChangeState("RunToIdle");
	}

	if (IsFall)
	{
		PlayerStateManager.ChangeState("Fall");
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
		MoveSpeed = SPEED_PLAYER;
		PlayerStateManager.ChangeState("Run");
	}

	if (InputDir.y > 0)
	{
		MoveSpeed = SPEED_PLAYER;
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
	InputDir = MouseDir;

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

	

	//std::string Output = "Velocity : " + std::to_string(Velocity.x) + "/" + std::to_string(Velocity.y);
	//GameEngineDebug::OutPutString(Output);

	//std::string Output2 = "MoveDir : " + std::to_string(MoveVec.x) + "/" + std::to_string(MoveVec.y);
	//GameEngineDebug::OutPutString(Output2);
}
