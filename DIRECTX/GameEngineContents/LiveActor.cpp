#include "PreCompile.h"
#include "LiveActor.h"
#include <GameEngineCore/CoreMinimal.h>

const float FORCE_REACTION = 1.0f; // 반작용 강도
const float4 GREEN = { 0, 1, 0, 1 };
const float4 BLUE = { 1, 0, 0, 1 };

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
		MsgBoxAssert("충돌맵이 nullptr 입니다");
	}

	// y값 반전 주의
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
	Right_Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() - 35))
		.CompareInt3D(GREEN);
	Left_Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() - 35))
		.CompareInt3D(GREEN);
	DoubleDownBlue = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 35))
		.CompareInt3D(BLUE);
	Left = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() - 15))
		.CompareInt3D(GREEN);
	Right = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() - 15))
		.CompareInt3D(GREEN);
	DoubleDown = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 35))
		.CompareInt3D(GREEN);

}

void LiveActor::WallCheck()
{
	// 땅에 박힘
	if (Down)
	{
		WallState = STATE_WALL::UNDERGROUND;
		IsFall = false;
		return;
	}

	// 머리 위 부딪힘
	if (Up)
	{
		WallState = STATE_WALL::UP;
		IsFall = true;
		return;
	}

	// 공중
	if (!Down && !DoubleDown && !Left_Down && !Right_Down)
	{
		WallState = STATE_WALL::NONE;
		IsFall = true;
	}

	// 바닥 
	if (!Down && DoubleDown || !Down && DoubleDownBlue)
	{
		WallState = STATE_WALL::DOWN;
		IsFall = false;
	}

	// 왼쪽 오른쪽 벽
	if (Left && Left_Up)
	{
		WallState = STATE_WALL::LEFT;
		return;
	}
	if (Right && Right_Up)
	{
		WallState = STATE_WALL::RIGHT;
		return;
	}

	// 슬로프 체크
	if (Left && Left_Down)
	{
		IsFall = false;
		WallState = STATE_WALL::LEFTSLOPE;
	}

	if (Right && Right_Down)
	{
		IsFall = false;
		WallState = STATE_WALL::RIGHTSLOPE;
	}

}

void LiveActor::FloatTimeCheck(float _DeltaTime)
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

void LiveActor::LookCheck(float _InputOrVelocityDir)
{
	// 왼쪽 오른쪽 바라보기
	if (_InputOrVelocityDir > 0)
	{
		CurLookDir = 1;
	}
	else if (_InputOrVelocityDir < 0)
	{
		CurLookDir = -1;
	}

	// 렌더러 방향 결정
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


