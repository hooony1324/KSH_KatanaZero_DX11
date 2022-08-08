#include "PreCompile.h"
#include "PlayerZero.h"

#include "Timer.h"
#include "Cursor.h"

const float4 Gravity = { 0, -9.8f, 0 };

PlayerZero::PlayerZero()
	: AttackAble(true)
	, RollAble(true)
	, InputDir(float4::ZERO)
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
	Renderer_Slash = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Slash->GetTransform().SetLocalPosition({ 0, 0, -5 });
	CreateAllAnimation();

	Renderer_Character->SetTexture("spr_idle_0.png");
	Renderer_Character->ScaleToTexture();
	Renderer_Slash->SetTexture("spr_slash_0.png");
	Renderer_Slash->ScaleToTexture();

	Renderer_Character->ChangeFrameAnimation("idle");
	Renderer_Slash->ChangeFrameAnimation("slash");
	
	// 중심 디버그용
	//GameEngineTextureRenderer* test = CreateComponent<GameEngineTextureRenderer>();
	//test->SetTexture("None.png");
	//test->ScaleToTexture();

	// 콜리전
	Collision_Character = CreateComponent<GameEngineCollision>();
	Collision_Character->GetTransform().SetLocalScale(Renderer_Character->GetTransform().GetLocalScale());
	Collision_Character->ChangeOrder(COLLISIONGROUP::PLAYER);

	Collision_Slash = CreateComponent<GameEngineCollision>();
	Collision_Slash->GetTransform().SetLocalScale(Renderer_Slash->GetTransform().GetLocalScale());
	Collision_Slash->ChangeOrder(COLLISIONGROUP::PLAYER_ATTACK);
	Collision_Slash->Off();

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
}

void PlayerZero::Update(float _DeltaTime)
{
	// 프리카메라 모드면 움직임 X
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
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

	PrintPlayerDebug();
}

void PlayerZero::End()
{
}

void PlayerZero::InputCheck()
{
	InputDir = float4::ZERO;
	// CLICK
	if (GameEngineInput::GetInst()->IsDown("MouseLeft") && AttackAble)
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
		Velocity.x = -1;
		if (!IsAttack && IsFall && InputDir.x > 0 || IsFlip)
		{
			WallGrabCheck();
		}
		break;
	}
	case CharacterActor::STATE_WALL::LEFT:
	{
		Velocity.x = 1;
		if (!IsAttack && IsFall && InputDir.x < 0 || IsFlip)
		{
			WallGrabCheck();
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
		Velocity.y = 2.0f;
		break;
	case CharacterActor::STATE_WALL::RIGHTSLOPE:
	{
		if (Velocity.y > 0)
		{
			break;
		}
		// 우상향
		float Vector = MoveSpeed * _DeltaTime;
		if (Velocity.x > 0 && CurLookDir > 0)
		{
			GetTransform().SetWorldMove({ Vector, Vector, 0 });
			return;
		}
		// 좌하향
		else if(Velocity.x < 0 && CurLookDir < 0)
		{
			GetTransform().SetWorldMove({ -Vector, -Vector, 0 });
			return;
		}
		break;
	}
	case CharacterActor::STATE_WALL::LEFTSLOPE:
	{
		if (Velocity.y > 0)
		{
			break;
		}
		// 우하향
		float Vector = MoveSpeed * _DeltaTime;
		if (Velocity.x > 0 && CurLookDir > 0)
		{
			GetTransform().SetWorldMove({ Vector, -Vector, 0 });
			return;
		}
		// 좌상향
		else if(Velocity.x < 0 && CurLookDir < 0)
		{
			GetTransform().SetWorldMove({ -Vector, Vector, 0 });
			return;
		}
		break;
	}
	default:
		break;
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
}


void PlayerZero::CreateAllAnimation()
{
	// Player
	Renderer_Character->CreateFrameAnimationFolder("attack", FrameAnimation_DESC{ "attack", 0.05f , false});
	Renderer_Character->CreateFrameAnimationFolder("fall", FrameAnimation_DESC{ "fall", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "idle", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("idle_to_run", FrameAnimation_DESC{ "idle_to_run", 0.05f , false});
	Renderer_Character->CreateFrameAnimationFolder("jump", FrameAnimation_DESC{ "jump", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("postcrouch", FrameAnimation_DESC{ "postcrouch", 0.08f , false});
	Renderer_Character->CreateFrameAnimationFolder("precrouch", FrameAnimation_DESC{ "precrouch", 0.08f , false});
	Renderer_Character->CreateFrameAnimationFolder("roll", FrameAnimation_DESC{ "roll", 0.05f, false });
	Renderer_Character->CreateFrameAnimationFolder("run", FrameAnimation_DESC{ "run", 0.08f });
	Renderer_Character->CreateFrameAnimationFolder("run_to_idle", FrameAnimation_DESC{ "run_to_idle", 0.05f , false});
	Renderer_Character->CreateFrameAnimationFolder("wallgrab", FrameAnimation_DESC{ "wallgrab", 0.08f, false });
	Renderer_Character->CreateFrameAnimationFolder("wallslide", FrameAnimation_DESC{ "wallslide", 0.08f, false });
	Renderer_Character->CreateFrameAnimationFolder("flip", FrameAnimation_DESC{ "flip", 0.05f, false });
	
	Renderer_Character->CreateFrameAnimationFolder("hurtfly", FrameAnimation_DESC{ "hurtfly", 0.1125f, false });
	Renderer_Character->CreateFrameAnimationFolder("hurtground", FrameAnimation_DESC{ "hurtground", 0.1125f, false });


	// Slash
	Renderer_Slash->CreateFrameAnimationFolder("slash", FrameAnimation_DESC{ "player_slash", 0.07f, false });
	Renderer_Slash->Off();


	// 애니메이션 바인딩
	Renderer_Character->AnimationBindStart("idle_to_run", [=](const FrameAnimation_DESC&) { IdleRun_AniEnd = false; });
	Renderer_Character->AnimationBindFrame("idle_to_run", [=](const FrameAnimation_DESC&) 
		{
			if (InputDir.CompareInt2D({ 0, 0 }))
			{
				MoveVec = float4::ZERO;
				PlayerStateManager.ChangeState("Idle");
			}
		});
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
			Collision_Slash->Off();
		});
}

