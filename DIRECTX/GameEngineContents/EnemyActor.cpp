#include "PreCompile.h"
#include "EnemyActor.h"
#include "EnemyBullet.h"
#include <GameEngineBase/magic_enum.hpp>

#include "CharacterActor.h"
#include "SplattedBlood.h"
#include "NoDirBlood.h"
#include "ParticleShooter.h"

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
	, ChasingPlayer(nullptr)
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
	Collision_Character->SetCollisionMode(CollisionMode::Ex);

	Collision_ChaseSensor = CreateComponent<GameEngineCollision>();
	Collision_ChaseSensor->GetTransform().SetLocalScale({ 120, 50, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision_ChaseSensor->GetTransform().SetLocalPosition({ ChaseSensorPaddingX, 18 , 0 });
	Collision_ChaseSensor->ChangeOrder(COLLISIONGROUP::ENEMY_CAHSESENSOR);
	Collision_ChaseSensor->SetDebugSetting(CollisionType::CT_AABB2D, { 0.6f, 0.6f, 0, 0.20f });

	Renderer_GunArm = CreateComponent<GameEngineTextureRenderer>();
	Renderer_GunArm->Off();

	FRenderer_WallState = CreateComponent<GameEngineFontRenderer>();
	FRenderer_WallState->SetText("default");
	FRenderer_WallState->SetLeftAndRightSort(LeftAndRightSort::CENTER);
	FRenderer_WallState->SetPositionMode(FontPositionMode::WORLD);
	FRenderer_WallState->SetSize(16);
	FRenderer_WallState->GetTransform().SetLocalPosition({ 0, 50, 0 });

	FRenderer_FSMState = CreateComponent<GameEngineFontRenderer>();
	FRenderer_FSMState->SetText("default");
	FRenderer_FSMState->SetLeftAndRightSort(LeftAndRightSort::CENTER);
	FRenderer_FSMState->SetPositionMode(FontPositionMode::WORLD);
	FRenderer_FSMState->SetSize(16);
	FRenderer_FSMState->GetTransform().SetLocalPosition({ 0, 60, 0 });

	BloodShooter = GetLevel()->CreateActor<ParticleShooter>();
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

	if (false == Collision_Character->IsUpdate())
	{
		Collision_Character->On();
	}

	if (Pixels.size() == 0)
	{
		Pixels.push_back(DownPix);
		Pixels.push_back(DownBluePix);
		Pixels.push_back(DoubleDownPix);
		Pixels.push_back(DoubleDownBluePix);
		Pixels.push_back(Left_UpPix);
		Pixels.push_back(Right_UpPix);
		Pixels.push_back(Right_DownPix);
		Pixels.push_back(Left_DownPix);
		Pixels.push_back(LeftPix);
		Pixels.push_back(RightPix);

		// Pix Debug
		//for (Pix Pixel : Pixels)
		//{
		//	GameEngineTextureRenderer* Renderer = CreateComponent<GameEngineTextureRenderer>();
		//	Renderer->SetTexture("None_yellow.png");
		//	Renderer->GetTransform().SetLocalScale({ 1, 1, 1 });
		//	Renderer->GetTransform().SetLocalMove({ static_cast<float>(Pixel.x) * 0.5f, static_cast<float>(Pixel.y) * 0.5f });
		//}
	}


	ChasingPlayer = nullptr;
}

void EnemyActor::WallCheck()
{
	// y�� ���� ����
	EnemyPos = GetTransform().GetWorldPosition();
	EnemyPos.z = 0;

	Down = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + DownPix.x, -(EnemyPos.iy() + DownPix.y))).CompareInt3D(float4::GREEN);
	DoubleDown = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + DoubleDownPix.x, -(EnemyPos.iy() + DoubleDownPix.y))).CompareInt3D(float4::GREEN);
	DownBlue = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + DownBluePix.x, -(EnemyPos.iy() + DownBluePix.y))).CompareInt3D(float4::BLUE);
	DoubleDownBlue = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + DoubleDownBluePix.x, -(EnemyPos.iy() + DoubleDownBluePix.y))).CompareInt3D(float4::BLUE);

	Left = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + LeftPix.x, -(EnemyPos.iy() + LeftPix.y))).CompareInt3D(float4::GREEN);
	Right = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + RightPix.x, -(EnemyPos.iy() + RightPix.y))).CompareInt3D(float4::GREEN);
	Right_Up = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + Right_UpPix.x, -(EnemyPos.iy() + Right_UpPix.y))).CompareInt3D(float4::GREEN);
	Left_Up = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + Left_UpPix.x, -(EnemyPos.iy() + Left_UpPix.y))).CompareInt3D(float4::GREEN);
	Right_Down = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + Right_DownPix.x, -(EnemyPos.iy() + Right_DownPix.y))).CompareInt3D(float4::GREEN);
	Left_Down = (CurCollisionMap->GetCurTexture()
		->GetPixelToFloat4(EnemyPos.ix() + Left_DownPix.x, -(EnemyPos.iy() + Left_DownPix.y))).CompareInt3D(float4::GREEN);



	// �� üũ
	// ���� ����
	if (Down && DoubleDown)
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

	// �ٴ� 
	if (!Down && DoubleDown || !DownBlue && DoubleDownBlue)
	{
		WallState = STATE_WALL::DOWN;
		IsFall = false;
	}

	// ���� ������ ��
	if (Left_Up && Left)
	{
		WallState = STATE_WALL::LEFT;
		return;
	}
	if (Right_Up && Right)
	{
		WallState = STATE_WALL::RIGHT;
		return;
	}

	// ������
	if (Left && Left_Down)
	{
		IsFall = false;
		WallState = STATE_WALL::LEFTSLOPE;
		return;
	}

	if (Right && Right_Down )
	{
		IsFall = false;
		WallState = STATE_WALL::RIGHTSLOPE;
		return;
	}


	// �����
	if (true == LiveActor::WallStateDebugOn)
	{
		FRenderer_WallState->On();
		FRenderer_FSMState->On();

		std::string_view WState = magic_enum::enum_name(WallState);
		FRenderer_WallState->SetText(WState.data());

		std::string FSMState = StateManager.GetCurStateStateName();
		FRenderer_FSMState->SetText(FSMState);
	}
	else
	{
		FRenderer_WallState->Off();
		FRenderer_FSMState->Off();
	}

}


