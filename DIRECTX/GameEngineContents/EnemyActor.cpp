#include "PreCompile.h"
#include "EnemyActor.h"
#include "EnemyBullet.h"
#include <GameEngineBase/magic_enum.hpp>

#include "CharacterActor.h"

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

	Collision_ChaseSensor = CreateComponent<GameEngineCollision>();
	Collision_ChaseSensor->GetTransform().SetLocalScale({ 120, 50, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision_ChaseSensor->GetTransform().SetLocalPosition({ ChaseSensorPaddingX, 18 , 0 });
	Collision_ChaseSensor->ChangeOrder(COLLISIONGROUP::ENEMY_CAHSESENSOR);
	Collision_ChaseSensor->SetDebugSetting(CollisionType::CT_AABB2D, { 0.6f, 0.6f, 0, 0.20f });

	Renderer_GunArm = CreateComponent<GameEngineTextureRenderer>();
	Renderer_GunArm->Off();

	FRenderer_WallState = CreateComponent<GameEngineFontRenderer>();
	FRenderer_WallState->SetText("fonttest");
	FRenderer_WallState->SetLeftAndRightSort(LeftAndRightSort::CENTER);
	FRenderer_WallState->SetPositionMode(FontPositionMode::WORLD);
	FRenderer_WallState->SetSize(20);
	FRenderer_WallState->GetTransform().SetLocalPosition({ 0, 50, 0 });
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
		MsgBoxAssert("Enemy의 콜리전 센서가 nullptr 입니다");
	}

	CurCollisionMap = GlobalValueManager::ColMap;
	if (nullptr == CurCollisionMap)
	{
		MsgBoxAssert("충돌맵이 nullptr 입니다");
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
		for (Pix Pixel : Pixels)
		{
			GameEngineTextureRenderer* Renderer = CreateComponent<GameEngineTextureRenderer>();
			Renderer->SetTexture("None_yellow.png");
			Renderer->GetTransform().SetLocalScale({ 1, 1, 1 });
			Renderer->GetTransform().SetLocalMove({ static_cast<float>(Pixel.x) * 0.5f, static_cast<float>(Pixel.y) * 0.5f });
		}
	}


	ChasingPlayer = nullptr;
}

