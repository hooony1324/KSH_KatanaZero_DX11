#include "PreCompile.h"
#include "EnemyActor.h"
#include <GameEngineCore/CoreMinimal.h>

EnemyActor::EnemyActor() 
	: CurAction(ENEMYACTION::PATROL)
	, EnemyName(std::string())
{
}

EnemyActor::~EnemyActor() 
{
	
}

void EnemyActor::OnEvent()
{
	if (nullptr == Collision_ChaseSensor)
	{
		MsgBoxAssert("Enemy�� �ݸ��� ������ nullptr �Դϴ�");
	}
}

void EnemyActor::WallCheck()
{
	GameEngineTextureRenderer* CollisionMap = GlobalValueManager::ColMap;
	if (nullptr == CollisionMap)
	{
		MsgBoxAssert("�浹���� nullptr �Դϴ�");
	}


	// y�� ���� ����
	float4 CharacterPos = GetTransform().GetWorldPosition();
	Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 36))
		.CompareInt3D(float4::GREEN);
	Left_Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() + 37))
		.CompareInt3D(float4::GREEN);
	Right_Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() + 37))
		.CompareInt3D(float4::GREEN);
	Right_Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() - 37))
		.CompareInt3D(float4::GREEN);
	Left_Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() - 37))
		.CompareInt3D(float4::GREEN);
	DoubleDown = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 37))
		.CompareInt3D(float4::GREEN);

	// �� üũ
	{
		// ���� ����
		if (Down)
		{
			WallState = STATE_WALL::UNDERGROUND;
			IsFall = false;
			return;
		}

		// �Ӹ� �� �ε���
		if (Right_Up || Left_Up)
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
		if (Left_Up)
		{
			WallState = STATE_WALL::LEFT;
			return;
		}
		if (Right_Up)
		{
			WallState = STATE_WALL::RIGHT;
			return;
		}

		// ������ üũ
		if (!IsFall && Left && Left_Down)
		{
			WallState = STATE_WALL::LEFTSLOPE;
		}

		if (!IsFall && Right && Right_Down)
		{
			WallState = STATE_WALL::RIGHTSLOPE;
		}
	}
}

void EnemyActor::ChooseAction()
{
	// �÷��̾ Chase�Ÿ� �ۿ� �ִ� -> PATROL
	// �÷��̾ Chase�Ÿ� �ȿ� �ִ� -> Chase
	// Chase�̰� �÷��̾���� �Ÿ��� AttackRange ���̴� -> Attack






	//PrevAction = CurAction;
	CurAction = ENEMYACTION::PATROL;
}

void EnemyActor::Move(float _DeltaTime)
{



	switch (WallState)
	{
	case EnemyActor::STATE_WALL::NONE:
		break;
	case EnemyActor::STATE_WALL::RIGHT:
		break;
	case EnemyActor::STATE_WALL::LEFT:
		break;
	case EnemyActor::STATE_WALL::UP:
		MoveVec.y = -0.02f;
		break;
	case EnemyActor::STATE_WALL::DOWN:
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

	Velocity = MoveVec * MoveSpeed * _DeltaTime;
	GetTransform().SetWorldMove(Velocity);
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


}


