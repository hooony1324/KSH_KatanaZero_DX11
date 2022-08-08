#include "PreCompile.h"
#include "CharacterActor.h"
#include <GameEngineCore/CoreMinimal.h>


const float FORCE_REACTION = 1.0f; // ���ۿ� ����

CharacterActor::CharacterActor()
	: Renderer_Character(nullptr)
	, MoveVec(float4::ZERO)
	, PrevLookDir(1)
	, CurLookDir(1)
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
	if (Down)
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
	if (!Down && DoubleDown || !Down && DoubleDownBlue)
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
		//IsFall = false;
		WallState = STATE_WALL::LEFTSLOPE;
	}

	if (!IsFall && Right && Right_Down)
	{
		//IsFall = false;
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