void EnemyActor::WallCheck()
{
	// y값 반전 주의
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



	// 벽 체크
	// 땅에 박힘
	if (Down && DoubleDown)
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

	// 바닥 
	if (!Down && DoubleDown || !DownBlue && DoubleDownBlue)
	{
		WallState = STATE_WALL::DOWN;
		IsFall = false;
	}

	// 왼쪽 오른쪽 벽
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

	// 슬로프
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


	// 디버깅
	if (true == LiveActor::WallStateDebugOn)
	{
		FRenderer_WallState->On();
		std::string_view State = magic_enum::enum_name(WallState);
		FRenderer_WallState->SetText(State.data());
	}
	else
	{
		FRenderer_WallState->Off();
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

CollisionReturn EnemyActor::Damaged(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	// 플레이어의 공격 위치를 받아서 반대로 날려짐
	Hp--;

	if (Hp > 0)
	{
		return CollisionReturn::Break;
	}
	// 플레이어가 반사한 총알공격이면 총알 없앰

	float4 Enemy = _This->GetTransform().GetWorldPosition() + float4{ 0, 30 , 0 };
	Enemy.z = 0;
	float4 Attack = _Other->GetTransform().GetWorldPosition();
	Attack.z = 0;

	if (nullptr != dynamic_cast<EnemyBullet*>(_Other->GetActor()))
	{
		_Other->GetActor()->Death();

		FlyVec = Enemy - Attack;
		FlyPower = std::clamp(FlyVec.Length(), 1.0f, 3.0f);
		FlyVec.Normalize();
		StateManager.ChangeState("Hurtfly");
	}
	// 총알말고 다른 공격이면
	else
	{
		float SlashDegree = _Other->GetTransform().GetLocalRotation().z;
		float4 Dir = float4::DegreeToDirection2D(SlashDegree);
		FlyVec = Dir;
		FlyPower = std::clamp(float4(Enemy - Attack).Length(), 1.0f, 3.0f);
		FlyVec.Normalize();
		StateManager.ChangeState("Hurtfly");
	}



	return CollisionReturn::Break;
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

CollisionReturn EnemyActor::DoorCheck(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	return CollisionReturn::Break;
}

CollisionReturn EnemyActor::SeePlayer(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	// 느낌표 + 추적 시작
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
	if (abs(HeightSub) > 30.0f 
		&& true == ChasingPlayer->IsGround())
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
	// 도중에 플레이어 죽었으면
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

		// 추격중 내려가기 (먼저 계단에 도달해야됨)
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
		// 모든 설정 초기화
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
	Collision_ChaseSensor->Off();
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
	MoveVec.y = 0;
}

void EnemyActor::RunUpdate(float _DeltaTime, const StateInfo& _Info)
{


	// *플레이어 방향 확인(같은 층 인지도 고려)-> 반대면 ChaseTurn
	if (PrevLookDir != PlayerDir.ix() && true == PlayerSameFloor)
	{
		StateManager.ChangeState("ChaseTurn");
		return;
	}




	// 같은 층에 없으면 계단으로
	if (false == PlayerSameFloor)
	{
		CurDestEndStair = Stair::PlayerNearestStair;

		if (PlayerDir.y > 0)
		{
			StateManager.ChangeState("GoUpstair");
		}
		else
		{
			CurDestEndStair->SearchEnemyPassingDownStairs(GetTransform().GetWorldPosition().y, StairsToPlayer);
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


}

void EnemyActor::ChaseTurnStart(const StateInfo& _Info)
{
	MoveVec.x = 0;
	
	// 왼쪽으로 돔
	if (PrevLookDir > 0)
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
		PrevLookDir = -1;
	}
	// 오른쪽으로 돔
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

	FlyVec.x *= FlyPower;
	FlyVec.y *= FlyPower;
	MoveVec = FlyVec;
	FlyRadian = float4::VectorXYtoRadian({ 0, 0 }, FlyVec);
	MoveSpeed *= 4.0f;
}

void EnemyActor::HurtflyUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;

	// 날라감
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

	// 땅에 닿으면
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
}

void EnemyActor::HurtgroundUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// MoveVec.x -> 0으로
	float DT = _Info.StateTime;

	MoveVec.x = GameEngineMath::Lerp(MoveVec.x, 0, _DeltaTime * 3.0f);

	if (DT > 3.0f)
	{
		Off();
	}

	// 슬로프는 아니고 양쪽 벽 부딪히면
	if (Left && Left_Up || Right && Right_Up)
	{
		MoveVec.x *= -0.5f;
	}
	

}

void EnemyActor::GoUpstairStart(const StateInfo& _Info)
{
	int a = 0;
}

void EnemyActor::GoUpstairUpdate(float _DeltaTime, const StateInfo& _Info)
{



}


void EnemyActor::GoDownstairStart(const StateInfo& _Info)
{
	if (StairsToPlayer.size() > 0)
	{
		CurDestStair = StairsToPlayer.back();
		StairsToPlayer.pop_back();
	}
	else
	{
		StateManager.ChangeState("Run");
		return;
	}

	float4 StairPos = CurDestStair->GetTransform().GetWorldPosition();
	
	MoveVec.x = StairPos.x - EnemyPos.x < 0 ? -1.0f : 1.0f;

	// 렌더러 방향전환
}

void EnemyActor::GoDownstairUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// 첫 목표 계단까지 간다, 첫 목표 계단 다음은 무조건 슬로프임
	// 계단 도착했으면 남아있는 계단 확인한다
	// 남아있는 계단 있으면 changestate(godownstair)
	// 남아있는 계단 없으면 changestate(run)
	// 현재최종목표계단 달라졌으면 changestate(run)



	/*if (CurDestStair != Stair::PlayerNearestStair)
	{
		StateManager.ChangeState("Run");
		return;
	}*/
}

void EnemyActor::SlopeRunStart(const StateInfo& _Info)
{



}

void EnemyActor::SlopeRunUpdate(float _DeltaTime, const StateInfo& _Info)
{

}


// 역재생
void EnemyActor::PushFrameCpaturedData()
{
	// 역재생
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

	// 플레이어 업데이트 중지
	IsReverse = true;
}

void EnemyActor::ReverseEndSetting()
{
	IsReverse = false;
	FrameDataRenderer->Off();
	Renderer_Character->On();
}