void EnemyActor::PlayerAttackCheck()
{
	// ���� �Ŀ��� üũ ����
	if (Hp <= 0)
	{
		return;
	}

	Collision_Character->IsCollision(CollisionType::CT_OBB2D, static_cast<int>(COLLISIONGROUP::PLAYER_ATTACK), CollisionType::CT_OBB2D,
		std::bind(&EnemyActor::Damaged, this, std::placeholders::_1, std::placeholders::_2)
		, [=](GameEngineCollision* _This, GameEngineCollision* _Other) {return CollisionReturn::Break; }
		, [=](GameEngineCollision* _This, GameEngineCollision* _Other) {return CollisionReturn::Break; }
	);
}

CollisionReturn EnemyActor::Damaged(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	// �÷��̾��� ���� ��ġ�� �޾Ƽ� �ݴ�� ������
	Hp--;

	if (Hp > 0)
	{
		return CollisionReturn::Break;
	}
	// �÷��̾ �ݻ��� �Ѿ˰����̸� �Ѿ� ����

	float4 Enemy = _This->GetTransform().GetWorldPosition() + float4{ 0, 30 , 0 };
	Enemy.z = 0;
	float4 Attack = _Other->GetTransform().GetWorldPosition();
	Attack.z = 0;

	if (nullptr != dynamic_cast<EnemyBullet*>(_Other->GetActor()))
	{
		_Other->GetActor()->Death();

		FlyVec = Enemy - Attack;
		// �������� ū ��, �ּ��� ������
		float len = FlyVec.Length();
		FlyPower = 3 / (std::clamp(FlyVec.Length(), 1.0f, 2.6f));
		FlyVec.Normalize();
		StateManager.ChangeState("Hurtfly");
	}
	// �Ѿ˸��� �ٸ� �����̸�
	else
	{
		float SlashDegree = _Other->GetTransform().GetLocalRotation().z;
		float4 Dir = float4::DegreeToDirection2D(SlashDegree);
		FlyVec = Dir;
		float Normalize = float4(Enemy - Attack).Length() / 30.0f;
		FlyPower = 3 / std::clamp(Normalize, 1.0f, 3.0f);
		FlyVec.Normalize();
		StateManager.ChangeState("Hurtfly");
	}



	return CollisionReturn::Break;
}

