#include "PreCompile.h"
#include "LiveActor.h"
#include <GameEngineCore/CoreMinimal.h>

const float FORCE_REACTION = 1.0f; // ���ۿ� ����
const float4 GREEN = { 0, 1, 0, 0 };
const float4 BLUE = { 1, 0, 0, 0 };

LiveActor::LiveActor() 
	: Renderer_Character(nullptr)
	, MoveVec(float4::ZERO)
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
		.CompareInt3D(GREEN);
	MiddleDown = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 24))
		.CompareInt3D(GREEN);
	Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() + 34))
		.CompareInt3D(GREEN);
	Left_Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() + 34))
		.CompareInt3D(GREEN);
	Right_Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() + 34))
		.CompareInt3D(GREEN);
	Right_Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() - 34))
		.CompareInt3D(GREEN);
	Left_Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() - 34))
		.CompareInt3D(GREEN);
	DoubleDownBlue = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 35))
		.CompareInt3D(BLUE);
	Left = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() - 0))
		.CompareInt3D(GREEN);
	Right = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() - 0))
		.CompareInt3D(GREEN);
	DoubleDown = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 35))
		.CompareInt3D(GREEN);

}

void LiveActor::WallCheck()
{
	// ���ٴڿ� ����
	if (Down && DoubleDown)
	{
		GetTransform().SetWorldMove({ 0, 1, 0 });
		return;
	}

	// ��������
	if (!Down && !DoubleDown && !DoubleDownBlue)
	{
		IsFall = true;
	}

	// �� ����
	if (!Down && DoubleDown || DoubleDownBlue)
	{
		IsFall = false;
	}

	// ��
	if (Right_Up && Right_Down)
	{
		GetTransform().SetWorldMove({ -1, 0, 0 });
	}

	if (Left_Up && Left_Down)
	{
		GetTransform().SetWorldMove({ 1, 0, 0 });
	}


	if (Up)
	{
		GetTransform().SetWorldMove({ 0, -2, 0 });
	}

	if (IsJump)
	{
		return;
	}

	// ���
	// ������ �� ��
	if (Velocity.x > 0)
	{
		if (!Left_Down && DoubleDown && Right_Down)
		{
			GetTransform().SetWorldMove({ 0, 1, 0 });
		}
		else if (Left_Down && DoubleDown && !Right_Down)
		{
			GetTransform().SetWorldMove({ 0, -1, 0 });
		}
	}
	else if (Velocity.x < 0)
	{
		if (Left_Down && !DoubleDown && !Right_Down)
		{
			GetTransform().SetWorldMove({ 0, 1, 0 });
		}
		else if (!Left_Down && !DoubleDown && Right_Down)
		{
			GetTransform().SetWorldMove({ 0, -1, 0 });
		}
	}


}

void LiveActor::GravityCheck(float _DeltaTime)
{
	if (true == IsFall)
	{
		GrabityForce = float4{ 0, -9.8f, 0, 0 } * _DeltaTime * 25;
	}
	else
	{
		GrabityForce = float4::ZERO;
	}
}

void LiveActor::LookCheck(float _InputOrVelocityDir)
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


