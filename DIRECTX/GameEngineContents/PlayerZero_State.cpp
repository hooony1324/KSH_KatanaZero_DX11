#include "PreCompile.h"
#include "PlayerZero.h"
#include "Timer.h"
#include "Cursor.h"
#include <algorithm>


const float AntiGravity = 5.2f;

bool Flipable = false;
bool GroundAniStart = true;

void PlayerZero::IdleStart(const StateInfo& _Info)
{
	IsJump = false;
	Renderer_Character->ChangeFrameAnimation("idle");
}

void PlayerZero::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{

	if (true == IsFall && !IsJump)
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
	if (InputDir.y < 0 && DoubleDownBlue)
	{
		GetTransform().SetWorldMove({ 0, -4, 0 });
		PlayerStateManager.ChangeState("Fall");
	}

	float4 RollDir = float4{ abs(InputDir.x), InputDir.y };
	if (RollDir.CompareInt2D({ 1, -1 }))
	{
		PlayerStateManager.ChangeState("Roll");
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

	IsAttack = true;
	IsFlip = false;

}

void PlayerZero::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// 슬래쉬 애니메이션 끝나면 콜리전 끄기


	float DT = _Info.StateTime;
	MoveVec.x += InputDir.x * 0.4f * _DeltaTime;
	MoveVec.y = static_cast<float>(sinf(FlyAngle)) - 9.8f * DT / AntiGravity - FloatDeltaTime / 2;

	if (WallState == STATE_WALL::RIGHT || WallState == STATE_WALL::RIGHTSLOPE
		|| WallState == STATE_WALL::LEFT || WallState == STATE_WALL::LEFTSLOPE)
	{
		MoveVec.x = 0;
	}


	if (true == Attack_AniEnd)
	{
		MoveVec.y = 0.0f;
		Attack_AniEnd = false;
		IsAttack = false;
		Renderer_Slash->Off();
		PlayerStateManager.ChangeState("Fall");
	}

}

void PlayerZero::FallStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("fall");

	if (abs(MoveVec.x) <= 0.0005f)
	{
		MoveVec.x = 0.02f * CurLookDir;
	}
	FlyAngle = float4::VectorXYtoRadian({ 0, 0 }, { MoveVec.x, MoveVec.y });
}

void PlayerZero::FallUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;
	MoveVec.x += InputDir.x * 0.4f * _DeltaTime;
	MoveVec.y = static_cast<float>(sinf(FlyAngle)) - 9.8f * DT / AntiGravity;


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
	float DT = _Info.StateTime;
	MoveVec.y = static_cast<float>(sinf(FlyAngle)) - 9.8f * DT / AntiGravity;
	MoveVec.x += InputDir.x * 0.4f * _DeltaTime;

	if (MoveVec.y <= 0)
	{
		IsJump = false;
		PlayerStateManager.ChangeState("Fall");
	}
}

void PlayerZero::RollStart(const StateInfo& _Info)
{
	RollTimer->Activate();
	Renderer_Character->ChangeFrameAnimation("roll");
	MoveSpeed = SPEED_PLAYER * 1.3f;
	
	MoveVec.x = InputDir.x;
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


	if (IsFall)
	{
		PlayerStateManager.ChangeState("Fall");
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

	if (abs(InputDir.x) && IsFall)
	{
		PlayerStateManager.ChangeState("Fall");
	}

}

void PlayerZero::WallGrabStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("wallgrab");
	 // 렌더러만 벽에 붙임
	if (WallState == STATE_WALL::RIGHT)
	{
		// 오른쪽을 보고 달라붙음 -> 오른쪽 벽
		CurLookDir = 1;
		Renderer_Character->GetTransform().SetLocalPosition({ 10, 0, 0 });
	}
	else
	{
		CurLookDir = -1;
		Renderer_Character->GetTransform().SetLocalPosition({ -10, 0, 0 });
	}

	// MoveVec 이 음수면 잠깐 멈추고
	// MoveVec 이 양수면 조금 올라간다(Grab)
	// MoveVec 이 음수면 내려간다(Slide)
	MoveVec.x = 0; 
	FlyVector = { 0, MoveVec.y * 1.3f };
	MoveVec.y = 0;
	IsFlip = false;
	Flipable = false;
	WallGrab = true;
}

