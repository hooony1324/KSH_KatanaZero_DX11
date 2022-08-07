#include "PreCompile.h"
#include "EnemyActor.h"
#include <GameEngineCore/CoreMinimal.h>

#include "CharacterActor.h"

bool TurnEnd;
static float ChaseSensorPaddingX = 25.0f;

EnemyActor::EnemyActor() 
	: CurAction(ENEMYACTION::PATROL)
	, EnemyName(std::string())
	, MoveVec(float4::DOWN)
	, PrevLookDir(1)
	, WallState(STATE_WALL::NONE)
{
}

EnemyActor::~EnemyActor() 
{
	
}

void EnemyActor::EnemyActorDebug()
{
	switch (WallState)
	{
	case EnemyActor::STATE_WALL::NONE:
		GameEngineDebug::OutPutString("NONE");
		break;
	case EnemyActor::STATE_WALL::RIGHT:
		GameEngineDebug::OutPutString("RIGHT");
		break;
	case EnemyActor::STATE_WALL::LEFT:
		GameEngineDebug::OutPutString("LEFT");
		break;
	case EnemyActor::STATE_WALL::UP:
		GameEngineDebug::OutPutString("UP");
		break;
	case EnemyActor::STATE_WALL::DOWN:
		GameEngineDebug::OutPutString("DOWN");
		break;
	case EnemyActor::STATE_WALL::RIGHTSLOPE:
		GameEngineDebug::OutPutString("RIGHTSLOPE");
		break;
	case EnemyActor::STATE_WALL::LEFTSLOPE:
		GameEngineDebug::OutPutString("LEFTSLOPE");
		break;
	case EnemyActor::STATE_WALL::UNDERGROUND:
		GameEngineDebug::OutPutString("UNDERGROUND");
		break;
	default:
		break;
	}

	GameEngineDebug::DrawBox(Collision_ChaseSensor->GetTransform(), { 0, 0, 1, 0.25f });
}

