#include "PreCompile.h"
#include "PlayerZero.h"
#include "Timer.h"
#include "Cursor.h"
#include <algorithm>

#include "Door.h"
#include "JumpCloud.h"
#include "LandCloud.h"
#include "DustCloud.h"
#include "GameContentsCustomRenderer.h"
#include "ParticleShooter.h"

const float AntiGravity = 5.2f;

bool Flipable = false;
bool GroundAniStart = true;

void PlayerZero::IdleStart(const StateInfo& _Info)
{
	IsJump = false;
	Renderer_Character->ChangeFrameAnimation("idle");
	WallGrab = false;
	IsFlip = false;
	IsAttack = false;

	if (0 == _Info.PrevState.compare("Fall"))
	{
		// FX
		LandCloud* Cloud = GetLevel()->CreateActor<LandCloud>();
		Cloud->GetTransform().SetWorldPosition(GetTransform().GetWorldPosition() + float4{ 0, -35 });
	}
}

void PlayerZero::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{

	if (true == IsFall && !IsJump)
	{
		PlayerStateManager.ChangeState("Fall");
		return;
	}

	// Jump
	if (InputDir.y > 0 && !IsFall)
	{
		PlayerStateManager.ChangeState("Jump");
		return;
	}

	// Roll
	if (abs(InputDir.x) >= 1.0f && InputDir.y <= -1.0f)
	{
		PlayerStateManager.ChangeState("Roll");
		return;
	}

	// Run
	if (abs(InputDir.x) > 0 && !IsFall)
	{
		PlayerStateManager.ChangeState("IdleToRun");
		return;
	}

	// 땅 뚫고 내려가기
	if (InputDir.y < 0 && DoubleDownBlue)
	{
		GetTransform().SetWorldMove({ 0, -3, 0 });
		Renderer_Character->ChangeFrameAnimation("fall");
		PlayerStateManager.ChangeState("Fall");
		return;
	}


}

void PlayerZero::AttackStart(const StateInfo& _Info)
{
	CreateSlash();
	AttackTimer->Activate();
	Renderer_Character->ChangeFrameAnimation("attack");
	
	// 포물선
	MoveVec = InputDir;
	MoveSpeed = 700;
	MoveVec.y *= 1.5f;
	FlyAngle = float4::VectorXYtoRadian({ 0, 0 }, MoveVec);

	IsAttack = true;
	IsFlip = false;


	if (MoveVec.x >= 0.7f && WallState == STATE_WALL::RIGHTSLOPE)
	{
		MoveVec = float4::VectorRotationToDegreeZAxis(float4::RIGHT, 55).NormalizeReturn();
	}

	if (MoveVec.x <= -0.7f && WallState == STATE_WALL::LEFTSLOPE)
	{
		MoveVec = float4::VectorRotationToDegreeZAxis(float4::RIGHT, 50).NormalizeReturn();
		MoveVec.x *= -1.0f;
	}

	if (MoveVec.y <= 0.08f)
	{
		GetTransform().SetWorldMove({ 0, 10, 0 });
		MoveVec.y = 0.7f;
	}

}

void PlayerZero::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	CreateBrightShadow();

	// 슬래쉬 애니메이션 끝나면 콜리전 끄기
	float DT = _Info.StateTime;
	MoveVec.x = GameEngineMath::Lerp(MoveVec.x, 0, _DeltaTime);
	
	float MinusPower = FloatDeltaTime / 2;
	if (MinusPower > 0.7f)
	{
		MinusPower = 0.7f;
	}

	MoveVec.y = static_cast<float>(sinf(FlyAngle)) - 9.8f * DT / AntiGravity - MinusPower;


	if (true == Attack_AniEnd)
	{
		Renderer_Character->ChangeFrameAnimation("fall");
		Attack_AniEnd = false;
	}

	if (MoveVec.y < 0)
	{
		MoveSpeed = SPEED_PLAYER;
		PlayerStateManager.ChangeState("Fall");
		return;
	}


}

void PlayerZero::FallStart(const StateInfo& _Info)
{
	MoveSpeed = 600;

	if (abs(MoveVec.x) <= 0.005f)
	{
		MoveVec.x = 0.04f * CurLookDir;
	}


	FlyAngle = float4::VectorXYtoRadian({ 0, 0 }, { MoveVec.x, MoveVec.y });
}

