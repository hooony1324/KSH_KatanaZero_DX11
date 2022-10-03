#include "PreCompile.h"
#include "EnemyCop.h"
#include "EnemyBullet.h"

#include "ShotSpark.h"
#include "PlayLevel.h"

EnemyCop::EnemyCop() 
{
	EnemyName = "cop";
}

EnemyCop::~EnemyCop() 
{
}

void EnemyCop::Start()
{
	CreateRendererAndCollision();
	CreateAllFolderAnimation();
	CreateAllState();

	// Attack���� �������̵�
	Collision_ChaseSensor->GetTransform().SetLocalScale({ 450, 50, GetDepth(ACTOR_DEPTH::COLLISION) });

	StateManager.CreateStateMember("Attack"
		, std::bind(&EnemyCop::AttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyCop::AttackStart, this, std::placeholders::_1));


	// �⺻ ����
	Hp = 1;
	MoveSpeed = 150.0f;
	AttackRange = 550.0f;
	ChaseSensorPaddingX = 60.0f;
	Collision_ChaseSensor->GetTransform().SetLocalPosition({ ChaseSensorPaddingX, 18 , 0 });
	IsChasingEnemy = false;
	StateManager.ChangeState("Spawn");
	GetTransform().SetLocalScale({ 2, 2, 1 });

	// Aim -> Shoot
	Renderer_GunArm->SetTexture("spr_cop_aim_arms.png");
	Renderer_GunArm->ScaleToTexture();
	Renderer_GunArm->SetSamplingModePoint();
	Renderer_GunArm->GetTransform().SetLocalPosition({ 0, 20, 0 });
	Renderer_GunArm->Off();

	AttackStateManager.CreateStateMember("Aim"
		, std::bind(&EnemyCop::AimUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyCop::AimStart, this, std::placeholders::_1));

	AttackStateManager.CreateStateMember("Shoot"
		, std::bind(&EnemyCop::ShootUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyCop::ShootStart, this, std::placeholders::_1));

	// �����
	FrameDataRenderer = CreateComponent<GameEngineTextureRenderer>();
	FrameDataRenderer->SetPivot(PIVOTMODE::BOT);
	FrameDataRenderer->Off();
}

void EnemyCop::Update(float _DeltaTime)
{
	// �����
	if (true == IsReverse)
	{
		return;
	}

	PlayerAttackCheck();

	// ��
	WallCheck();

	// �׼� üũ
	PlayerAlertCheck();

	PlayerSameFloorCheck();
	// ���¿� ���� MoveVec ����
	StateManager.Update(_DeltaTime);

	// ������ ���� ���� MoveVec
	Move(_DeltaTime);


	//EnemyActorDebug();

	
}

void EnemyCop::End()
{
}

void EnemyCop::AttackStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("attack");
	MoveVec.x = 0;
	MoveVec.y = 0;

	if (PrevLookDir > 0)
	{
		Renderer_GunArm->GetTransform().PixLocalPositiveX();
	}
	else
	{
		Renderer_GunArm->GetTransform().PixLocalNegativeX();
	}
	AttackStateManager.ChangeState("Aim");
}

void EnemyCop::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// Aim -> Shoot
	AttackStateManager.Update(_DeltaTime);
}

void EnemyCop::AimStart(const StateInfo& _Info)
{
	Renderer_GunArm->On();

}

void EnemyCop::AimUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// ���� 
	AimDir = (PlayerPos - EnemyPos).NormalizeReturn();
	ShotDegree = float4::VectorXYtoDegree(EnemyPos, PlayerPos);
	float GunDegree = ShotDegree;
	if (AimDir.x > 0)
	{
		Renderer_Character->GetTransform().PixLocalPositiveX();
		Renderer_GunArm->GetTransform().PixLocalPositiveX();

	}
	else if (AimDir.x < 0)
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
		Renderer_GunArm->GetTransform().PixLocalNegativeX();

		GunDegree -= 178.9f;
	}

	Renderer_GunArm->GetTransform().SetWorldRotation({ 0, 0, GunDegree });
	


	// �߰��� ��ֹ� ������ Run����

	// ���� �ð� �� �߻�
	if (_Info.StateTime > 0.8f)
	{
		AttackStateManager.ChangeState("Shoot");
		return;
	}

}


void EnemyCop::ShootStart(const StateInfo& _Info)
{
	SoundPlayer = GameEngineSound::SoundPlayControl("sound_gun_fire_1.wav");
	SoundPlayer.Volume(0.1f);

	PlayLevel::PlayLevelInst->ShakeRoom(false);

	Bullet* Blt = GetLevel()->CreateActor<EnemyBullet>(static_cast<int>(ACTORGROUP::TIMEGROUP_PARTICLE));
	float4 GunPos = Renderer_GunArm->GetTransform().GetWorldPosition();

	float4 ShotPoint = GunPos + float4{ 0, 0, GetDepth(ACTOR_DEPTH::FX) } + AimDir * 70.0f;
	Blt->Instance(ShotPoint, AimDir);

	ShotSpark* Spark = GetLevel()->CreateActor<ShotSpark>();
	Spark->GetTransform().SetWorldPosition(ShotPoint);
	Spark->GetTransform().SetWorldRotation({ 0, 0, ShotDegree });

	Renderer_GunArm->Off();
	Renderer_Character->ChangeFrameAnimation("idle");
}

void EnemyCop::ShootUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// �ѹ� ��� ��Ÿ��
	if (_Info.StateTime > 1.0f)
	{
		AttackAniEnd = true;
		StateManager.ChangeState("Run");
		return;
	}
}



