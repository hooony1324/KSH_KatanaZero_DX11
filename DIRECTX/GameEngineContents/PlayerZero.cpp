#include "PreCompile.h"
#include "PlayerZero.h"

#include "Timer.h"
#include "Cursor.h"
#include <GameEngineBase/magic_enum.hpp>
#include "Door.h"


const float4 Gravity = { 0, -9.8f, 0 };

PlayerZero::PlayerZero()
	: AttackAble(true)
	, RollAble(true)
	, MouseDir(float4::ZERO)

{
	InitSpeed = SPEED_PLAYER;
	MoveSpeed = InitSpeed;
}

PlayerZero::~PlayerZero()
{
}

void PlayerZero::Start()
{
	// 렌더러
	Renderer_Character = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Character->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));
	Renderer_Character->SetSamplingModePoint();

	Renderer_Slash = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Slash->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));
	Renderer_Slash->SetSamplingModePoint();
	Renderer_Slash->GetTransform().SetLocalPosition({ 0, 0, -5 });
	CreateAllAnimation();

	Renderer_Character->SetTexture("spr_idle_0.png");
	Renderer_Character->ScaleToTexture();
	Renderer_Slash->SetTexture("spr_slash_0.png");
	Renderer_Slash->GetTransform().SetLocalScale({92, 32, 1});

	Renderer_Character->ChangeFrameAnimation("idle");
	Renderer_Slash->ChangeFrameAnimation("slash");

	// 콜리전
	float4 	CharacterScale = Renderer_Character->GetTransform().GetLocalScale();
	Collision_Character = CreateComponent<GameEngineCollision>();
	Collision_Character->GetTransform().SetLocalScale({ CharacterScale.x * 0.5f, CharacterScale.y * 0.8f, GetDepth(ACTOR_DEPTH::COLLISION)});
	Collision_Character->ChangeOrder(COLLISIONGROUP::PLAYER);
	Collision_Character->SetDebugSetting(CollisionType::CT_AABB2D, { 0, 0, 1, 0.25f });

	float4 SlashScale = Renderer_Slash->GetTransform().GetLocalScale();
	Collision_Slash = CreateComponent<GameEngineCollision>();
	Collision_Slash->GetTransform().SetLocalScale({ SlashScale.x * 0.7f, SlashScale.y * 0.7f, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision_Slash->ChangeOrder(COLLISIONGROUP::PLAYER_ATTACK);
	Collision_Slash->Off();
	Collision_Slash->SetDebugSetting(CollisionType::CT_OBB2D, { 1, 1, 1, 0.25f });
	Collision_Slash->SetCollisionMode(CollisionMode::Ex);

	// 쿨타임 설정
	AttackTimer = CreateComponent<Timer>();
	AttackTimer->Init(0.36f);
	RollTimer = CreateComponent<Timer>();
	RollTimer->Init(0.4f);

	// PlayerStateManager
	StateManagerInit();

	// 최초 상태
	PlayerStateManager.ChangeState("Idle");
	GetTransform().SetLocalScale({ 2, 2, 1 });

	// 벽체크
	FRenderer_WallState = CreateComponent<GameEngineFontRenderer>();
	FRenderer_WallState->SetText("fonttest");
	FRenderer_WallState->SetLeftAndRightSort(LeftAndRightSort::CENTER);
	FRenderer_WallState->SetPositionMode(FontPositionMode::WORLD);
	FRenderer_WallState->SetSize(16);
	FRenderer_WallState->GetTransform().SetLocalPosition({ 0, 30, 0 });

	FRenderer_FSMState = CreateComponent<GameEngineFontRenderer>();
	FRenderer_FSMState->SetText("default");
	FRenderer_FSMState->SetLeftAndRightSort(LeftAndRightSort::CENTER);
	FRenderer_FSMState->SetPositionMode(FontPositionMode::WORLD);
	FRenderer_FSMState->SetSize(16);
	FRenderer_FSMState->GetTransform().SetLocalPosition({ 0, 40, 0 });
}

void PlayerZero::Update(float _DeltaTime)
{
	// 역재생
	if (true == IsReverse)
	{
		return;
	}

	// 프리카메라 모드면 움직임 X
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	if (true == IsPlayerDeadEnd())
	{
		return;
	}

	GlobalValueManager::PlayerPos = GetTransform().GetWorldPosition();
	EnemyAttackCheck();
	WallCheck();
	InputCheck();

	PlayerStateManager.Update(_DeltaTime);
	FloatTimeCheck(_DeltaTime);


	// 이동
	LookCheck(InputDir.x);
	PlayerMove(_DeltaTime);
	CoolTimeCheck();

	// 디버그
	//PrintPlayerDebug();
	auto State = magic_enum::enum_name(WallState);
	GlobalValueManager::PlayerWallState = State;

}

void PlayerZero::End()
{
}

void PlayerZero::InputCheck()
{
	if (false == InputValid)
	{
		return;
	}

	InputDir = float4::ZERO;
	// 게임 화면 밖이면 공격 안하도록
	bool ClickAble = Cursor::IsClickAble();


	if (true == ClickAble && GameEngineInput::GetInst()->IsDown("MouseLeft") && AttackAble)
	{
		PlayerStateManager.ChangeState("Attack");
	}

	// WASD
	if (GameEngineInput::GetInst()->IsPress("w"))
	{
		InputDir[1] += 1;
	}
	if (GameEngineInput::GetInst()->IsPress("a") && !WallGrab)
	{
		InputDir[0] += -1;
	}
	if (GameEngineInput::GetInst()->IsPress("s"))
	{
		InputDir[1] += -1;
	}
	if (GameEngineInput::GetInst()->IsPress("d") && !WallGrab)
	{
		InputDir[0] += 1;
	}
}

void PlayerZero::PlayerMove(float _DeltaTime)
{	
	Velocity = MoveVec * MoveSpeed * _DeltaTime;

	// 벽 막기
	switch (WallState)
	{
	case STATE_WALL::NONE:
		break;
	case CharacterActor::STATE_WALL::RIGHT:
	{

		if (!IsAttack && IsFall && MoveVec.x > 0 || IsFlip)
		{
			if (InputDir.x <= 0 && !IsFlip)
			{
				Velocity.x = 0;
			}
			else
			{
				WallGrabCheck();
			}
		}
		else
		{
			GetTransform().SetWorldMove(float4::LEFT);
			return;
		}

		break;
	}
	case CharacterActor::STATE_WALL::LEFT:
	{

		if (!IsAttack && IsFall && MoveVec.x < 0 || IsFlip)
		{
			if (InputDir.x >= 0 && !IsFlip)
			{
				Velocity.x = 0;
			}
			else
			{
				WallGrabCheck();
			}
		}
		else
		{
			GetTransform().SetWorldMove(float4::RIGHT);
			return;
		}

		break;
	}		
	case CharacterActor::STATE_WALL::UP:
		Velocity.y = -1;
		break;
	case CharacterActor::STATE_WALL::DOWN:
	{
		if (MoveVec.y < 0)
		{
			MoveVec.y = 0;
		}
		break;
	}
	case CharacterActor::STATE_WALL::UNDERGROUND:
		GetTransform().SetWorldMove({ 0, 1, 0 });
		return;
		break;
	case CharacterActor::STATE_WALL::RIGHTSLOPE:
	{
		// 점프나 공격 위로하면 슬로프 타는게 아님
		if (Velocity.y > 1.0f)
		{
			break;
		}

		// 움직임 없으면 가만히 있을것
		if (abs(Velocity.x) < 0.5f )
		{
			Velocity.y = 0;
			return;
		}
		
		float NewVelocity = Velocity.x;
		Velocity = float4{ NewVelocity, NewVelocity, 0 };
		break;

	}
	case CharacterActor::STATE_WALL::LEFTSLOPE:
	{
		if (Velocity.y > 1.0f)
		{
			break;
		}

		if (abs(Velocity.x) < 0.5f)
		{
			Velocity.y = 0;
			return;
		}

		float NewVelocity = Velocity.x * 0.75f;
		Velocity = float4{ NewVelocity, -NewVelocity, 0 };
		break;
	}
	default:
		break;
	}

	if (false == DoorBreaking)
	{
		Collision_Character->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::DOOR, CollisionType::CT_AABB2D,
			[=](GameEngineCollision* _This, GameEngineCollision* _Other)
			{
				DoorBreaking = true;
				PlayerStateManager.ChangeState("DoorBreak");
				Velocity.x = 0;
				DoorPtr = dynamic_cast<Door*>(_Other->GetActor());
				return CollisionReturn::Break;
			});
	}



	GetTransform().SetWorldMove(Velocity);

}

