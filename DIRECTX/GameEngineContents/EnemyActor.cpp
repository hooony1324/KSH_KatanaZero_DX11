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
	Collision_ChaseSensor->ChangeOrder(COLLISIONGROUP::ENEMY_CAHSESENSOR);
	Collision_ChaseSensor->SetDebugSetting(CollisionType::CT_AABB2D, { 1, 1, 0, 0.25f });

	Renderer_GunArm = CreateComponent<GameEngineTextureRenderer>();
	Renderer_GunArm->Off();

}

void EnemyActor::CreateAllFolderAnimation()
{
	if (EnemyName.empty())
	{
		MsgBoxAssert("Enemy�� �ڽ��� �̸��� �������� ���� ä�� �ִϸ��̼��� �����Ϸ��� �߽��ϴ�");
	}

	Renderer_Character->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ EnemyName + "_idle", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("attack", FrameAnimation_DESC{ EnemyName + "_attack", 0.1125f , false });
	Renderer_Character->CreateFrameAnimationFolder("walk", FrameAnimation_DESC{ EnemyName + "_walk", 0.08f });
	Renderer_Character->CreateFrameAnimationFolder("turn", FrameAnimation_DESC{ EnemyName + "_turn", 0.08f, false });
	Renderer_Character->CreateFrameAnimationFolder("run", FrameAnimation_DESC{ EnemyName + "_run", 0.08f });
	Renderer_Character->CreateFrameAnimationFolder("hurtfly", FrameAnimation_DESC{ EnemyName + "_hurtfly", 0.1125f, false });
	Renderer_Character->CreateFrameAnimationFolder("hurtground", FrameAnimation_DESC{ EnemyName + "_hurtground", 0.1125f, false });

	// ���ε�
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
	StateManager.CreateStateMember("Hurtfly"
		, std::bind(&EnemyActor::HurtflyUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyActor::HurtflyStart, this, std::placeholders::_1));
	StateManager.CreateStateMember("Hurtground"
		, std::bind(&EnemyActor::HurtgroundUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyActor::HurtgroundStart, this, std::placeholders::_1));
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
		MsgBoxAssert("Enemy�� �ݸ��� ������ nullptr �Դϴ�");
	}

	CurCollisionMap = GlobalValueManager::ColMap;
	if (nullptr == CurCollisionMap)
	{
		MsgBoxAssert("�浹���� nullptr �Դϴ�");
	}

	StateManager.ChangeState("Spawn");


}

void EnemyActor::WallCheck()
{
	// y�� ���� ����
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



	// �� üũ
	// ���� ����
	if (Down || DownBlue && Left_Down && Right_Down)
	{
		WallState = STATE_WALL::UNDERGROUND;
		GetTransform().SetWorldMove({ 0, 1, 0 });
		IsFall = false;
		return;
	}

	// �Ӹ� �� �ε���
	if (Left_Up && Right_Up)
	{
		WallState = STATE_WALL::UP;
		IsFall = true;
		return;
	}

	// ����
	if (!Down && !DoubleDown && !Left_Down && !Right_Down && !DoubleDownBlue)
	{
		WallState = STATE_WALL::NONE;
		IsFall = true;
	}

	// ���� ������ ��
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


	// �ٴ� 
	if (!Down && DoubleDown || !DownBlue && DoubleDownBlue)
	{
		WallState = STATE_WALL::DOWN;
		IsFall = false;
	}

	// ������
	if (Left && Left_Down && DoubleDown && !Right_Down)
	{
		//IsFall = false;
		WallState = STATE_WALL::LEFTSLOPE;
	}

	if (Right && Right_Down && DoubleDown && !Left_Down)
	{
		//IsFall = false;
		WallState = STATE_WALL::RIGHTSLOPE;
	}

}