void PlayerZero::WallGrabUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;
	MoveVec.y = FlyVector.y - 9.8f * DT / AntiGravity;

	if (MoveVec.y <= 0.00005f)
	{
		PlayerStateManager.ChangeState("WallSlide");
	}

	if (GameEngineInput::GetInst()->IsDown("W"))
	{
		WallGrab = false;
		Renderer_Character->GetTransform().SetLocalPosition({ 0, 0, 0 });
		PlayerStateManager.ChangeState("Flip");
	}

	if (false == IsFall)
	{
		MoveVec = float4::ZERO;
		Velocity = float4::ZERO;
		Renderer_Character->GetTransform().SetLocalPosition({ 0, 0, 0 });
		PlayerStateManager.ChangeState("Idle");
		WallGrab = false;
	}

}

void PlayerZero::WallSlideStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("wallslide");

	FlyVector = { 0, MoveVec.y };
}

void PlayerZero::WallSlideUpdate(float _DeltaTime, const StateInfo& _Info)
{

	float DT = _Info.StateTime;
	MoveVec.y = FlyVector.y - 9.8f * DT / AntiGravity;

	if (false == IsFall)
	{
		MoveVec = float4::ZERO;
		Velocity = float4::ZERO;
		Renderer_Character->GetTransform().SetLocalPosition({ 0, 0, 0 });
		PlayerStateManager.ChangeState("Idle");
		WallGrab = false;
	}

	if (GameEngineInput::GetInst()->IsDown("W"))
	{
		Renderer_Character->GetTransform().SetLocalPosition({ 0, 0, 0 });
		PlayerStateManager.ChangeState("Flip");
		WallGrab = false;
	}

	
}

void PlayerZero::FlipStart(const StateInfo& _Info)
{
	IsFlip = true;
	WallGrab = false;
	Renderer_Character->ChangeFrameAnimation("flip");
	
	if (CurLookDir == -1)
	{
		FlyVector = { 1, 1.25f };
	}
	else
	{
		FlyVector = { -1, 1.25f };
	}
	MoveVec.x = FlyVector.x;
}

void PlayerZero::FlipUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;
	MoveVec.y = FlyVector.y - 9.8f * DT / AntiGravity;
	MoveVec.x += InputDir.x * 0.4f * _DeltaTime;

	if (false == IsFall)
	{
		IsFlip = false;
		MoveVec = float4::ZERO;
		Velocity = float4::ZERO;
		Renderer_Character->GetTransform().SetLocalPosition({ 0, 0, 0 });
		PlayerStateManager.ChangeState("Idle");
	}

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

	// 땅 뚫고 내려가기
	if (InputDir.y < 0 && DoubleDownBlue)
	{
		GetTransform().SetWorldMove({ 0, -4, 0 });
		PlayerStateManager.ChangeState("Fall");
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

	// 땅 뚫고 내려가기
	if (InputDir.y < 0 && DoubleDownBlue)
	{
		MoveSpeed = SPEED_PLAYER;
		GetTransform().SetWorldMove({ 0, -4, 0 });
		PlayerStateManager.ChangeState("Fall");
	}

	if (InputDir.y > 0)
	{
		MoveSpeed = SPEED_PLAYER;
		PlayerStateManager.ChangeState("Jump");
	}


}

void PlayerZero::DeadStart(const StateInfo& _Info)
{
	MoveSpeed = 150.0f;
	Renderer_Character->ChangeFrameAnimation("hurtfly");

	FlyVector.x *= 0.6f;
	FlyVector.y *= 20.0f;
	MoveVec = FlyVector;
	FlyAngle = float4::VectorXYtoRadian({ 0, 0 }, FlyVector);
	MoveSpeed *= 3.0f;

	if (MoveVec.x > 0)
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
	}
	else
	{
		Renderer_Character->GetTransform().PixLocalPositiveX();
	}

	GroundAniStart = false;
}

