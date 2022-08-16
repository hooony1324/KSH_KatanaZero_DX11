#include "PreCompile.h"
#include "EnemyActor.h"
#include "EnemyBullet.h"


bool TurnEnd;
bool GroundAniEnd = false;

bool StairArrived;
float4 StairEntrance;

EnemyActor::EnemyActor() 
	: EnemyName(std::string())
	, MoveVec(float4::DOWN)
	, PrevLookDir(1)
	, WallState(STATE_WALL::NONE)
	, AttackAniEnd(false)
	, ChaseSensorPaddingX(25.0f)
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

}

void EnemyActor::CreateRendererAndCollision()
{

	Renderer_Character = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Character->SetPivotToVector({ 0, 18 });
	Renderer_Character->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));

	Renderer_Alert = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Alert->SetTexture("spr_enemy_follow_0.png");
	Renderer_Alert->SetPivot(PIVOTMODE::BOT);
	Renderer_Alert->ScaleToTexture();
	Renderer_Alert->SetSamplingModePoint();
	Renderer_Alert->GetTransform().SetLocalPosition({ 0, 45, 0 });
	Renderer_Alert->Off();

	Collision_Character = CreateComponent<GameEngineCollision>();
	Collision_Character->GetTransform().SetLocalScale({ 30, 36, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision_Character->GetTransform().SetLocalPosition({ 0, 18, 0 });
	Collision_Character->ChangeOrder(COLLISIONGROUP::ENEMY);
	Collision_Character->SetDebugSetting(CollisionType::CT_AABB2D, { 1, 0, 0, 0.25f });

	Collision_ChaseSensor = CreateComponent<GameEngineCollision>();
	Collision_ChaseSensor->GetTransform().SetLocalScale({ 120, 50, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision_ChaseSensor->GetTransform().SetLocalPosition({ ChaseSensorPaddingX, 18 , 0 });
	Collision_ChaseSensor->ChangeOrder(COLLISIONGROUP::ENEMY);
	Collision_ChaseSensor->SetDebugSetting(CollisionType::CT_AABB2D, { 1, 1, 0, 0.25f });

	//Collision_Aim = CreateComponent<GameEngineCollision>();
	//Collision_Aim->GetTransform().SetLocalScale({ 300, 2, GetDepth(ACTOR_DEPTH::COLLISION) });
	//AimPaddingX = Collision_Aim->GetTransform().GetLocalScale().hx();
	//Collision_Aim->GetTransform().SetLocalPosition({ AimPaddingX, 18, 0 });
	//Collision_Aim->ChangeOrder(COLLISIONGROUP::ENEMY_AIM);
	//Collision_Aim->SetDebugSetting(CollisionType::CT_OBB2D, { 0, 1, 0, 0.5f });
	//Collision_Aim->Off();

	Renderer_GunArm = CreateComponent<GameEngineTextureRenderer>();
}

void EnemyActor::CreateAllFolderAnimation()
{
	if (EnemyName.empty())
	{
		MsgBoxAssert("Enemy가 자신의 이름을 설정하지 않을 채로 애니메이션을 생성하려고 했습니다");
	}

	Renderer_Character->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ EnemyName + "_idle", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("attack", FrameAnimation_DESC{ EnemyName + "_attack", 0.1125f , false });
	Renderer_Character->CreateFrameAnimationFolder("walk", FrameAnimation_DESC{ EnemyName + "_walk", 0.08f });
	Renderer_Character->CreateFrameAnimationFolder("turn", FrameAnimation_DESC{ EnemyName + "_turn", 0.08f, false });
	Renderer_Character->CreateFrameAnimationFolder("run", FrameAnimation_DESC{ EnemyName + "_run", 0.08f });
	Renderer_Character->CreateFrameAnimationFolder("hurtfly", FrameAnimation_DESC{ EnemyName + "_hurtfly", 0.1125f, false });
	Renderer_Character->CreateFrameAnimationFolder("hurtground", FrameAnimation_DESC{ EnemyName + "_hurtground", 0.1125f, false });

	// 바인드
	Renderer_Character->AnimationBindStart("turn", [=](const FrameAnimation_DESC&) { TurnEnd = false; });
	Renderer_Character->AnimationBindEnd("turn", [=](const FrameAnimation_DESC&) { TurnEnd = true; });
	Renderer_Character->AnimationBindStart("attack", [=](const FrameAnimation_DESC&) { AttackAniEnd = false; });
	Renderer_Character->AnimationBindEnd("attack", [=](const FrameAnimation_DESC&) { AttackAniEnd = true; });

	Renderer_Character->SetScaleModeImage();
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
	StateManager.CreateStateMember("ChaseTurn"
		, std::bind(&EnemyActor::ChaseTurnUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyActor::ChaseTurnStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("GoUpstair"
		, std::bind(&EnemyActor::GoUpstairUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyActor::GoUpstairStart, this, std::placeholders::_1));
	StateManager.CreateStateMember("GoDownstair"
		, std::bind(&EnemyActor::GoDownstairUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyActor::GoDownstairStart, this, std::placeholders::_1));
	StateManager.CreateStateMember("SlopeRun"
		, std::bind(&EnemyActor::SlopeRunUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyActor::SlopeRunStart, this, std::placeholders::_1));

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
	EnemyPos = GetTransform().GetWorldPosition();

	Down = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix(), -(EnemyPos.iy() - 1))).CompareInt3D(float4::GREEN);
	DoubleDown = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix(), -(EnemyPos.iy() - 2))).CompareInt3D(float4::GREEN);
	DownBlue = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix(), -(EnemyPos.iy() - 1))).CompareInt3D(float4::BLUE);
	DoubleDownBlue = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix(), -(EnemyPos.iy() - 2))).CompareInt3D(float4::BLUE);

	Left = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() - 20, -(EnemyPos.iy() - 1))).CompareInt3D(float4::GREEN);
	Right = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + 20, -(EnemyPos.iy() - 1))).CompareInt3D(float4::GREEN);
	Right_Up = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + 20, -(EnemyPos.iy() + 72))).CompareInt3D(float4::GREEN);
	Left_Up = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() - 20, -(EnemyPos.iy() + 72))).CompareInt3D(float4::GREEN);
	Right_Up = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + 20, -(EnemyPos.iy() + 72))).CompareInt3D(float4::GREEN);
	Right_Down = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + 20, -(EnemyPos.iy() - 2))).CompareInt3D(float4::GREEN);
	Left_Down = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() - 20, -(EnemyPos.iy() - 2))).CompareInt3D(float4::GREEN);



	// 벽 체크
	// 땅에 박힘
	if (Down || DownBlue && Left_Down && Right_Down)
	{
		WallState = STATE_WALL::UNDERGROUND;
		GetTransform().SetWorldMove({ 0, 1, 0 });
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
		GetTransform().SetWorldMove({ 1, 0, 0 });
		return;
	}
	if (Right_Up && Right_Down)
	{
		WallState = STATE_WALL::RIGHT;
		GetTransform().SetWorldMove({ -1, 0, 0 });
		return;
	}


	// 바닥 
	if (DoubleDown || DoubleDownBlue)
	{
		WallState = STATE_WALL::DOWN;
		IsFall = false;
	}

	// 슬로프
	if (Left && Left_Down && !Down && !Right_Down)
	{
		//IsFall = false;
		WallState = STATE_WALL::LEFTSLOPE;
	}

	if (Right && Right_Down && !Down && !Left_Down)
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

	// 플레이어가 반사한 총알공격이면 총알 없앰
	if (nullptr != dynamic_cast<EnemyBullet*>(_Other->GetActor()))
	{
		_Other->GetActor()->Death();
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

	// 문에 막히면
	IsDoor = Collision_Character->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::DOOR, CollisionType::CT_AABB2D,
		std::bind(&EnemyActor::DoorCheck, this, std::placeholders::_1, std::placeholders::_2));

	// 문이 플레이어 앞에 있으면
	IsDoorFront = Collision_ChaseSensor->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::DOOR, CollisionType::CT_AABB2D,
		nullptr);
	if (IsDoorFront)
	{
		return;
	}

	// 범위안에 들어오면
	Collision_ChaseSensor->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::PLAYER, CollisionType::CT_AABB2D,
		std::bind(&EnemyActor::SeePlayer, this, std::placeholders::_1, std::placeholders::_2));
}