void PlayerZero::FallUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;

	if (abs(InputDir.x) < 0.1f)
	{
		//_DeltaTime * 1.5f
		MoveVec.x = GameEngineMath::Lerp(MoveVec.x, 0, _DeltaTime);
	}
	else
	{
		MoveVec.x += InputDir.x * _DeltaTime * 3;
		if (abs(MoveVec.x) > 0.7f)
		{
			MoveVec.x = MoveVec.x < 0.0f ? -0.7f : 0.7f;
		}
	}

	MoveVec.y = static_cast<float>(sinf(FlyAngle)) - 9.8f * DT / AntiGravity * 0.8f;
	// 부유감 좀 있음
	if (MoveVec.y <= -0.8f)
	{
		MoveVec.y = -0.8f;
	}


	if (DownBlue || DoubleDownBlue || 
		WallState == STATE_WALL::RIGHTSLOPE || WallState == STATE_WALL::LEFTSLOPE)
	{
		MoveVec = float4::ZERO;
		Velocity = float4::ZERO;
		MoveSpeed = SPEED_PLAYER;
		PlayerStateManager.ChangeState("Idle");
		return;
	}

	if (false == IsFall)
	{
		MoveVec = float4::ZERO;
		Velocity = float4::ZERO;
		MoveSpeed = SPEED_PLAYER;
		PlayerStateManager.ChangeState("Idle");
		return;
	}
}


void PlayerZero::JumpStart(const StateInfo& _Info)
{
	IsJump = true;
	MoveSpeed = 700;
	Renderer_Character->ChangeFrameAnimation("jump");
	MoveVec = InputDir.NormalizeReturn();

	if (MoveVec.x < 0.1f)
	{
		MoveVec.y = 0.7f;
	}

	if (MoveVec.x >= 0.7f && WallState == STATE_WALL::RIGHTSLOPE)
	{
		MoveVec = float4::VectorRotationToDegreeZAxis(float4::RIGHT, 55).NormalizeReturn();
	}

	if (MoveVec.x <= -0.7f && WallState == STATE_WALL::LEFTSLOPE)
	{
		MoveVec = float4::VectorRotationToDegreeZAxis(float4::RIGHT, 50).NormalizeReturn();
		MoveVec.x *= -1.0f;
	}

	// FX
	JumpCloud* Cloud = GetLevel()->CreateActor<JumpCloud>();
	Cloud->GetTransform().SetWorldPosition(GetTransform().GetWorldPosition() + float4{0, -35});
}

void PlayerZero::JumpUpdate(float _DeltaTime, const StateInfo& _Info)
{
	CreateBrightShadow();

	float DT = _Info.StateTime;

	MoveVec.y = MoveVec.y - 9.8f * _DeltaTime / AntiGravity * 0.8f;
	MoveVec.x = GameEngineMath::Lerp(MoveVec.x, 0, _DeltaTime);

	if (MoveVec.y <= 0)
	{
		IsJump = false;
		MoveSpeed = SPEED_PLAYER;

		Renderer_Character->ChangeFrameAnimation("fall");
		PlayerStateManager.ChangeState("Fall");
	}

}

float CloudSumTime;
int CloudNum = 8;
void PlayerZero::RollStart(const StateInfo& _Info)
{
	RollTimer->Activate();
	Renderer_Character->ChangeFrameAnimation("roll");
	
	MoveVec.x = InputDir.x;
	MoveSpeed = SPEED_PLAYER * 1.7f;
	Invincible = true;

	RollSoundPlayer = GameEngineSound::SoundPlayControl("sound_player_roll.wav");
	RollSoundPlayer.Volume(0.05f);

	CloudSumTime = 0.0f;

}

