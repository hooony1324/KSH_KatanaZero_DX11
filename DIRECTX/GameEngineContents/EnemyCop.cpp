#include "PreCompile.h"
#include "EnemyCop.h"
#include "EnemyBullet.h"

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
	Collision_ChaseSensor->GetTransform().SetLocalScale({ 300, 50, GetDepth(ACTOR_DEPTH::COLLISION) });

	StateManager.CreateStateMember("Attack"
		, std::bind(&EnemyCop::AttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyCop::AttackStart, this, std::placeholders::_1));


	// �⺻ ����
	Hp = 1;
	MoveSpeed = 150.0f;
	AttackRange = 500.0f;
	ChaseSensorPaddingX = 100.0f;
	IsChasingEnemy = false;
	StateManager.ChangeState("Spawn");
	GetTransform().SetLocalScale({ 2, 2, 1 });

	// Aim -> Shoot
	Renderer_GunArm->SetTexture("spr_cop_aim_rightarm.png");
	Renderer_GunArm->ScaleToTexture();
	Renderer_GunArm->GetTransform().SetLocalPosition({ 0, 20, 0 });
	Renderer_GunArm->Off();

	AttackStateManager.CreateStateMember("Aim"
		, std::bind(&EnemyCop::AimUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyCop::AimStart, this, std::placeholders::_1));

	AttackStateManager.CreateStateMember("Shoot"
		, std::bind(&EnemyCop::ShootUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyCop::ShootStart, this, std::placeholders::_1));

	Collision_Aim->On();
}

void EnemyCop::Update(float _DeltaTime)
{
	PlayerAttackCheck();

	// ��
	WallCheck();

	// �׼� üũ
	PlayerAlertCheck();

	PlayerLeftRightCheck();
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

	AttackStateManager.ChangeState("Aim");
}

void EnemyCop::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// �÷��̾� �����Ÿ� ���̶�� Run
	if (abs((EnemyPos - PlayerPos).Length()) > AttackRange)
	{
		Renderer_GunArm->Off();
		StateManager.ChangeState("Run");
		return;
	}

	// Aim -> Shoot
	AttackStateManager.Update(_DeltaTime);
}

void EnemyCop::AimStart(const StateInfo& _Info)
{
	Renderer_GunArm->On();
	if (PrevLookDir > 0)
	{
		Renderer_GunArm->GetTransform().PixLocalPositiveX();
	}
	else
	{
		Renderer_GunArm->GetTransform().PixLocalNegativeX();
	}

}

void EnemyCop::AimUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// �����ϰ� 
	AimDir = (PlayerPos - EnemyPos).NormalizeReturn();

	// �߰��� �÷��̾ �ݴ�� �Ѿ�� ���������� �ٲ�

	// �߰��� ��ֹ� ������ Run����

	// ���� �ð� �� �߻�
	if (_Info.StateTime > 0.8f)
	{
		AttackStateManager.ChangeState("Shoot");
		return;
	}

}


Bullet* Blt;
void EnemyCop::ShootStart(const StateInfo& _Info)
{
	Blt = GetLevel()->CreateActor<EnemyBullet>();
	
	Blt->Instance(EnemyPos + float4{ 0, 30, GetDepth(ACTOR_DEPTH::FX)}, AimDir);
}

void EnemyCop::ShootUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// �ѹ� ��� ��Ÿ��
	if (_Info.StateTime > 1.0f)
	{
		AttackStateManager.ChangeState("Aim");
	}
}

// RAY??
bool EnemyCop::PointToPlayer()
{


	return false;
}