bool EnemyActor::DoorCheck(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	return true;
}

bool EnemyActor::SeePlayer(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	// 느낌표 + 추적 시작
	MoveVec.x = 0;
	Renderer_Character->ChangeFrameAnimation("idle");
	StateManager.ChangeState("Alert");
	FindPlayer = true;
	return true;
}

void EnemyActor::PlayerOnFloorCheck()
{
	if (false == FindPlayer)
	{
		return;
	}

	EnemyPos = GetTransform().GetWorldPosition() + float4{ 0, 30 , 0}; // Pivot이 Bot임
	EnemyPos.z = 0;
	PlayerPos = GlobalValueManager::PlayerPos;
	PlayerPos.z = 0;
	
	// 왼쪽 오른쪽
	if (EnemyPos.x - PlayerPos.x < 0)
	{
		// Ememy(800) Player(900) 플레이어 오른쪽
		PlayerDir.x = 1;
	}
	else
	{
		PlayerDir.x = -1;
	}


	// 같은층 다른층 판단
	float HeightSub = PlayerPos.y - EnemyPos.y;
	if (abs(HeightSub) > Renderer_Character->GetCurTexture()->GetScale().y * 3.0f)
	{
		PlayerSameFloor = false;
		if (HeightSub > 0)
		{
			PlayerDir.y = 1;
		}
		else
		{
			PlayerDir.y = -1;
		}
	}
	else
	{
		PlayerSameFloor = true;
	}
	
	

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
		// 모든 설정 초기화
		MoveSpeed = 150.0f;
		FindPlayer = false;
		Hp = 1;
		Renderer_Alert->Off();
		PrevLookDir = 1;
		Renderer_Character->GetTransform().PixLocalPositiveX();
		AttackAniEnd = false;
		Renderer_GunArm->Off();
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
	if (false == IsChasingEnemy)
	{
		return;
	}

	if (1.5f < _Info.StateTime)
	{
		StateManager.ChangeState("Walk");
		return;
	}
}