void EnemyActor::PlayerAttackCheck()
{
	// ���� �Ŀ��� üũ ����
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
	// �÷��̾��� ���� ��ġ�� �޾Ƽ� �ݴ�� ������
	Hp--;
	if (Hp <= 0)
	{
		FlyVec = _This->GetTransform().GetWorldPosition() - _Other->GetTransform().GetWorldPosition();
		FlyVec.z = 0;
		FlyVec.Normalize();
		StateManager.ChangeState("Hurtfly");
	}

	// �÷��̾ �ݻ��� �Ѿ˰����̸� �Ѿ� ����
	if (nullptr != dynamic_cast<EnemyBullet*>(_Other->GetActor()))
	{
		_Other->GetActor()->Death();
	}

	return true;
}

void EnemyActor::PlayerAlertCheck()
{
	// ���� �Ŀ��� üũ ����
	if (Hp <= 0 || true == FindPlayer)
	{
		return;
	}

	// ���� ������
	IsDoor = Collision_Character->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::DOOR, CollisionType::CT_AABB2D,
		std::bind(&EnemyActor::DoorCheck, this, std::placeholders::_1, std::placeholders::_2));

	// ���� �÷��̾� �տ� ������
	IsDoorFront = Collision_ChaseSensor->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::DOOR, CollisionType::CT_AABB2D,
		nullptr);
	if (IsDoorFront)
	{
		return;
	}

	// �����ȿ� ������
	Collision_ChaseSensor->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::PLAYER, CollisionType::CT_AABB2D,
		std::bind(&EnemyActor::SeePlayer, this, std::placeholders::_1, std::placeholders::_2));
}

bool EnemyActor::DoorCheck(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	return true;
}

bool EnemyActor::SeePlayer(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	// ����ǥ + ���� ����
	PlayerCollision = _Other;
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

	EnemyPos = GetTransform().GetWorldPosition() + float4{ 0, 30 , 0}; // Pivot�� Bot��
	EnemyPos.z = 0;
	PlayerPos = GlobalValueManager::PlayerPos;
	PlayerPos.z = 0;
	
	// ���� ������
	if (EnemyPos.x - PlayerPos.x < 0)
	{
		// Ememy(800) Player(900) �÷��̾� ������
		PlayerDir.x = 1;
	}
	else
	{
		PlayerDir.x = -1;
	}


	// ������ �ٸ��� �Ǵ�
	float HeightSub = PlayerPos.y - EnemyPos.y;
	if (abs(HeightSub) > Renderer_Character->GetCurTexture()->GetScale().y * 2.0f)
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
	// ���߿� �÷��̾� �׾�����
	if (PlayerCollision != nullptr && false == PlayerCollision->IsUpdate())
	{
		Renderer_Character->ChangeFrameAnimation("idle");
		return;
	}

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
		// ��� ���� �ʱ�ȭ
		MoveSpeed = 150.0f;
		FindPlayer = false;
		Hp = 1;
		Renderer_Alert->Off();
		PrevLookDir = 1;
		Renderer_Character->GetTransform().PixLocalPositiveX();
		AttackAniEnd = false;
		Renderer_GunArm->Off();
		Collision_ChaseSensor->On();
		PlayerCollision = nullptr;
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

	if (MoveVec.x >= 0)
	{
		Renderer_Character->GetTransform().PixLocalPositiveX();
	}
	else
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
	}
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
	// �������� ��
	if (PrevLookDir > 0)
	{
		Collision_ChaseSensor->GetTransform().SetLocalPosition({ -ChaseSensorPaddingX, 18 , 0 });
		Renderer_Character->GetTransform().PixLocalNegativeX();
		PrevLookDir = -1;
	}
	// ���������� ��
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
	Collision_ChaseSensor->Off();
}

void EnemyActor::AlertUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 0.5f)
	{
		Renderer_Alert->SetTexture("spr_enemy_follow_1.png");
		
		// �� ���� �� �� ����
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
	MoveVec.y = 0;
}

