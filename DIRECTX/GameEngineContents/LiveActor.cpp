#include "PreCompile.h"
#include "LiveActor.h"
#include <GameEngineCore/CoreMinimal.h>

float FORCE_REACTION = 0.38f;


LiveActor::LiveActor() 
	: Renderer_Character(nullptr)
	, MoveDir(float4::ZERO)
	, Down(float4::ZERO)
	, DoubleDown(float4::ZERO)
	, Up(float4::ZERO)
	, Left(float4::ZERO)
	, Right(float4::ZERO)
	, UpRight(float4::ZERO)
	, DownRight(float4::ZERO)
	, UpLeft(float4::ZERO)
	, DownLeft(float4::ZERO)
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
	Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x, -(CharacterPos.y - 31));
	DoubleDown = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x, -(CharacterPos.y - 32));
	Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x, -(CharacterPos.y + 32));
	Left = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x - 34, -(CharacterPos.y));
	Right = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x + 34, -(CharacterPos.y));

	UpRight = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x + 34, -(CharacterPos.y + 32));
	DownRight = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x + 34, -(CharacterPos.y - 32));
	UpLeft = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x - 34, -(CharacterPos.y + 32));
	DownLeft = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x - 34, -(CharacterPos.y - 32));
	
	WallState = STATE::NOWALL;

	// 공중
	float4 SumPixel = Right + Down + Left;
	if (SumPixel.CompareInt4D({ 3, 3, 3, 1 }))
	{
		WallState = STATE::NOWALL;
		return;
	}

	// 아래 벽
	if (DoubleDown.CompareInt4D({ 0, 1, 0, 0 }))
	{
		WallState = STATE::DOWN_WALL; // 지면(중력X)
		if (Down.CompareInt4D({ 0, 1, 0, 0 }))
		{
			WallState = STATE::DOWN_GROUND; // 땅에 박힘
			return;
		}
	}

	// 왼쪽 벽 또는 왼쪽 경사
	if (Velocity.x < 0 && Left.CompareInt4D({ 0, 1, 0, 0 }))
	{
		WallState = STATE::LEFT_WALL;
		if (false == UpLeft.CompareInt4D({ 0, 1, 0, 0 }))
		{
			WallState = STATE::LEFTUP_SLOPE;
		}
		else if (false == DownLeft.CompareInt4D({ 0, 1, 0, 0 }))
		{
			WallState = STATE::LEFTDOWN_SLOPE;
		}
		return;
	}

	// 오른쪽 벽 또는 오른쪽 경사
	if (Velocity.x > 0 && Right.CompareInt4D({ 0, 1, 0, 0 }))
	{
d		WallState = STATE::RIGHT_WALL;

		if (false == UpRight.CompareInt4D({ 0, 1, 0, 0 }))
		{
			WallState = STATE::RIGHTUP_SLOPE;
		}
		else if (false == DownRight.CompareInt4D({ 0, 1, 0, 0 }))
		{
			WallState = STATE::RIGHTDOWN_SLOPE;
		}
		return;
	}

	// 특수(땅 통과, 다음 스테이지 넘어가기, 위 벽)
	if (Down.CompareInt4D({ 0, 0, 1, 0 }))
	{
		WallState = STATE::DOWN_PASS;
		return;
	}

	if (Right.CompareInt4D({ 0, 0, 1, 0 })
		&& UpRight.CompareInt4D({ 0, 0, 1, 0 })
		&& DownRight.CompareInt4D({ 0, 0, 1, 0 }) 
		)
	{
		WallState = STATE::RIGHT_PASS;
		return;
	}

	if (Up.CompareInt4D({ 0, 1, 0, 0 }))
	{
		WallState = STATE::UP_WALL;
	}
}

// 중력 적용/미적용 후 속도(속력 * 방향) 측정
void LiveActor::VelocityCheck(float _DeltaTime)
{
	Velocity = MoveDir * _DeltaTime * MoveSpeed;

	switch (WallState)
	{
	case LiveActor::STATE::NOWALL:

		Velocity.y = Velocity.y - 270.0f * _DeltaTime;
		break;
	case LiveActor::STATE::UP_WALL:
		Velocity.y += -1;
		break;
	case LiveActor::STATE::RIGHT_WALL:
		Velocity.x += -1;
		break;
	case LiveActor::STATE::RIGHT_PASS:
		break;
	case LiveActor::STATE::LEFT_WALL:
		Velocity.x += 1;
		break;
	case LiveActor::STATE::DOWN_WALL:
		break;
	case LiveActor::STATE::DOWN_GROUND:
		Velocity.y += 1;
		break;
	case LiveActor::STATE::DOWN_PASS:
		break;
	case LiveActor::STATE::RIGHTUP_SLOPE:
		break;
	case LiveActor::STATE::RIGHTDOWN_SLOPE:
		Velocity.y += -1;
		break;
	case LiveActor::STATE::LEFTUP_SLOPE:
		break;
	case LiveActor::STATE::LEFTDOWN_SLOPE:
		Velocity.x = -1;
		Velocity.y = -1;
		break;
	default:
		break;
	}


}