void EnemyActor::WalkStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("walk");
	MoveVec.x = static_cast<float>(PrevLookDir);
}

void EnemyActor::WalkUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (MoveVec.x > 0 && WallState == STATE_WALL::RIGHT || MoveVec.x < 0 && WallState == STATE_WALL::LEFT
		|| IsDoor)
	{
		MoveVec.x = 0;
		StateManager.ChangeState("PatrolTurn");
		return;
	}
}

void EnemyActor::PatrolTurnStart(const StateInfo& _Info)
{
	// 왼쪽으로 돔
	if (PrevLookDir > 0)
	{
		Collision_ChaseSensor->GetTransform().SetLocalPosition({ -ChaseSensorPaddingX, 18 , 0 });
		Renderer_Character->GetTransform().PixLocalNegativeX();
		PrevLookDir = -1;
	}
	// 오른쪽으로 돔
	else if (PrevLookDir < 0)
	{
		Collision_ChaseSensor->GetTransform().SetLocalPosition({ ChaseSensorPaddingX, 18 , 0 });
		Renderer_Character->GetTransform().PixLocalPositiveX();
		PrevLookDir = 1;
	}
	Renderer_Character->ChangeFrameAnimation("turn");
}

void EnemyActor::PatrolTurnUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == TurnEnd)
	{
		TurnEnd = false;
		StateManager.ChangeState("Idle");
		return;
	}
}