void EnemyActor::RunUpdate(float _DeltaTime, const StateInfo& _Info)
{


	// *�÷��̾� ���� Ȯ��(���� �� ������ ���)-> �ݴ�� ChaseTurn
	if (PrevLookDir != PlayerDir.ix() && true == PlayerSameFloor)
	{
		StateManager.ChangeState("ChaseTurn");
		return;
	}

	switch (WallState)
	{
	case EnemyActor::STATE_WALL::NONE:
		MoveVec.y = -1;
		break;
	case EnemyActor::STATE_WALL::DOWN:
		MoveVec.y = 0;
		break;
	case EnemyActor::STATE_WALL::DOWNBLUE:
		break;
	case EnemyActor::STATE_WALL::UNDERGROUND:
		MoveVec.y = 1;
		break;
	case EnemyActor::STATE_WALL::RIGHTSLOPE:
		break;
	case EnemyActor::STATE_WALL::LEFTSLOPE:
		break;
	default:
		break;
	}




	// ���� ���� ������ �������
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


	// ��Ÿ� �ȿ� ������ Attack
	if (abs(PlayerPos.x - EnemyPos.x) < AttackRange)
	{
		StateManager.ChangeState("Attack");
		return;
	}

	// ���̳� ���� ������
	if (MoveVec.x > 0 && WallState == STATE_WALL::RIGHT || MoveVec.x < 0 && WallState == STATE_WALL::LEFT
		|| IsDoor)
	{
		MoveVec.x = 0;
		StateManager.ChangeState("ChaseTurn");
		return;
	}


}

void EnemyActor::ChaseTurnStart(const StateInfo& _Info)
{
	MoveVec.x = 0;
	
	// �������� ��
	if (PrevLookDir > 0)
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
		PrevLookDir = -1;
	}
	// ���������� ��
	else if (PrevLookDir < 0)
	{
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


void EnemyActor::HurtflyStart(const StateInfo& _Info)
{
	MoveSpeed = 150.0f;
	Renderer_Alert->Off();
	Renderer_Character->ChangeFrameAnimation("hurtfly");
	Renderer_GunArm->Off();

	FlyVec.x *= 0.8f;
	FlyVec.y *= 1.1f;
	MoveVec = FlyVec;
	FlyRadian = float4::VectorXYtoRadian({ 0, 0 }, FlyVec);
	MoveSpeed *= 3.0f;
}

void EnemyActor::HurtflyUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;

	// ����
	MoveVec.y = FlyVec.y * sinf(FlyRadian) - (9.8f * DT) / 6.0f;

	// ���� �浹 üũ
	if (WallState == STATE_WALL::LEFT || WallState == STATE_WALL::RIGHT)
	{
		MoveVec.x *= -0.3f;
	}

	if (WallState == STATE_WALL::UP)
	{
		MoveVec.y = -1;
	}


	// ���� ������
	if (MoveVec.y < 0 && (Down || Left || Right))
	{
		StateManager.ChangeState("Hurtground");
	}
}

void EnemyActor::HurtgroundStart(const StateInfo& _Info)
{
	MoveVec.y = 0;
	Renderer_Character->ChangeFrameAnimation("hurtground");
}

void EnemyActor::HurtgroundUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// MoveVec.x -> 0����
	float DT = _Info.StateTime;

	if (DT > 3.0f)
	{
		Off();
	}

	if (Left || Right)
	{
		MoveVec.x = 0;
	}
	else
	{
		MoveVec.x = GameEngineMath::LerpLimit(FlyVec.x, 0, DT);
	}

}

// �÷��̾���� �Ÿ��� ª�� �� ����(�������� ���)
bool FindPlayerNearestStair(GameEngineCollision* _Left, GameEngineCollision* _Right)
{
	float Left = (_Left->GetTransform().GetWorldPosition() - GlobalValueManager::PlayerPos ).Length();
	float Right = (_Right->GetTransform().GetWorldPosition() - GlobalValueManager::PlayerPos).Length();

	return Left < Right;
}


