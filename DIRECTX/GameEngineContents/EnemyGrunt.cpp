#include "PreCompile.h"
#include "EnemyGrunt.h"



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
	StateManager.CreateStateMember("Attack"
		, std::bind(&EnemyGrunt::AttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyGrunt::AttackStart, this, std::placeholders::_1));
	
	
	Renderer_Slash = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Slash->CreateFrameAnimationFolder("slash", FrameAnimation_DESC{"grunt_slash", 0.08f, false});
	Renderer_Slash->SetScaleModeImage();

	Renderer_Slash->AnimationBindStart("slash", [=](const FrameAnimation_DESC& _Info)
		{
			Collision_Slash->On();
		});
	Renderer_Slash->AnimationBindEnd("slash", [=](const FrameAnimation_DESC& _Info) 
		{
			Renderer_Slash->Off();
			Collision_Slash->Off();
		});

	Renderer_Slash->Off();
	Collision_Slash = CreateComponent<GameEngineCollision>();
	Collision_Slash->GetTransform().SetLocalScale({ 25, 70 , GetDepth(ACTOR_DEPTH::COLLISION)});
	Collision_Slash->ChangeOrder(COLLISIONGROUP::ENEMY_ATTACK);
	Collision_Slash->Off();

	Renderer_Character->AnimationBindFrame("attack", [=](const FrameAnimation_DESC& _Info)
		{
			if (_Info.CurFrame == 2)
			{
				Renderer_Slash->On();
				Renderer_Slash->ChangeFrameAnimation("slash");
				Renderer_Slash->CurAnimationReset();
			}
		
		});


	// �⺻ ����
	Hp = 1;
	MoveSpeed = 150.0f;
	AttackRange = 80.0f;
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

	PlayerLeftRightCheck();
	// ���¿� ���� MoveVec ����
	StateManager.Update(_DeltaTime);

	// ������ ���� ���� MoveVec
	Move(_DeltaTime);


	EnemyActorDebug();

	if (Collision_Slash->IsUpdate())
	{
		GameEngineDebug::DrawBox(Collision_Slash->GetTransform(), { 1, 1, 1, 0.3f });
	}
}

void EnemyGrunt::End()
{
}

void EnemyGrunt::AttackStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("attack");

	MoveVec.x = 0;

	// ������ �浹ü ��ġ ���� PrevLookDir or MoveVec
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
	if (PrevLookDir == PlayerDir)
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