void PlayerZero::CoolTimeCheck()
{
	AttackAble = !AttackTimer->IsCoolTime();
	RollAble = !RollTimer->IsCoolTime();
}

void PlayerZero::StateManagerInit()
{
	PlayerStateManager.CreateStateMember("Idle"
		, std::bind(&PlayerZero::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayerZero::IdleStart, this, std::placeholders::_1)
	);;

	PlayerStateManager.CreateStateMember("Jump"
		, std::bind(&PlayerZero::JumpUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayerZero::JumpStart, this, std::placeholders::_1)
	);;
	PlayerStateManager.CreateStateMember("Fall"
		, std::bind(&PlayerZero::FallUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayerZero::FallStart, this, std::placeholders::_1)
	);;
	PlayerStateManager.CreateStateMember("Attack"
		, std::bind(&PlayerZero::AttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayerZero::AttackStart, this, std::placeholders::_1)
	);;
	PlayerStateManager.CreateStateMember("Roll"
		, std::bind(&PlayerZero::RollUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayerZero::RollStart, this, std::placeholders::_1)
	);;
	PlayerStateManager.CreateStateMember("Run"
		, std::bind(&PlayerZero::RunUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayerZero::RunStart, this, std::placeholders::_1)
	);;
	PlayerStateManager.CreateStateMember("RunToIdle"
		, std::bind(&PlayerZero::RunToIdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayerZero::RunToIdleStart, this, std::placeholders::_1)
	);;
	PlayerStateManager.CreateStateMember("IdleToRun"
		, std::bind(&PlayerZero::IdleToRunUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayerZero::IdleToRunStart, this, std::placeholders::_1)
	);;
	PlayerStateManager.CreateStateMember("WallGrab"
		, std::bind(&PlayerZero::WallGrabUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayerZero::WallGrabStart, this, std::placeholders::_1)
	);;
	PlayerStateManager.CreateStateMember("WallSlide"
		, std::bind(&PlayerZero::WallSlideUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayerZero::WallSlideStart, this, std::placeholders::_1)
	);;
	PlayerStateManager.CreateStateMember("Flip"
		, std::bind(&PlayerZero::FlipUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayerZero::FlipStart, this, std::placeholders::_1)
	);;

	// DEAD추가
	PlayerStateManager.CreateStateMember("Dead"
		, std::bind(&PlayerZero::DeadUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayerZero::DeadStart, this, std::placeholders::_1)
	);;

	PlayerStateManager.CreateStateMember("DoorBreak"
		, std::bind(&PlayerZero::DoorBreakUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayerZero::DoorBreakStart, this, std::placeholders::_1)
	);;
}