void EnemyActor::GoUpstairStart(const StateInfo& _Info)
{
	StairArrived = false;

	// ũ�� 0�̸� Run
	if (GlobalValueManager::Collision_UpStairs.size() == 0)
	{
		StateManager.ChangeState("Run");
		return;
	}
	// ���� �� ��� ��
	std::list<GameEngineCollision*> Stairs;
	for (GameEngineCollision* Stair : GlobalValueManager::Collision_UpStairs)
	{
		if (abs(Stair->GetTransform().GetWorldPosition().iy() - GetTransform().GetWorldPosition().iy()) < 50)
		{
			Stairs.push_back(Stair);
		}
	}

	//// ���� ���̿� ��ġ�� ��� ������(ex. ��� �߰��� ��������)
	if (Stairs.size() == 0)
	{
		StateManager.ChangeState("SlopeRun");
		return;
	}

	// ���� �� UpStair�� �÷��̾�� ���� ���������� �� -> Sort [0]
	Stairs.sort(FindPlayerNearestStair);
	StairEntrance = Stairs.front()->GetTransform().GetWorldPosition();

	MoveVec.x = (StairEntrance.x - EnemyPos.x < 0) ? -1.0f : 1.0f;
	// �������� ��
	if (MoveVec.x > 0)
	{
		Renderer_Character->GetTransform().PixLocalPositiveX();
	}
	// ���������� ��
	else if (MoveVec.x < 0)
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
	}
}

void EnemyActor::GoUpstairUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// ��� �Ա����� ��
	if (true == Collision_Character->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::STAIR, CollisionType::CT_AABB2D,
		nullptr))
	{
		// �Ա� �����ϸ� ���� ��� ����(�¿�) Ȯ��
		// UpStair�� �ݵ�� DownStair �Ѱ��� ����Ǿ� ����
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

		MoveVec.x = NearestDownStairDest.x > 0 ? 1.0f : -1.0f;
		PrevLookDir = MoveVec.ix();
		// �������� ��
		if (PrevLookDir > 0)
		{
			Renderer_Character->GetTransform().PixLocalPositiveX();
		}
		// ���������� ��
		else if (PrevLookDir < 0)
		{
			Renderer_Character->GetTransform().PixLocalNegativeX();
		}

		StairArrived = true;
	}


	// ��� �Ա��� ���� ���ϸ�
	if (false == StairArrived)
	{
		return;
	}

	if (Right || Left)
	{
		StateManager.ChangeState("SlopeRun");
	}


}


void EnemyActor::GoDownstairStart(const StateInfo& _Info)
{
	StairArrived = false;

	// ũ�� 0�̸� Run
	if (GlobalValueManager::Collision_DownStairs.size() == 0)
	{
		StateManager.ChangeState("Run");
		return;
	}
	// ���� �� ��� ��
	std::list<GameEngineCollision*> Stairs;
	for (GameEngineCollision* Stair : GlobalValueManager::Collision_DownStairs)
	{
		if (abs(Stair->GetTransform().GetWorldPosition().iy() - GetTransform().GetWorldPosition().iy()) < 50)
		{
			Stairs.push_back(Stair);
		}
	}

	// ���� ���̿� ��ġ�� ��� ������(ex. ��� �߰��� ��������)
	if (Stairs.size() == 0)
	{
		StateManager.ChangeState("SlopeRun");
		return;
	}


	// ���� �� DownStair�� �÷��̾�� ���� ���������� �� -> Sort [0]
	Stairs.sort(FindPlayerNearestStair);
	StairEntrance = Stairs.front()->GetTransform().GetWorldPosition();

	MoveVec.x = (StairEntrance.x - EnemyPos.x < 0) ? -1.0f : 1.0f;
	// �������� ��
	if (MoveVec.x > 0)
	{
		Renderer_Character->GetTransform().PixLocalPositiveX();
	}
	// ���������� ��
	else if (MoveVec.x < 0)
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
	}
}

