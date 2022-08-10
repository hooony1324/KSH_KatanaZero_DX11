#include "PreCompile.h"
#include "EnemyActor.h"
#include <GameEngineCore/CoreMinimal.h>

// ��ȯ ����

bool TurnEnd;
static float ChaseSensorPaddingX = 25.0f;
bool GroundAniEnd = false;

EnemyActor::EnemyActor() 
	: EnemyName(std::string())
	, MoveVec(float4::DOWN)
	, PrevLookDir(1)
	, WallState(STATE_WALL::NONE)
	, AttackAniEnd(false)
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
	// y�� ���� ����
	float4 CharacterPos = GetTransform().GetWorldPosition();
	//Down = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() - 36))
	//	== GREEN;
	//DoubleDown = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() - 37))
	//	== GREEN;
	//DownBlue = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() - 36))
	//	== BLUE;
	//DoubleDownBlue = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() - 37))
	//	== BLUE;

	//Left = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() - 34, -(CharacterPos.iy() - 34))
	//	== GREEN;
	//Right = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() + 34, -(CharacterPos.iy() - 34))
	//	== GREEN;
	//Right_Up = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() + 34, -(CharacterPos.iy() + 37))
	//	== GREEN;
	//Left_Up = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() - 34, -(CharacterPos.iy() + 37))
	//	== GREEN;
	//Right_Up = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() + 34, -(CharacterPos.iy() + 37))
	//	== GREEN;
	//Right_Down = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() + 34, -(CharacterPos.iy() - 37))
	//	== GREEN;
	//Left_Down = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() - 34, -(CharacterPos.iy() - 37))
	//	== GREEN;

	Down = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() - 1))
		== GREEN;
	DoubleDown = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() - 2))
		== GREEN;
	DownBlue = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() - 1))
		== BLUE;
	DoubleDownBlue = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() - 2))
		== BLUE;

	Left = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() - 34, -(CharacterPos.iy() + 2))
		== GREEN;
	Right = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() + 34, -(CharacterPos.iy() + 2))
		== GREEN;
	Right_Up = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() + 34, -(CharacterPos.iy() + 72))
		== GREEN;
	Left_Up = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() - 34, -(CharacterPos.iy() + 72))
		== GREEN;
	Right_Up = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() + 34, -(CharacterPos.iy() + 72))
		== GREEN;
	Right_Down = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() + 34, -(CharacterPos.iy() - 2))
		== GREEN;
	Left_Down = CurCollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() - 34, -(CharacterPos.iy() - 2))
		== GREEN;


	// �� üũ
	// ���� ����
	if (Down || DownBlue)
	{
		WallState = STATE_WALL::UNDERGROUND;
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
		return;
	}
	if (Right_Up && Right_Down)
	{
		WallState = STATE_WALL::RIGHT;
		return;
	}


	// �ٴ� 
	if (DoubleDown || DoubleDownBlue)
	{
		WallState = STATE_WALL::DOWN;
		IsFall = false;
	}

	// ������
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
		StateManager.ChangeState("Dead");
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
		std::bind(&EnemyActor::DoorCheck, this, std::placeholders::_1, std::placeholders::_2));

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
	MoveVec.x = 0;
	Renderer_Character->ChangeFrameAnimation("idle");
	StateManager.ChangeState("Alert");
	FindPlayer = true;
	return true;
}

void EnemyActor::PlayerLeftRightCheck()
{
	if (false == FindPlayer)
	{
		return;
	}

	EnemyPos = GetTransform().GetWorldPosition();
	PlayerPos = GlobalValueManager::PlayerPos;
	

	// ������ �ٸ��� �Ǵ�
	float HeightSub = PlayerPos.y - EnemyPos.y;
	if (abs(HeightSub) > Renderer_Character->GetCurTexture()->GetScale().y)
	{

	}
	
	// Ex) �� 2��, �÷��̾� 1��
	// 1. �÷��̾� ��/�Ʒ� Ȯ��(�Ʒ��� ����) >>> 2. �Ʒ��� ���� ����Ʈ�� ���� ����� ����Ʈ Ȯ��(�װ��� �÷��̾� ����)
	// 3. ����Ʈ���� ���� �� ������ ������ �밢�� ���� ������ ���� �߰�
	
	// ������
	if (EnemyPos.x - PlayerPos.x< 0)
	{
		// Ememy(800) Player(900) �÷��̾� ������
		PlayerDir = 1;
	}
	else
	{
		PlayerDir = -1;
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
		// ��� ���� �ʱ�ȭ
		MoveSpeed = 150.0f;
		FindPlayer = false;
		Hp = 1;
		Renderer_Alert->Off();
		PrevLookDir = 1;
		Renderer_Character->GetTransform().PixLocalPositiveX();
		AttackAniEnd = false;
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
}

void EnemyActor::AlertUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 0.5f)
	{
		Renderer_Alert->SetTexture("spr_enemy_follow_1.png");
		
		// �� ���� �� �� ����
		if (PrevLookDir == PlayerDir)
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
	// *�÷��̾� ���� Ȯ��(���� �� ������ ���)-> �ݴ�� Turn
	if (PrevLookDir != PlayerDir)
	{
		StateManager.ChangeState("ChaseTurn");
		return;
	}
	
	// �߰� �˰���(����)
	// ��Ÿ� ���̸� ����
	// ���� �� LeftSlope RightSlope ������ �÷��̾� ���� �Ʒ��� üũ
	
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

void EnemyActor::ChaseTurnUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (false == TurnEnd)
	{
		return;
	}

	// �� or �� ������ߵ�
	TurnEnd = false;
	StateManager.ChangeState("Run");
}

void EnemyActor::DeadStart(const StateInfo& _Info)
{
	MoveSpeed = 150.0f;
	Renderer_Alert->Off();
	Renderer_Character->ChangeFrameAnimation("hurtfly");

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

	if (MoveVec.y < 0 && WallState == STATE_WALL::DOWN || WallState == STATE_WALL::LEFTSLOPE 
		|| WallState == STATE_WALL::RIGHTSLOPE || WallState == STATE_WALL::UNDERGROUND)
	{
		if (false == GroundAniEnd)
		{
			GroundAniEnd = true;
			Renderer_Character->ChangeFrameAnimation("hurtground");
		}
		MoveVec.y = 0;
		MoveVec.x = GameEngineMath::LerpLimit(FlyVec.x, 0, DT);
	}
	else
	{
		MoveVec.y = FlyVec.y * sinf(FlyRadian) - (9.8f * DT) / 6.0f;
	}

	// ���� �浹 üũ
	if (WallState == STATE_WALL::LEFT || WallState == STATE_WALL::RIGHT)
	{
		MoveVec.x *= -0.3f;
	}
	
	if (WallState == STATE_WALL::UP)
	{
		MoveVec.y = -1;
	}

	if (DT > 4.5f)
	{
		Off();
	}

}