void PlayerZero::RollUpdate(float _DeltaTime, const StateInfo& _Info)
{
	CreateBrightShadow();

	CloudSumTime += _DeltaTime;
	if (CloudSumTime > 0.03f)
	{
		CloudSumTime = 0.0f;
		float4 Dir = { MoveVec.x * -1, 0 };
		CloudShooter->OneShot<DustCloud>(2, GetTransform().GetWorldPosition() + float4{0, -30}, Dir, 200);
	}


	if (true == Roll_AniEnd)
	{
		Roll_AniEnd = false;
		MoveSpeed = SPEED_PLAYER;
		Invincible = false;

		if (abs(InputDir.x) > 0)
		{
			PlayerStateManager.ChangeState("Run");
			return;
		}
		else
		{
			PlayerStateManager.ChangeState("RunToIdle");
			return;
		}
	}


	if (IsFall)
	{
		Invincible = false;
		Renderer_Character->ChangeFrameAnimation("fall");
		PlayerStateManager.ChangeState("Fall");
		return;
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
		return;
	}

	// 중간 방향전환
	if (InputDir.x * MoveVec.x < 0)
	{
		PlayerStateManager.ChangeState("IdleToRun");
		return;
	}

	float4 RollDir = float4{ abs(InputDir.x), InputDir.y };
	if (RollDir.CompareInt2D({ 1, -1 }))
	{
		PlayerStateManager.ChangeState("Roll");
		return;
	}

	if (InputDir.CompareInt2D(float4::ZERO))
	{
		PlayerStateManager.ChangeState("RunToIdle");
		return;
	}

	// 슬로프->평지 문제있음
	// 달리다가 슬로프, 달리다가 낭떨어지 구분
	if (WallState == STATE_WALL::NONE)
	{
		Renderer_Character->ChangeFrameAnimation("fall");
		PlayerStateManager.ChangeState("Fall");
		return;
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
	FlyVector = { 0, MoveVec.y * 1.6f };
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
		return;
	}

	if (GameEngineInput::GetInst()->IsDown("W"))
	{
		WallGrab = false;
		Renderer_Character->GetTransform().SetLocalPosition({ 0, 0, 0 });
		PlayerStateManager.ChangeState("Flip");
		return;
	}

	if (false == IsFall)
	{
		MoveVec = float4::ZERO;
		Velocity = float4::ZERO;
		Renderer_Character->GetTransform().SetLocalPosition({ 0, 0, 0 });
		PlayerStateManager.ChangeState("Idle");
		WallGrab = false;
		return;
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
		return;
	}

	if (GameEngineInput::GetInst()->IsDown("W"))
	{
		Renderer_Character->GetTransform().SetLocalPosition({ 0, 0, 0 });
		PlayerStateManager.ChangeState("Flip");
		WallGrab = false;
		return;
	}

	
}


void PlayerZero::FlipStart(const StateInfo& _Info)
{
	IsFlip = true;
	WallGrab = false;
	Renderer_Character->ChangeFrameAnimation("flip");
	
	float y = abs(static_cast<float>(sinf(40 * GameEngineMath::DegreeToRadian)));
	float x = static_cast<float>(cosf(40 * GameEngineMath::DegreeToRadian));

	if (CurLookDir == -1)
	{
		FlyVector = float4{ x, y }.NormalizeReturn();
	}
	else
	{
		FlyVector = float4{ -x, y }.NormalizeReturn();
	}
	MoveVec.x = FlyVector.x;
	MoveSpeed = 700;

	RollSoundPlayer = GameEngineSound::SoundPlayControl("sound_player_roll.wav");
	RollSoundPlayer.Volume(0.05f);
}

void PlayerZero::FlipUpdate(float _DeltaTime, const StateInfo& _Info)
{
	CreateBrightShadow();

	float DT = _Info.StateTime;
	MoveVec.y = FlyVector.y - 9.8f * DT / AntiGravity;
	MoveVec.x = GameEngineMath::Lerp(MoveVec.x, 0, _DeltaTime * 0.7f);

	if (false == IsFall)
	{
		IsFlip = false;
		MoveVec = float4::ZERO;
		Velocity = float4::ZERO;
		MoveSpeed = SPEED_PLAYER;
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
	float DT = _Info.StateTime;
	
	MoveVec.x = GameEngineMath::Lerp(MoveVec.x, 0, _DeltaTime * 10.0f);

	// Roll
	if (abs(InputDir.x) >= 1.0f && InputDir.y <= -1.0f)
	{
		PlayerStateManager.ChangeState("Roll");
		return;
	}

	if (abs(MoveVec.x) <= 0.001f)
	{
		PlayerStateManager.ChangeState("Idle");
		return;
	}
	
	if (DT >= 0.15f && abs(InputDir.x) > 0.5f)
	{
		MoveVec.x = 0.0f;
		PlayerStateManager.ChangeState("IdleToRun");
		return;
	}
	

	if (InputDir.y > 0)
	{
		PlayerStateManager.ChangeState("Jump");
		return;
	}

	// 땅 뚫고 내려가기
	if (InputDir.y < 0 && DoubleDownBlue)
	{
		GetTransform().SetWorldMove({ 0, -3, 0 });
		Renderer_Character->ChangeFrameAnimation("fall");
		PlayerStateManager.ChangeState("Fall");
		return;
	}
}

void PlayerZero::IdleToRunStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("idle_to_run");
	
}