void EnemyActor::GoDownstairUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// ��� �Ա����� ��
	if (true == Collision_Character->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::STAIR, CollisionType::CT_AABB2D,
		nullptr))
	{
		// �Ա� �����ϸ�
		MoveVec.x = (PlayerPos.x - EnemyPos.x < 0) ? -1.0f : 1.0f;
		PrevLookDir = MoveVec.ix();
		// �������� ��
		if (PrevLookDir > 0)
		{
			Renderer_Character->GetTransform().PixLocalPositiveX();
		}
		// ���������� ��
		else if (PrevLookDir < 0)
		{
			Renderer_Character->GetTransform().PixLocalNegativeX();
		}
		StairArrived = true;

	}


	// ��� �Ա��� ���� ���ϸ�
	if (false == StairArrived)
	{
		return;
	}

	if (!DoubleDown)
	{
		StateManager.ChangeState("SlopeRun");
	}


}

void EnemyActor::SlopeRunStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("run");
	// �̹� �������� �־����� ���� ���� ��������� (����/�Ʒ���)
	if (Right || Left)
	{
		if (PlayerDir.y > 0)
		{
			if (Left)
			{
				MoveVec.x = -1;
			}
			else if (Right)
			{
				MoveVec.x = 1;
			}
		}
		// �÷��̾� �Ʒ���
		else
		{
			if (Left)
			{
				MoveVec.x = 1;
			}
			else if (Right)
			{
				MoveVec.x = -1;
			}
		}
		return;
	}

	// GODOWNSTAIR ->
	// RIGHT DOWN SLOPE
	if (!Left_Down && Right_Down)
	{
		GetTransform().SetWorldMove(float4::DOWN);
	}

	// LEFT DOWN SLOPE
	if (Left_Down && !Right_Down)
	{
		GetTransform().SetWorldMove(float4::DOWN);
	}

	// GOUPSTAIR ->
	// RIGHT/LEFT UP SLOPE
	if (Right || Left)
	{
		GetTransform().SetWorldMove(float4::UP);
	}



}

void EnemyActor::SlopeRunUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (false == (Left || Right))
	{
		MoveVec.y = 0;
		StateManager.ChangeState("Run");
		return;
	}

	if (MoveVec.x > 0)
	{
		MoveVec.y = (Right == true ? 1.0f : -1.0f);
	}
	else if (MoveVec.x < 0)
	{
		MoveVec.y = (Left == true ? 1.0f : -1.0f);
	}
	
	if (true == PlayerSameFloor && abs(PlayerPos.x - EnemyPos.x) < AttackRange)
	{
		StateManager.ChangeState("Attack");
		return;
	}


	if (true == Collision_Character->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::STAIR, CollisionType::CT_AABB2D,
		nullptr))
	{
		MoveVec.y = 0;
		StateManager.ChangeState("Run");
	}
}


// �����
void EnemyActor::PushFrameCpaturedData()
{
	// �����
	FrameCapturedData* Data = new FrameCapturedData();
	Data->Position = GetTransform().GetWorldPosition();
	Data->Texture = Renderer_Character->GetCurTexture();
	if (nullptr != Data->Texture)
	{
		Data->Texture = Renderer_Character->GetCurTexture();
		Data->TextureScale = Data->Texture->GetScale();
	}

	CapturedDataList.push_back(Data);
}

void EnemyActor::ReverseStartSetting()
{
	if (false == IsUpdate())
	{
		On();
	}

	Renderer_Character->Off();
	FrameDataRenderer->On();

	// �÷��̾� ������Ʈ ����
	IsReverse = true;
}

void EnemyActor::ReverseEndSetting()
{
	IsReverse = false;
	FrameDataRenderer->Off();
	Renderer_Character->On();
}
