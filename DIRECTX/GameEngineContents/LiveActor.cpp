#include "PreCompile.h"
#include "LiveActor.h"
#include <GameEngineCore/CoreMinimal.h>

const float FORCE_REACTION = 1.0f; // ���ۿ� ����
const float4 GREEN = { 0, 1, 0, 0 };
const float4 BLUE = { 1, 0, 0, 0 };

LiveActor::LiveActor() 
	: Renderer_Character(nullptr)
	, MoveDir(float4::ZERO)
	, PrevLookDir(1)
	, CurLookDir(1)
{

}

LiveActor::~LiveActor() 
{
}

void LiveActor::PixelCheck()
{
	GameEngineTextureRenderer* CollisionMap = GlobalValueManager::ColMap;
	if (nullptr == CollisionMap)
	{
		MsgBoxAssert("�浹���� nullptr �Դϴ�");
	}

	// y�� ���� ����
	float4 CharacterPos = GetTransform().GetWorldPosition();
	Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 34))
		.CompareInt4D(GREEN);
	MiddleDown = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 24))
		.CompareInt4D(GREEN);
	Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() + 34))
		.CompareInt4D(GREEN);
	Left_Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() + 34))
		.CompareInt4D(GREEN);
	Right_Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() + 34))
		.CompareInt4D(GREEN);
	Right_Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() - 34))
		.CompareInt4D(GREEN);
	Left_Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() - 34))
		.CompareInt4D(GREEN);
	DownBlue = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 34))
		.CompareInt4D(BLUE);
	Left = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() - 26))
		.CompareInt4D(GREEN);
	Right = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() - 26))
		.CompareInt4D(GREEN);

	WallState = STATE::NONE;

	// �ٴ� Ȯ�� > ���� Ȯ�� > ������ Ȯ��, �� Ȯ��, �� �� 
	if (Down || DownBlue)
	{
		IsFly = false;
		WallState = STATE::DOWN_WALL;
		// ���� ������
		if (!MiddleDown)
		{
			return;
		}

		if (Left && Right)
		{
			WallState = STATE::UNDER_GROUND;
			return;
		}

		if (Left_Down && Right)
		{
			WallState = STATE::UNDER_RIGHTUP_SLOPE;
			return;
		}

		if (Right_Down && Left)
		{
			WallState = STATE::UNDER_LEFTUP_SLOPE;
			return;
		}
	}
	else
	{
		IsFly = true;
		if (Up)
		{
			WallState = STATE::UP_WALL;
			return;
		}
	}


	// �ü� ��
	if (CurLookDir > 0)
	{
		if (Right && Right_Up && Right_Down)
		{
			WallState = STATE::RIGHT_WALL;
			return;
		}
		
		if (Right_Down && Right && !Right_Up && !DownBlue)
		{
			WallState = STATE::RIGHTUP_SLOPE;
			return;
		}

		if (Left_Down && !Down && !Right_Down && !Right && !DownBlue)
		{
			WallState = STATE::RIGHTDOWN_SLOPE;
			return;
		}
		
	}
	// �ü� ��
	else
	{
		if (Left && Left_Up && Left_Down)
		{
			WallState = STATE::LEFT_WALL;
			return;
		}

		if (Left_Down && Left && !Left_Up && !DownBlue)
		{
			WallState = STATE::LEFTUP_SLOPE;
			return;
		}

		if (Right_Down && !Down && !Left_Down && !Left && !DownBlue)
		{
			WallState = STATE::LEFTDOWN_SLOPE;
			return;
		}
	}


}

// �߷� ����/������ �� �ӵ�(�ӷ� * ����) ����
void LiveActor::VelocityCheck(float _DeltaTime)
{
	switch (WallState)
	{
	case LiveActor::STATE::NONE:
		break;
	case LiveActor::STATE::UP_WALL:
		GetTransform().SetWorldMove({ 0, -FORCE_REACTION * 10 , 0 });
		break;
	case LiveActor::STATE::RIGHT_WALL:
		GetTransform().SetWorldMove({ -FORCE_REACTION * 2, 0 , 0 });
		break;
	case LiveActor::STATE::RIGHT_PASS:
		break;
	case LiveActor::STATE::LEFT_WALL:
		GetTransform().SetWorldMove({ FORCE_REACTION * 2, 0 , 0 });
		break;
	case LiveActor::STATE::DOWN_WALL:
		GetTransform().SetWorldMove({ 0, FORCE_REACTION , 0});
		break;
	case LiveActor::STATE::UNDER_GROUND:
		GetTransform().SetWorldMove({ 0, FORCE_REACTION * 10.0f , 0 });
		break;
	case LiveActor::STATE::RIGHTUP_SLOPE:
		GetTransform().SetWorldMove({ 0, FORCE_REACTION , 0});
		break;
	case LiveActor::STATE::RIGHTDOWN_SLOPE:
		GetTransform().SetWorldMove({ 0, -FORCE_REACTION , 0 });
		break;
	case LiveActor::STATE::LEFTUP_SLOPE:
		GetTransform().SetWorldMove({ 0, FORCE_REACTION , 0 });
		break;
	case LiveActor::STATE::LEFTDOWN_SLOPE:
		GetTransform().SetWorldMove({ 0, -FORCE_REACTION , 0});
		break;
	case LiveActor::STATE::UNDER_LEFTUP_SLOPE:
		GetTransform().SetWorldMove({ 0.7f, 0.7f , 0 });
		break;
	case LiveActor::STATE::UNDER_RIGHTUP_SLOPE:
		GetTransform().SetWorldMove({ -0.7f, 0.7f , 0 });
		break;
	default:
		break;
	}

	Velocity = MoveDir * _DeltaTime * MoveSpeed;

	if (IsFly)
	{
		Velocity.y = Velocity.y - 270.0f * _DeltaTime;
	}

	GameEngineDebug::OutPutString(std::to_string((int)WallState));
	LookCheck();
}

void LiveActor::LookCheck()
{
	// ���� ������ �ٶ󺸱�
	if (Velocity.x > 0)
	{
		CurLookDir = 1;
	}
	else if (Velocity.x < 0)
	{
		CurLookDir = -1;
	}

	// ������ ���� ����
	if (CurLookDir == PrevLookDir)
	{
		return;
	}
	else if (CurLookDir > 0)
	{
		Renderer_Character->GetTransform().PixLocalPositiveX();
	}
	else
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
	}
	PrevLookDir = CurLookDir;
}