void PlayerZero::IdleToRunUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;
	float MoveX = pow(DT, 3.0f) * 50; // 25 : 0.2초만에 0~1
	MoveVec.x = MoveX * InputDir.x;

	// Roll
	if (abs(InputDir.x) >= 1.0f && InputDir.y <= -1.0f)
	{
		PlayerStateManager.ChangeState("Roll");
		return;
	}

	if (DT >= 0.2f)
	{
		float4 CloudDir{ MoveVec.x * -1, 0};
		CloudShooter->OneShot<DustCloud>(5, GetTransform().GetWorldPosition() + float4{0, -30}, CloudDir, 200);
		PlayerStateManager.ChangeState("Run");
		return;
	}

	if (abs(InputDir.x) < 0.1f)
	{
		PlayerStateManager.ChangeState("RunToIdle");
		return;
	}

	// 땅 뚫고 내려가기
	if (InputDir.y < 0 && DoubleDownBlue)
	{
		MoveSpeed = SPEED_PLAYER;
		GetTransform().SetWorldMove({ 0, -3, 0 });
	}

	if (true == IsFall)
	{
		Renderer_Character->ChangeFrameAnimation("fall");
		PlayerStateManager.ChangeState("Fall");
		return;
	}

	if (InputDir.y > 0)
	{
		MoveSpeed = SPEED_PLAYER;
		PlayerStateManager.ChangeState("Jump");
		return;
	}


}

void PlayerZero::DeadStart(const StateInfo& _Info)
{
	MoveSpeed = 150.0f;
	Renderer_Character->ChangeFrameAnimation("hurtfly");

	FlyVector.x *= 0.5f;
	FlyVector.y *= 1.2f;
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
	IsDead = true;
	DeadAniend = false;
	Collision_Character->Off();

	Renderer_Slash->Off();
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

void PlayerZero::DoorBreakStart(const StateInfo& _Info)
{
	MoveVec.x = 0;
	Renderer_Character->ChangeFrameAnimation("doorbreak");


	if (InputDir.x < 0)
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
	}
	else if (InputDir.x > 0)
	{
		Renderer_Character->GetTransform().PixLocalPositiveX();
	}

}

void PlayerZero::DoorBreakUpdate(float _DeltaTime, const StateInfo& _Info)
{

}


void PlayerZero::CreateSlash()
{
	// 렌더러
	Renderer_Slash->On();
	Renderer_Slash->CurAnimationReset();

	// 마우스 좌표 얻어오기
	float4 PlayerPos = GetTransform().GetWorldPosition();
	PlayerPos.z = 0;
	MousePos = Cursor::GetCursorPositionToActor();
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
	SlashSoundPlayer = GameEngineSound::SoundPlayControl(Sound);
	SlashSoundPlayer.Volume(0.1f);
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

	switch (WallState)
	{
	case CharacterActor::STATE_WALL::NONE:
		GameEngineDebug::OutPutString("NONE");
		break;
	case CharacterActor::STATE_WALL::RIGHT:
		GameEngineDebug::OutPutString("RIGHT");
		break;
	case CharacterActor::STATE_WALL::LEFT:
		GameEngineDebug::OutPutString("LEFT");
		break;
	case CharacterActor::STATE_WALL::UP:
		GameEngineDebug::OutPutString("UP");
		break;
	case CharacterActor::STATE_WALL::DOWN:
		GameEngineDebug::OutPutString("DOWN");
		break;
	case CharacterActor::STATE_WALL::RIGHTSLOPE:
		GameEngineDebug::OutPutString("RIGHTSLOPE");
		break;
	case CharacterActor::STATE_WALL::LEFTSLOPE:
		GameEngineDebug::OutPutString("LEFTSLOPE");
		break;
	case CharacterActor::STATE_WALL::UNDERGROUND:
		GameEngineDebug::OutPutString("UNDERGROUND");
		break;
	default:
		break;
	}
	


	//std::string Output = "Velocity : " + std::to_string(Velocity.x) + "/" + std::to_string(Velocity.y);
	//GameEngineDebug::OutPutString(Output);

	//std::string Output2 = "MoveDir : " + std::to_string(MoveVec.x) + "/" + std::to_string(MoveVec.y);
	//GameEngineDebug::OutPutString(Output2);
}