void EnemyActor::PlayerAlertCheck()
{
	// ���� �Ŀ��� üũ ����
	if (Hp <= 0 || true == FindPlayer)
	{
		return;
	}

	// ���� ������
	IsDoor = Collision_Character->IsCollision(CollisionType::CT_AABB2D, static_cast<int>(COLLISIONGROUP::DOOR), CollisionType::CT_AABB2D,
		std::bind(&EnemyActor::DoorCheck, this, std::placeholders::_1, std::placeholders::_2), 
		[=](GameEngineCollision* _This, GameEngineCollision* _Other) {return CollisionReturn::ContinueCheck; },
		[=](GameEngineCollision* _This, GameEngineCollision* _Other) {return CollisionReturn::ContinueCheck; });

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

CollisionReturn EnemyActor::DoorCheck(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	return CollisionReturn::Break;
}

CollisionReturn EnemyActor::SeePlayer(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	// ����ǥ + ���� ����
	PlayerCollision = _Other;
	MoveVec.x = 0;
	Renderer_Character->ChangeFrameAnimation("idle");
	StateManager.ChangeState("Alert");
	FindPlayer = true;
	ChasingPlayer = dynamic_cast<CharacterActor*>(_Other->GetActor());
	return CollisionReturn::Break;
}

void EnemyActor::PlayerSameFloorCheck()
{
	if (nullptr == ChasingPlayer)
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


	float FloorPosY = Stair::PlayerNearestStair->GetTransform().GetWorldPosition().y;
	float HeightSub = PlayerPos.y - EnemyPos.y;
	float FloorSub = FloorPosY - EnemyPos.y;
	if (HeightSub > 0)
	{
		PlayerDir.y = 1;
	}
	else
	{
		PlayerDir.y = -1;
	}
	// ������ �ٸ��� �Ǵ�
	if (true == ChasingPlayer->IsGround())
	{
		// �÷��̾ ������ ���� ����
		if (HeightSub > 0 && HeightSub < 240 && abs(FloorSub) < 50)
		{
			PlayerSameFloor = true;
		}
		else if (HeightSub >= 240 || FloorSub >= 50)
		{
			PlayerSameFloor = false;
		}
		// �÷��̾ ������ �Ʒ��� ����
		else
		{
			if (abs(FloorSub) >= 230)
			{
				PlayerSameFloor = false;
			}
			else
			{
				PlayerSameFloor = true;
			}
		}
	}
	// �÷��̾� �������̶��?
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

	switch (WallState)
	{
	case EnemyActor::STATE_WALL::NONE:
	{
		if (0 != StateManager.GetCurStateStateName().compare("Hurtfly"))
		{
			Velocity.y += -MoveSpeed * _DeltaTime;
		}
		break;
	}
	case EnemyActor::STATE_WALL::RIGHT:
		break;
	case EnemyActor::STATE_WALL::LEFT:
		break;
	case EnemyActor::STATE_WALL::UP:
		break;
	case EnemyActor::STATE_WALL::DOWN:
	{
		if (Velocity.y < 0)
		{
			Velocity.y = 0;
		}

		// �߰��� �������� (���� ��ܿ� �����ؾߵ�)
		if (DoubleDownBlue && 0 == StateManager.GetCurStateStateName().compare("Run"))
		{
			int a = 0;
		}
		
	}
		
	case EnemyActor::STATE_WALL::DOWNBLUE:
		break;
	case EnemyActor::STATE_WALL::UNDERGROUND:
	{
		GetTransform().SetWorldMove(float4::UP);
		return;
	}
	case EnemyActor::STATE_WALL::RIGHTSLOPE:
	{
		float Vector = Velocity.x * 0.7f;
		Velocity = float4{ Vector, Vector, 0 };
		break;
	}
	case EnemyActor::STATE_WALL::LEFTSLOPE:
	{
		float Vector = Velocity.x * 0.7f;
		Velocity = float4{ Vector, -Vector, 0 };
		break;
	}
	default:
		break;
	}

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
		PrevLookDir = 1;
	}
	else
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
		PrevLookDir = -1;
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
		GetTransform().SetWorldMove(float4::LEFT);
	}
	// ���������� ��
	else if (PrevLookDir < 0)
	{
		Collision_ChaseSensor->GetTransform().SetLocalPosition({ ChaseSensorPaddingX, 18 , 0 });
		Renderer_Character->GetTransform().PixLocalPositiveX();
		PrevLookDir = 1;
		GetTransform().SetWorldMove(float4::RIGHT);
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
	MoveVec.x = static_cast<float>(PlayerDir.x);
	MoveVec.y = 0;
}

