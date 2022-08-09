#include "PreCompile.h"
#include "CharacterActor.h"
#include <GameEngineCore/CoreMinimal.h>


const float FORCE_REACTION = 1.0f; // ���ۿ� ����

CharacterActor::CharacterActor()
	: Renderer_Character(nullptr)
	, MoveVec(float4::ZERO)
	, PrevLookDir(1)
	, CurLookDir(1)
	, Hp(1)
{

}

CharacterActor::~CharacterActor()
{
}

void CharacterActor::WallCheck()
{
	GameEngineTextureRenderer* CollisionMap = GlobalValueManager::ColMap;
	if (nullptr == CollisionMap)
	{
		MsgBoxAssert("�浹���� nullptr �Դϴ�");
	}

	// y�� ���� ����
	float4 CharacterPos = GetTransform().GetWorldPosition();

	PixelColor val = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() - 34));
	val == GREEN;

	Down = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() - 34))
		 == GREEN;
	DownBlue = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() - 34))
		 == BLUE;
	Up = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() + 34))
		== GREEN;
	Left_Up = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() - 34, -(CharacterPos.iy() + 34))
		== GREEN;
	Right_Up = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() + 34, -(CharacterPos.iy() + 34))
		== GREEN;
	Right_Down = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() + 34, -(CharacterPos.iy() - 35))
		== GREEN;
	Left_Down = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() - 34, -(CharacterPos.iy() - 35))
		== GREEN;
	DoubleDownBlue = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() - 35))
		== BLUE;
	Left = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() - 34, -(CharacterPos.iy() - 25))
		== GREEN;
	Right = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() + 34, -(CharacterPos.iy() - 25))
		== GREEN;
	DoubleDown = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix(), -(CharacterPos.iy() - 35))
		== GREEN;
	Right_UpBlue = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() + 34, -(CharacterPos.iy() + 20))
		== BLUE;
	Right_DownBlue = CollisionMap->GetCurTexture()->GetPixelToPixelColor(CharacterPos.ix() + 34, -(CharacterPos.iy() - 20))
		== BLUE;

	// ���� ����
	if (Down || DownBlue)
	{
		WallState = STATE_WALL::UNDERGROUND;
		IsFall = false;
		return;
	}

	// �Ӹ� �� �ε���
	if (Up)
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
	if (!Down && DoubleDown || !DownBlue && DoubleDownBlue)
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
		IsFall = false;
		WallState = STATE_WALL::LEFTSLOPE;
	}

	if (!IsFall && Right && Right_Down)
	{
		IsFall = false;
		WallState = STATE_WALL::RIGHTSLOPE;
	}

}

void CharacterActor::FloatTimeCheck(float _DeltaTime)
{
	if (true == IsFall)
	{
		FloatDeltaTime += _DeltaTime;
	}
	else
	{
		FloatDeltaTime = 0;
	}
}

void CharacterActor::LookCheck(float _InputOrVelocityDir)
{
	if (true == IsDead)
	{
		return;
	}

	// ���� ������ �ٶ󺸱�
	if (_InputOrVelocityDir > 0)
	{
		CurLookDir = 1;
	}
	else if (_InputOrVelocityDir < 0)
	{
		CurLookDir = -1;
	}

	if (PrevLookDir > 0)
	{
		Renderer_Character->GetTransform().PixLocalPositiveX();
	}
	else
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
	}

	PrevLookDir = CurLookDir;
}

void CharacterActor::EnemyAttackCheck()
{
	// ���� �Ŀ��� üũ ����
	if (Hp <= 0)
	{
		return;
	}

	Collision_Character->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::ENEMY_ATTACK, CollisionType::CT_AABB2D,
		std::bind(&CharacterActor::Damaged, this, std::placeholders::_1, std::placeholders::_2)
	);
}

bool CharacterActor::Damaged(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	// ���� ��ġ�� �޾Ƽ� �ݴ�� ������
	Hp--;
	if (Hp <= 0)
	{
		float4 This = _This->GetTransform().GetWorldPosition();
		This.z = 0;
		float4 Other = _Other->GetActor()->GetTransform().GetWorldPosition();
		Other.z = 0;
		FlyVector = This - Other;
		FlyVector.Normalize();
		PlayerStateManager.ChangeState("Dead");
	}

	return true;
}