void EnemyActor::AlertStart(const StateInfo& _Info)
{
	MoveSpeed *= 2.0f;
	Renderer_Alert->On();
}

void EnemyActor::AlertUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 0.5f)
	{
		Renderer_Alert->SetTexture("spr_enemy_follow_1.png");
		
		// 턴 할지 뛸 지 결정
		if (PrevLookDir == PlayerDir.ix())
		{
			StateManager.ChangeState("Run");
		}
		else
		{
			StateManager.ChangeState("ChaseTurn");
		}

	}
}

void EnemyActor::RunStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("run");
	MoveVec.x = static_cast<float>(PrevLookDir);
}

void EnemyActor::RunUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// *플레이어 방향 확인(같은 층 인지도 고려)-> 반대면 ChaseTurn
	if (PrevLookDir != PlayerDir.ix() && true == PlayerSameFloor)
	{
		StateManager.ChangeState("ChaseTurn");
		return;
	}

	if (WallState == STATE_WALL::UNDERGROUND)
	{
		GetTransform().SetWorldMove({ 0, 1, 0 });
	}


	// 같은 층에 없으면 계단으로
	if (false == PlayerSameFloor)
	{
		if (PlayerDir.y > 0)
		{
			StateManager.ChangeState("GoUpstair");
		}
		else
		{
			StateManager.ChangeState("GoDownstair");
		}
		return;
	}


	// 사거리 안에 들어오면 Attack
	if (abs(PlayerPos.x - EnemyPos.x) < AttackRange)
	{
		StateManager.ChangeState("Attack");
		return;
	}

	// 벽이나 문에 막히면
	if (MoveVec.x > 0 && WallState == STATE_WALL::RIGHT || MoveVec.x < 0 && WallState == STATE_WALL::LEFT
		|| IsDoor)
	{
		MoveVec.x = 0;
		StateManager.ChangeState("ChaseTurn");
		return;
	}

	if (WallState == STATE_WALL::NONE)
	{
		MoveVec.y = -1;
		return;
	}

	if (WallState == STATE_WALL::DOWN)
	{
		MoveVec.y = 0;
		return;
	}


}

void EnemyActor::ChaseTurnStart(const StateInfo& _Info)
{
	MoveVec.x = 0;
	
	// 왼쪽으로 돔
	if (PrevLookDir > 0)
	{
		Collision_ChaseSensor->GetTransform().SetLocalPosition({ -ChaseSensorPaddingX, 18 , 0 });
		Renderer_Character->GetTransform().PixLocalNegativeX();
		PrevLookDir = -1;
	}
	// 오른쪽으로 돔
	else if (PrevLookDir < 0)
	{
		Collision_ChaseSensor->GetTransform().SetLocalPosition({ ChaseSensorPaddingX, 18 , 0 });
		Renderer_Character->GetTransform().PixLocalPositiveX();
		PrevLookDir = 1;
	}
	Renderer_Character->ChangeFrameAnimation("turn");
}

void EnemyActor::ChaseTurnUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (false == TurnEnd)
	{
		return;
	}

	TurnEnd = false;
	StateManager.ChangeState("Run");
}

void EnemyActor::DeadStart(const StateInfo& _Info)
{
	MoveSpeed = 150.0f;
	Renderer_Alert->Off();
	Renderer_Character->ChangeFrameAnimation("hurtfly");
	Renderer_GunArm->Off();

	FlyVec.x *= 1.3f;
	FlyVec.y *= 1.5f;
	MoveVec = FlyVec;
	FlyRadian = float4::VectorXYtoRadian({ 0, 0 }, FlyVec);
	MoveSpeed *= 3.0f;

	GroundAniEnd = false;
}