void EnemyActor::RunUpdate(float _DeltaTime, const StateInfo& _Info)
{


	// *�÷��̾� ���� Ȯ��(���� �� ������ ���)-> �ݴ�� ChaseTurn
	if (MoveVec.x != PlayerDir.ix() && true == PlayerSameFloor)
	{
		StateManager.ChangeState("ChaseTurn");
		return;
	}


	// ���� ���� ������ �������
	if (false == PlayerSameFloor)
	{
		CurPlayerHereStair = Stair::PlayerNearestStair;

		if (PlayerDir.y > 0)
		{
			CurPlayerHereStair->SearchEnemyPassingUpStairs(GetTransform().GetWorldPosition().y, StairsToPlayer);
			StateManager.ChangeState("GoUpstair");
		}
		else
		{
			CurPlayerHereStair->SearchEnemyPassingDownStairs(GetTransform().GetWorldPosition().y, StairsToPlayer);
			StateManager.ChangeState("GoDownstair");
		}
		return;
	}


	// ��Ÿ� �ȿ� ������ Attack
	if (true == PlayerSameFloor && abs(PlayerPos.x - EnemyPos.x) < AttackRange)
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
	if (PrevLookDir != PlayerDir.x)
	{
		StateManager.ChangeState("ChaseTurn");
		return;
	}

	if (false == TurnEnd)
	{
		return;
	}

	TurnEnd = false;
	StateManager.ChangeState("Run");
}

bool BloodSplatted;
float BloodDegree;
float SplattSumTime;
void EnemyActor::HurtflyStart(const StateInfo& _Info)
{
	MoveSpeed = 150.0f;
	Renderer_Alert->Off();
	Renderer_Character->ChangeFrameAnimation("hurtfly");
	Renderer_GunArm->Off();

	FlyVec.x *= FlyPower;
	FlyVec.y *= FlyPower;
	MoveVec = FlyVec;
	FlyRadian = float4::VectorXYtoRadian({ 0, 0 }, FlyVec);
	MoveSpeed *= 4.0f;

	BloodSplatted = false;
	BloodDegree = float4::VectorXYtoDegree(float4::RIGHT, FlyVec);
	SplattSumTime = 0.0f;
}

