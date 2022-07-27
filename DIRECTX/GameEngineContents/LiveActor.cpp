#include "PreCompile.h"
#include "LiveActor.h"
#include <GameEngineCore/CoreMinimal.h>

const float FORCE_REACTION = 0.38f;
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
		MsgBoxAssert("충돌맵이 nullptr 입니다");
	}

	// y값 반전 주의
	float4 CharacterPos = GetTransform().GetWorldPosition();
	Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x, -(CharacterPos.y - 34))
		.CompareInt4D(GREEN);
	Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x, -(CharacterPos.y + 34))
		.CompareInt4D(GREEN);
	Left_Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x - 34, -(CharacterPos.y + 34))
		.CompareInt4D(GREEN);
	Right_Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x + 34, -(CharacterPos.y + 34))
		.CompareInt4D(GREEN);
	Right_Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x + 34, -(CharacterPos.y - 34))
		.CompareInt4D(GREEN);
	Left_Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x - 34, -(CharacterPos.y - 34))
		.CompareInt4D(GREEN);
	DownBlue = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x, -(CharacterPos.y - 34))
		.CompareInt4D(BLUE);
	Left = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x - 34, -(CharacterPos.y - 26))
		.CompareInt4D(GREEN);
	Right = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x + 34, -(CharacterPos.y - 26))
		.CompareInt4D(GREEN);

	WallState = STATE::NONE;

	// 바닥 확인 > 점프 확인 > 슬로프 확인, 벽 확인, 윗 벽 
	if (Down || DownBlue)
	{
		IsFly = false;
		WallState = STATE::DOWN_WALL;
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


	// 시선 우
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
	// 시선 좌
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

// 중력 적용/미적용 후 속도(속력 * 방향) 측정
void LiveActor::VelocityCheck(float _DeltaTime)
{
	switch (WallState)
	{
	case LiveActor::STATE::NONE:
		break;
	case LiveActor::STATE::UP_WALL:
		
		break;
	case LiveActor::STATE::RIGHT_WALL:
		 // 향후 벽타기 하려면 변경
		break;
	case LiveActor::STATE::RIGHT_PASS:
		break;
	case LiveActor::STATE::LEFT_WALL:
		
		break;
	case LiveActor::STATE::DOWN_WALL:
		GetTransform().SetWorldMove({ 0, 1 , 0});
		break;
	case LiveActor::STATE::DOWN_GROUND:
		break;
	case LiveActor::STATE::RIGHTUP_SLOPE:
		GetTransform().SetWorldMove({ 0, 1 , 0});
		break;
	case LiveActor::STATE::RIGHTDOWN_SLOPE:

		break;
	case LiveActor::STATE::LEFTUP_SLOPE:

		break;
	case LiveActor::STATE::LEFTDOWN_SLOPE:
		GetTransform().SetWorldMove({ 0, -1 , 0});
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
	// 왼쪽 오른쪽 바라보기
	if (Velocity.x > 0)
	{
		CurLookDir = 1;
	}
	else if (Velocity.x < 0)
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