void EnemyActor::CreateRendererAndCollision()
{
	Renderer_Character = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Character->SetScaleModeImage();

	Renderer_Alert = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Alert->SetTexture("spr_enemy_follow_0.png");
	Renderer_Alert->ScaleToTexture();
	Renderer_Alert->SetSamplingModePoint();
	Renderer_Alert->GetTransform().SetLocalPosition({ 0, 30, 0 });
	Renderer_Alert->Off();

	float4 RendererScale = Renderer_Character->GetTransform().GetLocalScale();
	Collision_Character = CreateComponent<GameEngineCollision>();
	Collision_Character->GetTransform().SetLocalScale({ RendererScale.x, RendererScale.y, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision_Character->ChangeOrder(COLLISIONGROUP::ENEMY);

	Collision_ChaseSensor = CreateComponent<GameEngineCollision>();
	Collision_ChaseSensor->GetTransform().SetLocalScale({ 120, 50, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision_ChaseSensor->GetTransform().SetLocalPosition({ ChaseSensorPaddingX, 0 , 0 });

	Collision_ChaseSensor->ChangeOrder(COLLISIONGROUP::ENEMY);
}

void EnemyActor::CreateAllFolderAnimation()
{
	if (EnemyName.empty())
	{
		MsgBoxAssert("Enemy가 자신의 이름을 설정하지 않을 채로 애니메이션을 생성하려고 했습니다");
	}

	Renderer_Character->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ EnemyName + "_idle", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("attack", FrameAnimation_DESC{ EnemyName + "_attack", 0.05f , false });
	Renderer_Character->CreateFrameAnimationFolder("walk", FrameAnimation_DESC{ EnemyName + "_walk", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("turn", FrameAnimation_DESC{ EnemyName + "_turn", 0.1125f, false });
	Renderer_Character->CreateFrameAnimationFolder("run", FrameAnimation_DESC{ EnemyName + "_run", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("hurtfly", FrameAnimation_DESC{ EnemyName + "_hurtfly", 0.1125f, false });
	Renderer_Character->CreateFrameAnimationFolder("hurtground", FrameAnimation_DESC{ EnemyName + "_hurtground", 0.1125f, false });

	// 바인드
	Renderer_Character->AnimationBindStart("turn", [=](const FrameAnimation_DESC&) { TurnEnd = false; });
	Renderer_Character->AnimationBindEnd("turn", [=](const FrameAnimation_DESC&) { TurnEnd = true; });
}

void EnemyActor::CreateAllState()
{
	StateManager.CreateStateMember("Spawn"
		, std::bind(&EnemyActor::SpawnUpdate, this, std::placeholders::_1, std::placeholders::_2));
	StateManager.CreateStateMember("Idle"
		, std::bind(&EnemyActor::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyActor::IdleStart, this, std::placeholders::_1));
	StateManager.CreateStateMember("Dead"
		, std::bind(&EnemyActor::DeadUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyActor::DeadStart, this, std::placeholders::_1));
	StateManager.CreateStateMember("Walk"
		, std::bind(&EnemyActor::WalkUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyActor::WalkStart, this, std::placeholders::_1));
	StateManager.CreateStateMember("PatrolTurn"
		, std::bind(&EnemyActor::PatrolTurnUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyActor::PatrolTurnStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Alert"
		, std::bind(&EnemyActor::AlertUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyActor::AlertStart, this, std::placeholders::_1));
	StateManager.CreateStateMember("Run"
		, std::bind(&EnemyActor::RunUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyActor::RunStart, this, std::placeholders::_1));
}

void EnemyActor::OnEvent()
{
	if (nullptr == Collision_ChaseSensor)
	{
		MsgBoxAssert("Enemy의 콜리전 센서가 nullptr 입니다");
	}

	CurCollisionMap = GlobalValueManager::ColMap;
	if (nullptr == CurCollisionMap)
	{
		MsgBoxAssert("충돌맵이 nullptr 입니다");
	}

	StateManager.ChangeState("Spawn");

	if (PrevLookDir > 0)
	{
		GetTransform().PixLocalPositiveX();
	}
	if (PrevLookDir < 0)
	{
		GetTransform().PixLocalNegativeX();
	}
}

void EnemyActor::WallCheck()
{
	// y값 반전 주의
	float4 CharacterPos = GetTransform().GetWorldPosition();
	Down = CurCollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 36))
		.CompareInt3D(float4::GREEN);
	DoubleDown = CurCollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 37))
		.CompareInt3D(float4::GREEN);
	DownBlue = CurCollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 36))
		.CompareInt3D(float4::RED);
	DoubleDownBlue = CurCollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 37))
		.CompareInt3D(float4::RED);

	Left = CurCollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() - 34))
		.CompareInt3D(float4::GREEN);
	Right = CurCollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() - 34))
		.CompareInt3D(float4::GREEN);
	Right_Up = CurCollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() + 37))
		.CompareInt3D(float4::GREEN);
	Left_Up = CurCollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() + 37))
		.CompareInt3D(float4::GREEN);
	Right_Up = CurCollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() + 37))
		.CompareInt3D(float4::GREEN);
	Right_Down = CurCollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() - 37))
		.CompareInt3D(float4::GREEN);
	Left_Down = CurCollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() - 37))
		.CompareInt3D(float4::GREEN);


	// 벽 체크
	// 땅에 박힘
	if (Down || DownBlue)
	{
		WallState = STATE_WALL::UNDERGROUND;
		IsFall = false;
		return;
	}

	// 머리 위 부딪힘
	if (Left_Up && Right_Up)
	{
		WallState = STATE_WALL::UP;
		IsFall = true;
		return;
	}

	// 공중
	if (!Down && !DoubleDown && !Left_Down && !Right_Down && !DoubleDownBlue)
	{
		WallState = STATE_WALL::NONE;
		IsFall = true;
	}

	// 왼쪽 오른쪽 벽
	if (Left_Up && Left_Down)
	{
		WallState = STATE_WALL::LEFT;
		return;
	}
	if (Right_Up && Right_Down)
	{
		WallState = STATE_WALL::RIGHT;
		return;
	}


	// 바닥 
	if (DoubleDown || DoubleDownBlue)
	{
		WallState = STATE_WALL::DOWN;
		IsFall = false;
	}

	// 슬로프
	if (Left_Down && !Right_Down)
	{
		//IsFall = false;
		WallState = STATE_WALL::LEFTSLOPE;
	}

	if (Right_Down && !Left_Down)
	{
		//IsFall = false;
		WallState = STATE_WALL::RIGHTSLOPE;
	}

}


void EnemyActor::PlayerAttackCheck()
{
	// 죽은 후에는 체크 안함
	if (Hp <= 0)
	{
		return;
	}

	Collision_Character->IsCollision(CollisionType::CT_OBB2D, COLLISIONGROUP::PLAYER_ATTACK, CollisionType::CT_OBB2D,
		std::bind(&EnemyActor::Damaged, this, std::placeholders::_1, std::placeholders::_2)
	);
}

bool EnemyActor::Damaged(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	// 플레이어의 공격 위치를 받아서 반대로 날려짐
	Hp--;
	if (Hp <= 0)
	{
		FlyVec = _This->GetTransform().GetWorldPosition() - _Other->GetTransform().GetWorldPosition();
		FlyVec.z = 0;
		FlyVec.Normalize();
		StateManager.ChangeState("Dead");
	}

	return true;
}

void EnemyActor::PlayerAlertCheck()
{
	// 죽은 후에는 체크 안함
	if (Hp <= 0 || true == FindPlayer)
	{
		return;
	}

	// 범위안에 들어오면
	Collision_ChaseSensor->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::PLAYER, CollisionType::CT_AABB2D,
		std::bind(&EnemyActor::SeePlayer, this, std::placeholders::_1, std::placeholders::_2));
}