void EnemyActor::HurtflyUpdate(float _DeltaTime, const StateInfo& _Info)
{

	if (SplattSumTime > 0.08f)
	{
		BloodShooter->OneShot<NoDirBlood>(2, { EnemyPos.x, EnemyPos.y, GetDepth(ACTOR_DEPTH::BLOOD) }, FlyVec, 50);
		SplattSumTime = 0.0f;
	}
	SplattSumTime += _DeltaTime;

	float DT = _Info.StateTime;

	if (false == BloodSplatted && DT> 0.25f)
	{
		BloodSplatted = true;
		SplattedBlood* Blood = GetLevel()->CreateActor<SplattedBlood>(ACTORGROUP::BLOOD);
		Blood->GetTransform().SetWorldPosition({ EnemyPos.x, EnemyPos.y, GetDepth(ACTOR_DEPTH::BACKGROUND_4) });
		Blood->SpawnRandomBlood(BloodDegree);
	}

	

	// ����
	//MoveVec.y = FlyVec.y * sinf(FlyRadian) - (9.8f * DT) / 6.0f;
	MoveVec.y = MoveVec.y - (9.8f * _DeltaTime) / 6.0f;
	MoveVec.x = GameEngineMath::Lerp(MoveVec.x, 0, _DeltaTime);
	MoveSpeed = GameEngineMath::Lerp(MoveSpeed, 300, _DeltaTime);

	if (Left_Up != Right_Up)
	{
		MoveVec.x *= -0.5f;
	}

	if (Right_Up && Left_Up && (Left != Right))
	{
		MoveVec.x = 0.0f;
	}

	if (WallState == STATE_WALL::UP)
	{
		MoveVec.y = -0.1f;
	}

	if (MoveVec.y <= -1.0f)
	{
		MoveVec.y = -0.99f;
	}

	// ���� ������
	if (MoveVec.y <= 0.0f && (WallState == STATE_WALL::DOWN || WallState == STATE_WALL::UNDERGROUND ||
		WallState == STATE_WALL::LEFTSLOPE || WallState == STATE_WALL::RIGHTSLOPE))
	{
		MoveVec.y = 0;
		StateManager.ChangeState("Hurtground");
	}
}

void EnemyActor::HurtgroundStart(const StateInfo& _Info)
{
	MoveVec.y = 0;
	Renderer_Character->ChangeFrameAnimation("hurtground");

	SplattSumTime = 0.0f;
}

void EnemyActor::HurtgroundUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;

	// �� Ƣ��
	if (SplattSumTime > 0.2f && DT <= 1.5f)
	{
		BloodShooter->OneShotRandomPos<NoDirBlood>(3, { EnemyPos.x, EnemyPos.y, GetDepth(ACTOR_DEPTH::BLOOD) }, float4::UP , 50);
		SplattSumTime = 0.0f;
	}
	SplattSumTime += _DeltaTime;


	MoveVec.x = GameEngineMath::Lerp(MoveVec.x, 0, _DeltaTime * 3.0f);

	if (DT > 3.0f)
	{
		Off();
	}

	// �������� �ƴϰ� ���� �� �ε�����
	if (Left && Left_Up || Right && Right_Up)
	{
		MoveVec.x *= -0.5f;
	}
	

}

bool UpStairArrived;
void EnemyActor::GoUpstairStart(const StateInfo& _Info)
{
	if (StairsToPlayer.size() > 0)
	{
		CurDestStair = StairsToPlayer.back();
		StairsToPlayer.pop_back();
	}
	else
	{
		StateManager.ChangeState("ChaseTurn");
		return;
	}

	float4 StairPos = CurDestStair->GetTransform().GetWorldPosition();

	MoveVec.x = StairPos.x - EnemyPos.x < 0 ? -1.0f : 1.0f;

	// ������ ������ȯ
	if (MoveVec.x > 0)
	{
		Renderer_Character->GetTransform().PixLocalPositiveX();
		PrevLookDir = 1;
	}
	else
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
		PrevLookDir = -1;
	}

	UpStairArrived = false;
}