void PlayerZero::DeadUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;

	
	MoveVec.y = FlyVector.y * sinf(FlyAngle) - (9.8f * DT) / 6.0f;

	if (MoveVec.y < 0 && false == IsFall)
	{
		if (false == GroundAniStart)
		{
			GroundAniStart = true;
			Renderer_Character->ChangeFrameAnimation("hurtground");
		}
		MoveVec.y = 0;
		MoveVec.x = GameEngineMath::LerpLimit(FlyVector.x, 0, DT);
	}

	// 벽과 충돌 체크
	if (WallState == STATE_WALL::LEFT || WallState == STATE_WALL::RIGHT)
	{
		MoveVec.x *= -0.3f;
	}


}


void PlayerZero::CreateSlash()
{
	// 렌더러
	Renderer_Slash->On();
	Renderer_Slash->CurAnimationReset();

	// 마우스 좌표 얻어오기
	float4 PlayerPos = GetTransform().GetWorldPosition();
	PlayerPos.z = 0;
	MousePos = Cursor::GetCursorPosition();
	MousePos.z = 0;
	MouseDir = MousePos - PlayerPos;
	MouseDir.z = 0;
	MouseDir.Normalize();
	InputDir = MouseDir;

	// 회전
	float4 Rot = float4::VectorXYtoDegree(PlayerPos, MousePos);
	Renderer_Slash->GetTransform().SetWorldRotation({ 0, 0, Rot.z });
	Collision_Slash->GetTransform().SetWorldRotation({ 0, 0, Rot.z });

	// 소리(3개 있음)
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

void PlayerZero::WallGrabCheck()
{
	if (false == (WallState == STATE_WALL::RIGHT || WallState == STATE_WALL::LEFT))
	{
		return;
	}

	PlayerStateManager.ChangeState("WallGrab");
}


void PlayerZero::PrintPlayerDebug()
{
	if (Collision_Slash->IsUpdate())
	{
		GameEngineDebug::DrawBox(Collision_Slash->GetTransform(), { 1, 1, 1, 0.25f });
	}
	if (Collision_Character->IsUpdate())
	{
		GameEngineDebug::DrawBox(Collision_Character->GetTransform(), { 1, 0, 0, 0.25f });
	}

	//switch (WallState)
	//{
	//case CharacterActor::STATE_WALL::NONE:
	//	GameEngineDebug::OutPutString("NONE");
	//	break;
	//case CharacterActor::STATE_WALL::RIGHT:
	//	GameEngineDebug::OutPutString("RIGHT");
	//	break;
	//case CharacterActor::STATE_WALL::LEFT:
	//	GameEngineDebug::OutPutString("LEFT");
	//	break;
	//case CharacterActor::STATE_WALL::UP:
	//	GameEngineDebug::OutPutString("UP");
	//	break;
	//case CharacterActor::STATE_WALL::DOWN:
	//	GameEngineDebug::OutPutString("DOWN");
	//	break;
	//case CharacterActor::STATE_WALL::RIGHTSLOPE:
	//	GameEngineDebug::OutPutString("RIGHTSLOPE");
	//	break;
	//case CharacterActor::STATE_WALL::LEFTSLOPE:
	//	GameEngineDebug::OutPutString("LEFTSLOPE");
	//	break;
	//case CharacterActor::STATE_WALL::UNDERGROUND:
	//	GameEngineDebug::OutPutString("UNDERGROUND");
	//	break;
	//default:
	//	break;
	//}

	//std::string Output = "Velocity : " + std::to_string(Velocity.x) + "/" + std::to_string(Velocity.y);
	//GameEngineDebug::OutPutString(Output);

	//std::string Output2 = "MoveDir : " + std::to_string(MoveVec.x) + "/" + std::to_string(MoveVec.y);
	//GameEngineDebug::OutPutString(Output2);
}