bool EnemyActor::SeePlayer(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	// 느낌표 + 추적 시작
	FindPlayer = true;
	float4 PlayerPos = CharacterActor::CharacterPosition;
	return true;
}

void EnemyActor::LookDirCheck()
{
	if (Hp <= 0)
	{
		return;
	}

	if (MoveVec.x == PrevLookDir)
	{
		return;
	}


	if (PrevLookDir > 0)
	{
		Renderer_Character->GetTransform().PixLocalPositiveX();
	}
	else if (PrevLookDir < 0)
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
	}

}

void EnemyActor::Move(float _DeltaTime)
{



	MoveVec.z = 0;
	Velocity = MoveVec * MoveSpeed * _DeltaTime;
	GetTransform().SetWorldMove(Velocity);

	//LookDirCheck();


}


void EnemyActor::SpawnUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (WallState != STATE_WALL::DOWN)
	{
		MoveVec.y = -1.5f;
	}

	if (WallState == STATE_WALL::DOWN || WallState == STATE_WALL::UNDERGROUND)
	{
		StateManager.ChangeState("Idle");
		return;
	}

}

void EnemyActor::IdleStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("idle");

	MoveVec = float4::ZERO;
}

void EnemyActor::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (1.5f < _Info.StateTime)
	{
		StateManager.ChangeState("Walk");
		return;
	}

	if (true == FindPlayer)
	{
		StateManager.ChangeState("Alert");
		return;
	}
}

void EnemyActor::WalkStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("walk");
	MoveVec.x = PrevLookDir;
}

void EnemyActor::WalkUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (MoveVec.x > 0 && WallState == STATE_WALL::RIGHT || MoveVec.x < 0 && WallState == STATE_WALL::LEFT)
	{
		MoveVec.x = 0;
		StateManager.ChangeState("PatrolTurn");
		return;
	}

	if (true == FindPlayer)
	{
		StateManager.ChangeState("Alert");
		return;
	}
}

void EnemyActor::PatrolTurnStart(const StateInfo& _Info)
{
	// 왼쪽으로 돔
	if (PrevLookDir > 0)
	{
		Collision_ChaseSensor->GetTransform().SetLocalPosition({ -ChaseSensorPaddingX, 0 , 0 });
		Renderer_Character->GetTransform().PixLocalNegativeX();
		PrevLookDir = -1;
	}
	// 오른쪽으로 돔
	else if (PrevLookDir < 0)
	{
		Collision_ChaseSensor->GetTransform().SetLocalPosition({ ChaseSensorPaddingX, 0 , 0 });
		Renderer_Character->GetTransform().PixLocalPositiveX();
		PrevLookDir = 1;
	}
	Renderer_Character->ChangeFrameAnimation("turn");
}

void EnemyActor::PatrolTurnUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == FindPlayer)
	{
		StateManager.ChangeState("Alert");
		return;
	}

	if (true == TurnEnd)
	{
		TurnEnd = false;
		StateManager.ChangeState("Idle");
		return;
	}


}

void EnemyActor::AlertStart(const StateInfo& _Info)
{
	Renderer_Alert->On();
}

void EnemyActor::AlertUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 0.2f)
	{
		Renderer_Alert->SetTexture("spr_enemy_follow_1.png");
		StateManager.ChangeState("Run");
	}
}

void EnemyActor::RunStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("run");
}

void EnemyActor::RunUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

void EnemyActor::ChaseTurnStart(const StateInfo& _Info)
{
}

void EnemyActor::ChaseTurnUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

void EnemyActor::AttackStart(const StateInfo& _Info)
{
}

void EnemyActor::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

void EnemyActor::DeadStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("hurtfly");

	FlyVec.x *= 1.3f;
	FlyVec.y *= 1.5f;
	MoveVec = FlyVec;
	FlyRadian = float4::VectorXYtoRadian({ 0, 0 }, FlyVec);
	MoveSpeed *= 3.0f;

}

void EnemyActor::DeadUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;

	static bool IsGround = false;
	if (MoveVec.y < 0 && WallState == STATE_WALL::DOWN || WallState == STATE_WALL::LEFTSLOPE 
		|| WallState == STATE_WALL::RIGHTSLOPE || WallState == STATE_WALL::UNDERGROUND)
	{
		if (false == IsGround)
		{
			IsGround = true;
			Death(3.0f);
			Renderer_Character->ChangeFrameAnimation("hurtground");
		}
		MoveVec.y = 0;
		MoveVec.x = GameEngineMath::LerpLimit(FlyVec.x, 0, DT);
	}
	else
	{
		MoveVec.y = FlyVec.y * sinf(FlyRadian) - (9.8f * DT) / 6.0f;
	}

	// 벽과 충돌 체크
	if (WallState == STATE_WALL::LEFT || WallState == STATE_WALL::RIGHT)
	{
		MoveVec.x *= -0.3f;
	}
	
	if (WallState == STATE_WALL::UP)
	{
		MoveVec.y = -1;
	}

}