void EnemyActor::GoUpstairUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// ù ��ǥ ��ܱ��� ����(������ ���� ��), ù ��ǥ ��� ������ ��������
	if (false == UpStairArrived)
	{
		Collision_Character->IsCollisionExitBase(CollisionType::CT_AABB2D, static_cast<int>(COLLISIONGROUP::STAIR), CollisionType::CT_AABB2D,
			[=](GameEngineCollision* _This, GameEngineCollision* _Other)
			{
				bool TargetStairArrived = CurDestStair == _Other->GetActor();

				if (CurDestStair == _Other->GetActor())
				{
					// ������� �������̸�
					if (nullptr != CurDestStair->UpStair)
					{
						if (abs(CurDestStair->GetTransform().GetWorldPosition().y - CurDestStair->DownStair->GetTransform().GetWorldPosition().y)
							< 40)
						{
							StateManager.ChangeState("GoUpstair");
							return  CollisionReturn::ContinueCheck;
						}
					}
					// ������ ����̾��ٸ�
					else
					{
						MoveVec.x = PlayerDir.x;
					}

					// ������ȯ
					if (MoveVec.x > 0)
					{
						Renderer_Character->GetTransform().PixLocalPositiveX();
						PrevLookDir = 1;
					}
					else
					{
						Renderer_Character->GetTransform().PixLocalNegativeX();
						PrevLookDir = -1;
					}
					UpStairArrived = true;
				}
				else
				{
					UpStairArrived = false;
				}
				return CollisionReturn::ContinueCheck;
			}, [=](GameEngineCollision* _This, GameEngineCollision* _Other) {return CollisionReturn::Break; }
			, [=](GameEngineCollision* _This, GameEngineCollision* _Other) {return CollisionReturn::Break; });
	}

	// ��� �����ϸ� �ѹ� �� �ö󰡱�
	if (true == UpStairArrived && nullptr != CurDestStair->UpStair)
	{
		StateManager.ChangeState("GoUpstair");
		return;
	}


	// ��Ÿ� �ȿ� ������ Attack
	if (true == PlayerSameFloor && (PlayerPos - EnemyPos).Length() < AttackRange)
	{
		StairsToPlayer.clear();
		StateManager.ChangeState("Attack");
		return;
	}

	// ������ ���� ���� ��ǥ �ٲ�
	if (CurPlayerHereStair != Stair::PlayerNearestStair)
	{
		StairsToPlayer.clear();
		StateManager.ChangeState("ChaseTurn");
		return;
	}


}

bool DownStairArrived;
void EnemyActor::GoDownstairStart(const StateInfo& _Info)
{
	if (StairsToPlayer.size() > 0)
	{
		CurDestStair = StairsToPlayer.back();
		StairsToPlayer.pop_back();
	}
	else
	{
		StateManager.ChangeState("ChaseTurn");
		return;
	}

	float4 StairPos = CurDestStair->GetTransform().GetWorldPosition();
	
	MoveVec.x = StairPos.x - EnemyPos.x < 0 ? -1.0f : 1.0f;

	// ������ ������ȯ
	if (MoveVec.x > 0)
	{
		Renderer_Character->GetTransform().PixLocalPositiveX();
		PrevLookDir = 1;
	}
	else
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
		PrevLookDir = -1;
	}

	DownStairArrived = false;
}

void EnemyActor::GoDownstairUpdate(float _DeltaTime, const StateInfo& _Info)
{

	// ù ��ǥ ��ܱ��� ����(������ ���� ��), ù ��ǥ ��� ������ ��������
	if (false == DownStairArrived)
	{
		Collision_Character->IsCollisionExitBase(CollisionType::CT_AABB2D, static_cast<int>(COLLISIONGROUP::STAIR), CollisionType::CT_AABB2D,
			[=](GameEngineCollision* _This, GameEngineCollision* _Other) {return CollisionReturn::ContinueCheck; }
			, [=](GameEngineCollision* _This, GameEngineCollision* _Other) {return CollisionReturn::ContinueCheck; }
			, [=](GameEngineCollision* _This, GameEngineCollision* _Other)
			{
				bool TargetStairArrived = CurDestStair == _Other->GetActor();

				if (TargetStairArrived)
				{
					// ������� �������̸�
					if (nullptr != CurDestStair->DownStair)
					{
						if (abs(CurDestStair->GetTransform().GetWorldPosition().y - CurDestStair->DownStair->GetTransform().GetWorldPosition().y)
							< 40)
						{
							StateManager.ChangeState("GoDownstair");
							return  CollisionReturn::ContinueCheck;
						}
					}
					else
					{
						MoveVec.x = PlayerDir.x;
					}

					// ������� �Ʒ����̸� ���������ľߵ� 
					float NextStairDirX = CurDestStair->GetTransform().GetWorldPosition().x
						- CurDestStair->DownStair->GetTransform().GetWorldPosition().x > 0 ? -1.0f : 1.0f;
					MoveVec.x = NextStairDirX;
					if (MoveVec.x > 0)
					{
						Renderer_Character->GetTransform().PixLocalPositiveX();
						PrevLookDir = 1;
					}
					else
					{
						Renderer_Character->GetTransform().PixLocalNegativeX();
						PrevLookDir = -1;
					}
					DownStairArrived = true;
				}

				return CollisionReturn::ContinueCheck;
			});
	}


	// ������ ���� ���� ��ǥ �ٲ�
	if (CurPlayerHereStair != Stair::PlayerNearestStair)
	{
		StairsToPlayer.clear();
		StateManager.ChangeState("ChaseTurn");
		return;
	}


	// ������ �����ϸ� �������� ����
	if ( true == DownStairArrived 
		&&(MoveVec.x < 0 &&  !Right_Down|| MoveVec.x > 0 && !Left_Down)
		&& true == DoubleDownBlue)
	{
		StateManager.ChangeState("SlopeRun");
		return;
	}
	
}

