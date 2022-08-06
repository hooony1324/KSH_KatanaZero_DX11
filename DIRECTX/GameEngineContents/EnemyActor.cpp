#include "PreCompile.h"
#include "EnemyActor.h"
#include <GameEngineCore/CoreMinimal.h>

bool TurnEnd;

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

void EnemyActor::CreateRendererAndCollision()
{
	Renderer_Character = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Character->SetScaleModeImage();

	float4 RendererScale = Renderer_Character->GetTransform().GetLocalScale();
	Collision_Character = CreateComponent<GameEngineCollision>();
	Collision_Character->GetTransform().SetLocalScale({ RendererScale.x, RendererScale.y, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision_Character->ChangeOrder(COLLISIONGROUP::ENEMY);

	Collision_ChaseSensor = CreateComponent<GameEngineCollision>();
	Collision_ChaseSensor->GetTransform().SetLocalScale({ 400, 100, GetDepth(ACTOR_DEPTH::COLLISION) });
}

void EnemyActor::CreateAllFolderAnimation()
{
	if (EnemyName.empty())
	{
		MsgBoxAssert("Enemy�� �ڽ��� �̸��� �������� ���� ä�� �ִϸ��̼��� �����Ϸ��� �߽��ϴ�");
	}

	Renderer_Character->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ EnemyName + "_idle", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("attack", FrameAnimation_DESC{ EnemyName + "_attack", 0.05f , false });
	Renderer_Character->CreateFrameAnimationFolder("walk", FrameAnimation_DESC{ EnemyName + "_walk", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("turn", FrameAnimation_DESC{ EnemyName + "_turn", 0.1125f, false });
	Renderer_Character->CreateFrameAnimationFolder("run", FrameAnimation_DESC{ EnemyName + "_run", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("hurtfly", FrameAnimation_DESC{ EnemyName + "_hurtfly", 0.1125f, false });
	Renderer_Character->CreateFrameAnimationFolder("hurtground", FrameAnimation_DESC{ EnemyName + "_hurtground", 0.1125f, false });

	// ���ε�
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
	if (!Down && !DoubleDown && !Left_Down && !Right_Down)
	{
		WallState = STATE_WALL::NONE;
		IsFall = true;
	}

	// �ٴ� 
	if (!Down && DoubleDown)
	{
		WallState = STATE_WALL::DOWN;
		IsFall = false;
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

	// ������
	if (!IsFall && Left && Left_Down)
	{
		//IsFall = false;
		WallState = STATE_WALL::LEFTSLOPE;
	}

	if (!IsFall && Right && Right_Down)
	{
		//IsFall = false;
		WallState = STATE_WALL::RIGHTSLOPE;
	}

}

void EnemyActor::ChooseAction()
{
	// �÷��̾ Chase�Ÿ� �ۿ� �ִ� -> PATROL
	// �÷��̾ Chase�Ÿ� �ȿ� �ִ� -> Chase
	// Chase�̰� �÷��̾���� �Ÿ��� AttackRange ���̴� -> Attack






	//PrevAction = CurAction;
	//CurAction = ENEMYACTION::PATROL;
}

void EnemyActor::PlayerAttackCheck()
{
	Collision_Character->IsCollision(CollisionType::CT_OBB2D, COLLISIONGROUP::PLAYER_ATTACK, CollisionType::CT_OBB2D,
		std::bind(&EnemyActor::Damaged, this, std::placeholders::_1, std::placeholders::_2)
	);
}

bool EnemyActor::Damaged(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	// ���� �Ŀ��� üũ ����
	if (Hp <= 0)
	{
		return false;
	}

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


	// �����̴� ���߿� �극��ũ �ɾ��ٸ��� ��

	switch (WallState)
	{
	case EnemyActor::STATE_WALL::NONE:
		if (MoveVec.y >= 0 && Hp > 0)
		{
			MoveVec.y = -2;
			break;
		}
	case EnemyActor::STATE_WALL::UP:
		if (MoveVec.y >= 0)
		{
			MoveVec.y = -1;
			break;
		}
	case EnemyActor::STATE_WALL::DOWN:
		MoveVec.y = 0;
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


	MoveVec.z = 0;
	Velocity = MoveVec * MoveSpeed * _DeltaTime;
	GetTransform().SetWorldMove(Velocity);

	//LookDirCheck();
}


void EnemyActor::SpawnUpdate(float _DeltaTime, const StateInfo& _Info)
{

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
}

void EnemyActor::WalkStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("walk");
	MoveVec.x = PrevLookDir;
}

void EnemyActor::WalkUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (WallState == STATE_WALL::RIGHT || WallState == STATE_WALL::LEFT
		|| WallState == STATE_WALL::RIGHTSLOPE || WallState == STATE_WALL::LEFTSLOPE)
	{
		MoveVec.x = 0;
		StateManager.ChangeState("PatrolTurn");
	}
}

void EnemyActor::PatrolTurnStart(const StateInfo& _Info)
{
	if (PrevLookDir > 0)
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
		PrevLookDir = -1;
	}
	else if (PrevLookDir < 0)
	{
		Renderer_Character->GetTransform().PixLocalPositiveX();
		PrevLookDir = 1;
	}
	Renderer_Character->ChangeFrameAnimation("turn");
}

void EnemyActor::PatrolTurnUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == TurnEnd)
	{
		StateManager.ChangeState("Idle");
	}
}

void EnemyActor::AlertStart(const StateInfo& _Info)
{
}

void EnemyActor::AlertUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

void EnemyActor::RunStart(const StateInfo& _Info)
{
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

	MoveVec = FlyVec * 1.2f;
	FlyRadian = float4::VectorXYtoRadian({ 0, 0 }, FlyVec);
	MoveSpeed *= 1.5f;

}

void EnemyActor::DeadUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;

	static bool IsGround = false;
	if (WallState == STATE_WALL::DOWN)
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
}






