#include "PreCompile.h"
#include "EnemyGrunt.h"

const float SlashRendererPaddingX = 10.0f;
const float SlashColPaddingX = 20.0f;

EnemyGrunt::EnemyGrunt() 
{
	EnemyName = "grunt";
}

EnemyGrunt::~EnemyGrunt() 
{
}

void EnemyGrunt::Start()
{
	// �ʱ�ȭ
	CreateRendererAndCollision();
	CreateAllFolderAnimation();
	CreateAllState();

	// Attack ���� �������̵�
	Collision_ChaseSensor->GetTransform().SetLocalScale({ 300, 50, GetDepth(ACTOR_DEPTH::COLLISION) });

	StateManager.CreateStateMember("Attack"
		, std::bind(&EnemyGrunt::AttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyGrunt::AttackStart, this, std::placeholders::_1));
	
	
	Renderer_Slash = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Slash->CreateFrameAnimationFolder("slash", FrameAnimation_DESC{"grunt_slash", 0.1125f, false});
	Renderer_Slash->SetScaleModeImage();


	Renderer_Slash->Off();
	Collision_Slash = CreateComponent<GameEngineCollision>();
	Collision_Slash->GetTransform().SetLocalScale({ 25, 35 , GetDepth(ACTOR_DEPTH::COLLISION)});
	Collision_Slash->ChangeOrder(COLLISIONGROUP::ENEMY_ATTACK);
	Collision_Slash->Off();
	Collision_Slash->SetDebugSetting(CollisionType::CT_AABB2D, { 1, 1, 1, 0.25f });


	Renderer_Slash->AnimationBindEnd("slash", [=](const FrameAnimation_DESC& _Info)
		{
			Renderer_Slash->Off();
			Collision_Slash->Off();
		});
	Renderer_Character->AnimationBindFrame("attack", [=](const FrameAnimation_DESC& _Info)
		{
			if (_Info.CurFrame == 3)
			{
				Renderer_Slash->On();
				Collision_Slash->On();
				Renderer_Slash->ChangeFrameAnimation("slash");
				Renderer_Slash->CurAnimationReset();
			}
		
		});


	// �⺻ ����
	Hp = 1;
	MoveSpeed = 150.0f;
	AttackRange = 80.0f;
	IsChasingEnemy = true;
	StateManager.ChangeState("Spawn");
	GetTransform().SetLocalScale({ 2, 2, 1 });
}

void EnemyGrunt::Update(float _DeltaTime)
{

	PlayerAttackCheck();

	// ��
	WallCheck();

	// �׼� üũ
	PlayerAlertCheck();

	PlayerOnFloorCheck();
	// ���¿� ���� MoveVec ����
	StateManager.Update(_DeltaTime);

	// ������ ���� ���� MoveVec
	Move(_DeltaTime);


	EnemyActorDebug();

}

void EnemyGrunt::End()
{
}

void EnemyGrunt::AttackStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("attack");

	MoveVec.x = 0;
	MoveVec.y = 0;

	// ������ �浹ü ��ġ ���� PrevLookDir or MoveVec
	Renderer_Slash->GetTransform().SetLocalPosition({ SlashRendererPaddingX * PrevLookDir, 20, 0 });
	Collision_Slash->GetTransform().SetLocalPosition({ SlashColPaddingX * PrevLookDir, 20, 0 });

	if (PrevLookDir > 0)
	{
		Renderer_Slash->GetTransform().PixLocalPositiveX();
	}
	else
	{
		Renderer_Slash->GetTransform().PixLocalNegativeX();
	}
}

void EnemyGrunt::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (false == AttackAniEnd)
	{
		return;
	}


	if (_Info.StateTime < 1.0f)
	{
		return;
	}

	// �� ���� �� �� ����
	if (PrevLookDir == PlayerDir.ix())
	{
		AttackAniEnd = true;
		StateManager.ChangeState("Run");
	}
	else
	{
		AttackAniEnd = true;
		StateManager.ChangeState("ChaseTurn");
	}


}