// ��� �������⿡�� ����Ǵ� ����
bool CurDestStairArrived;
void EnemyActor::SlopeRunStart(const StateInfo& _Info)
{
	// ������� ����������
	GetTransform().SetWorldMove(float4::DOWN * 2);

	// ��ܿ� ���������� ���ο� ��� ��ǥ��
	if (nullptr != CurDestStair->DownStair)
	{
		StairsToPlayer.pop_back();
		CurDestStair = CurDestStair->DownStair;
	}
	else
	{
		StateManager.ChangeState("ChaseTurn");
		return;
	}

	CurDestStairArrived = false;
}

void EnemyActor::SlopeRunUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// ������ ����������
	if (false == CurDestStairArrived)
	{
		Collision_Character->IsCollisionExitBase(CollisionType::CT_AABB2D, static_cast<int>(COLLISIONGROUP::STAIR), CollisionType::CT_AABB2D,
			[=](GameEngineCollision* _This, GameEngineCollision* _Other)
			{
				if (CurDestStair == _Other->GetActor())
				{
					// ������� �������̸�
					if (nullptr != CurDestStair->DownStair)
					{
						if (abs(CurDestStair->GetTransform().GetWorldPosition().y - CurDestStair->DownStair->GetTransform().GetWorldPosition().y)
							< 40)
						{
							StateManager.ChangeState("GoDownstair");
							return  CollisionReturn::ContinueCheck;
						}
					}
		

					CurDestStairArrived = true;
				}
				return CollisionReturn::ContinueCheck;
			}, [=](GameEngineCollision* _This, GameEngineCollision* _Other) {return CollisionReturn::Break; }
			, [=](GameEngineCollision* _This, GameEngineCollision* _Other) {return CollisionReturn::Break; });
	}


	// ���߿� ��ǥ �ٲ�
	if (CurPlayerHereStair != Stair::PlayerNearestStair)
	{
		StairsToPlayer.clear();
		StateManager.ChangeState("ChaseTurn");
		return;
	}

	// ��Ÿ� �ȿ� ������ == �������� ������, Attack
	if (true == PlayerSameFloor && (PlayerPos - EnemyPos).Length() < AttackRange)
	{
		StairsToPlayer.clear();
		StateManager.ChangeState("Attack");
		return;
	}
}


// ����� ���� ����
void EnemyActor::PushFrameCpaturedData()
{
	// �����
	FrameCapturedData* Data = new FrameCapturedData();
	Data->Position = GetTransform().GetWorldPosition();
	Data->Texture = Renderer_Character->GetCurTexture();
	if (nullptr != Data->Texture)
	{
		Data->Texture = Renderer_Character->GetCurTexture();
		Data->TextureScale = Renderer_Character->GetTransform().GetLocalScale();
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