void EnemyActor::DeadUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;
	if (DT > 4.0f)
	{
		Off();
	}

	if (MoveVec.y < 0 && WallState == STATE_WALL::DOWN)
	{
		if (false == GroundAniEnd)
		{
			GroundAniEnd = true;
			Renderer_Character->ChangeFrameAnimation("hurtground");
		}
		MoveVec.y = 0;
		MoveVec.x = GameEngineMath::LerpLimit(FlyVec.x, 0, DT);

		if (WallState == STATE_WALL::UNDERGROUND)
		{
			GetTransform().SetWorldMove({ 0, 1, 0 });
		}

		if (WallState == STATE_WALL::LEFTSLOPE || WallState == STATE_WALL::RIGHTSLOPE)
		{
			MoveVec.y = MoveVec.x;
			return;
		}

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

// 플레이어와의 거리가 짧은 순 정렬
bool FindPlayerNearestStair(GameEngineCollision* _Left, GameEngineCollision* _Right)
{
	float Left = (_Left->GetTransform().GetWorldPosition() - GlobalValueManager::PlayerPos ).Length();
	float Right = (_Right->GetTransform().GetWorldPosition() - GlobalValueManager::PlayerPos).Length();

	return Left < Right;
}

void EnemyActor::GoUpstairStart(const StateInfo& _Info)
{
	StairArrived = false;

	// 크기 0이면 Run
	if (GlobalValueManager::Collision_UpStairs.size() == 0)
	{
		StateManager.ChangeState("Run");
		return;
	}
	// 같은 층 계단 고름
	std::list<GameEngineCollision*> Stairs;
	for (GameEngineCollision* Stair : GlobalValueManager::Collision_UpStairs)
	{
		if (abs(Stair->GetTransform().GetWorldPosition().iy() - GetTransform().GetWorldPosition().iy()) < 50)
		{
			Stairs.push_back(Stair);
		}
	}

	// 같은 높이에 위치한 계단 없으면(ex. 계단 중간에 서있으면)
	if (Stairs.size() == 0)
	{
		StateManager.ChangeState("SlopeRun");
		return;
	}

	// 같은 층 UpStair중 플레이어와 가장 가까운곳으로 감 -> Sort [0]
	Stairs.sort(FindPlayerNearestStair);
	StairEntrance = Stairs.front()->GetTransform().GetWorldPosition();

	MoveVec.x = (StairEntrance.x - EnemyPos.x < 0) ? -1.0f : 1.0f;
}

void EnemyActor::GoUpstairUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// 계단 입구까지 감
	if (true == Collision_Character->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::STAIR, CollisionType::CT_AABB2D,
		nullptr))
	{
		// 입구 도착하면 위층 계단 방향 확인
		float4 NearestDownStairDest;
		float MinDownStairDist = 10000;
		for (GameEngineCollision* Stair : GlobalValueManager::Collision_DownStairs)
		{
			float4 CurDownStairDest = Stair->GetTransform().GetWorldPosition() - EnemyPos;
			float CurDownStairDist = CurDownStairDest.Length();
			if (CurDownStairDist < MinDownStairDist)
			{
				MinDownStairDist = CurDownStairDist;
				NearestDownStairDest = CurDownStairDest;
			}
		}

		MoveVec.x = NearestDownStairDest.x > 0 ? 1 : -1;
		PrevLookDir = MoveVec.x;
		// 왼쪽으로 돔
		if (PrevLookDir > 0)
		{
			Collision_ChaseSensor->GetTransform().SetLocalPosition({ -ChaseSensorPaddingX, 18 , 0 });
			Renderer_Character->GetTransform().PixLocalPositiveX();
		}
		// 오른쪽으로 돔
		else if (PrevLookDir < 0)
		{
			Collision_ChaseSensor->GetTransform().SetLocalPosition({ ChaseSensorPaddingX, 18 , 0 });
			Renderer_Character->GetTransform().PixLocalNegativeX();
		}

		StairArrived = true;
	}


	// 계단 입구에 도착 안하면
	if (false == StairArrived)
	{
		return;
	}

	if (WallState == STATE_WALL::UNDERGROUND)
	{
		StateManager.ChangeState("SlopeRun");
	}

	
	


}