void PlayerZero::CreateAllAnimation()
{
	// Player
	Renderer_Character->SetScaleModeImage();
	Renderer_Character->CreateFrameAnimationFolder("attack", FrameAnimation_DESC{ "attack", 0.03f , false});
	Renderer_Character->CreateFrameAnimationFolder("fall", FrameAnimation_DESC{ "fall", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "idle", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("idle_to_run", FrameAnimation_DESC{ "idle_to_run", 0.04f , false});
	Renderer_Character->CreateFrameAnimationFolder("jump", FrameAnimation_DESC{ "jump", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("postcrouch", FrameAnimation_DESC{ "postcrouch", 0.08f , false});
	Renderer_Character->CreateFrameAnimationFolder("precrouch", FrameAnimation_DESC{ "precrouch", 0.08f , false});
	Renderer_Character->CreateFrameAnimationFolder("roll", FrameAnimation_DESC{ "roll", 0.05f });
	Renderer_Character->CreateFrameAnimationFolder("run", FrameAnimation_DESC{ "run", 0.08f });
	Renderer_Character->CreateFrameAnimationFolder("run_to_idle", FrameAnimation_DESC{ "run_to_idle", 0.08f , false});
	Renderer_Character->CreateFrameAnimationFolder("wallgrab", FrameAnimation_DESC{ "wallgrab", 0.08f, false });
	Renderer_Character->CreateFrameAnimationFolder("wallslide", FrameAnimation_DESC{ "wallslide", 0.08f, false });
	Renderer_Character->CreateFrameAnimationFolder("flip", FrameAnimation_DESC{ "flip", 0.05f, false });
	
	Renderer_Character->CreateFrameAnimationFolder("hurtfly", FrameAnimation_DESC{ "hurtfly", 0.1125f, false });
	Renderer_Character->CreateFrameAnimationFolder("hurtground", FrameAnimation_DESC{ "hurtground", 0.1125f, false });

	Renderer_Character->CreateFrameAnimationFolder("doorbreak", FrameAnimation_DESC{ "doorbreak", 0.1f , false });

	// Slash
	Renderer_Slash->CreateFrameAnimationFolder("slash", FrameAnimation_DESC{ "player_slash", 0.03f, false });
	Renderer_Slash->Off();

	// 애니메이션 바인딩
	Renderer_Character->AnimationBindStart("idle_to_run", [=](const FrameAnimation_DESC&) { IdleRun_AniEnd = false; });
	//Renderer_Character->AnimationBindFrame("idle_to_run", [=](const FrameAnimation_DESC& _Info) 
		//{
		//	// 걷기 -> 달리기 중단하고 걷기->IDLE
		//
		//});
	Renderer_Character->AnimationBindEnd("idle_to_run", [=](const FrameAnimation_DESC&) { IdleRun_AniEnd = true; });

	Renderer_Character->AnimationBindStart("run_to_idle", [=](const FrameAnimation_DESC&) { RunIdle_AniEnd = false; });
	Renderer_Character->AnimationBindEnd("run_to_idle", [=](const FrameAnimation_DESC&) { RunIdle_AniEnd = true; });

	Renderer_Character->AnimationBindStart("roll", [=](const FrameAnimation_DESC&) { Roll_AniEnd = false; });
	Renderer_Character->AnimationBindEnd("roll", [=](const FrameAnimation_DESC&) { Roll_AniEnd = true; });

	Renderer_Character->AnimationBindStart("attack", [=](const FrameAnimation_DESC&) 
		{ 		
			Attack_AniEnd = false;
		});
	Renderer_Character->AnimationBindEnd("attack", [=](const FrameAnimation_DESC&) 
		{
			IsAttack = false;
			Renderer_Slash->Off();
			Collision_Slash->Off();
			Attack_AniEnd = true;
		});


	Renderer_Slash->AnimationBindFrame("slash", [=](const FrameAnimation_DESC& _Info)
		{
			_Info.CurFrame;
			if (_Info.CurFrame == 1)
			{
				Collision_Slash->On();
			}
		});

	Renderer_Slash->AnimationBindEnd("slash", [=](const FrameAnimation_DESC& _Info)
		{
			Renderer_Slash->Off();
			Collision_Slash->Off();
		});

	Renderer_Character->AnimationBindEnd("hurtground", [=](const FrameAnimation_DESC& _Info)
		{
			DeadAniend = true;
		});

	// DoorBreak
	Renderer_Character->AnimationBindStart("doorbreak", [=](const FrameAnimation_DESC& _Info)
		{
			Renderer_Character->SetPivotToVector({ 0, 5, 0 });
		});

	Renderer_Character->AnimationBindFrame("doorbreak", [=](const FrameAnimation_DESC& _Info)
		{
			if (4 == _Info.CurFrame)
			{
				if (nullptr != DoorPtr)
				{
					DoorPtr->Open();
					DoorPtr = nullptr;
				}
			}
		}); 
	Renderer_Character->AnimationBindEnd("doorbreak", [=](const FrameAnimation_DESC&) 
		{ 
			DoorBreaking = false;
			PlayerStateManager.ChangeState("Idle");
			Renderer_Character->SetPivotToVector(float4::ZERO);
		});
}