void EnemyActor::GoDownstairStart(const StateInfo& _Info)
{
	StairArrived = false;

	// 크기 0이면 Run
	if (GlobalValueManager::Collision_DownStairs.size() == 0)
	{
		StateManager.ChangeState("Run");
		return;
	}
	// 같은 층 계단 고름
	std::list<GameEngineCollision*> Stairs;
	for (GameEngineCollision* Stair : GlobalValueManager::Collision_DownStairs)
	{
		if (abs(Stair->GetTransform().GetWorldPosition().iy() - GetTransform().GetWorldPosition().iy()) < 50)
		{
			Stairs.push_back(Stair);
		}
	}

	// 같은 높이에 위치한 계단 없으면(ex. 계단 중간에 서있으면)
	if (Stairs.size() == 0)
	{
		StateManager.ChangeState("SlopeRun");
		return;
	}


	// 같은 층 DownStair중 플레이어와 가장 가까운곳으로 감 -> Sort [0]
	Stairs.sort(FindPlayerNearestStair);
	StairEntrance = Stairs.front()->GetTransform().GetWorldPosition();

	MoveVec.x = (StairEntrance.x - EnemyPos.x < 0) ? -1.0f : 1.0f;
}

void EnemyActor::GoDownstairUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// 계단 입구까지 감
	if (true == Collision_Character->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::STAIR, CollisionType::CT_AABB2D,
		nullptr))
	{
		// 입구 도착하면
		PrevLookDir = (PlayerPos.x - EnemyPos.x < 0) ? -1.0f : 1.0f;
		MoveVec.x = PrevLookDir;
		// 왼쪽으로 돔
		if (PrevLookDir > 0)
		{
			Collision_ChaseSensor->GetTransform().SetLocalPosition({ -ChaseSensorPaddingX, 18 , 0 });
			Renderer_Character->GetTransform().PixLocalPositiveX();
		}
		// 오른쪽으로 돔
		else if (PrevLookDir < 0)
		{
			Collision_ChaseSensor->GetTransform().SetLocalPosition({ ChaseSensorPaddingX, 18 , 0 });
			Renderer_Character->GetTransform().PixLocalNegativeX();
		}
		StairArrived = true;

	}


	// 계단 입구에 도착 안하면
	if (false == StairArrived)
	{
		return;
	}


	if (!DoubleDown)
	{
		GetTransform().SetWorldMove({ 0, -1, 0 });
	}


	if (WallState == STATE_WALL::RIGHTSLOPE || WallState == STATE_WALL::LEFTSLOPE)
	{
		StateManager.ChangeState("SlopeRun");
	}




}

// 슬로프 도중에 멈췄다가 달리는 상태
void EnemyActor::SlopeRunStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("run");

	switch (WallState)
	{
	case EnemyActor::STATE_WALL::RIGHTSLOPE:
		MoveVec = MoveVec.x < 0 ? float4{ -1, -1, 0 }.NormalizeReturn() : float4{ 1, 1, 0 }.NormalizeReturn();
		break;
	case EnemyActor::STATE_WALL::LEFTSLOPE:
		MoveVec = MoveVec.x < 0 ? float4{ -1, 1, 0}.NormalizeReturn() : float4{ 1, -1, 0 }.NormalizeReturn();
		break;
	default:
		break;
	}
}

void EnemyActor::SlopeRunUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (WallState == STATE_WALL::RIGHTSLOPE || WallState == STATE_WALL::LEFTSLOPE)
	{
		return;
	}


	if (true == Collision_Character->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::STAIR, CollisionType::CT_AABB2D,
		nullptr))
	{
		StateManager.ChangeState("Run");
	}
}
