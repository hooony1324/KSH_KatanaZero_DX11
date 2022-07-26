#include "PreCompile.h"
#include "LiveActor.h"
#include <GameEngineCore/CoreMinimal.h>

LiveActor::LiveActor() 
	: Renderer_Character(nullptr)
	, MoveDir(float4::ZERO)
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
	Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x, -(CharacterPos.y + 32));
	Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x, -(CharacterPos.y - 32));
	Left = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x - 34, -(CharacterPos.y));
	Right = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x + 34, -(CharacterPos.y));

	UpRight = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x + 34, -(CharacterPos.y + 32));
	DownRight = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x + 34, -(CharacterPos.y - 32));
	UpLeft = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x - 34, -(CharacterPos.y + 32));
	DownLeft = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x - 34, -(CharacterPos.y - 32));
	
	if (Down.CompareInt4D({ 0, 1, 0, 0 }))
	{
		WallState = STATE::DOWN_WALL;
		GetTransform().SetWorldMove({ 0, 1, 0, 0 });
	}

	if (Up.CompareInt4D({ 0, 1, 0, 0 }))
	{
		WallState = STATE::UP_WALL;
		GetTransform().SetWorldMove({ 0, -1, 0, 0 });
	}

	if (Left.CompareInt4D({ 0, 1, 0, 0 }))
	{
		WallState = STATE::LEFT_WALL;
		GetTransform().SetWorldMove({ 1, 0, 0, 0 });
		if (false == UpLeft.CompareInt4D({ 0, 1, 0, 0 }))
		{
			GetTransform().SetWorldMove({ 0, 1, 0, 0 });
		}
		else if (false == DownLeft.CompareInt4D({ 0, 1, 0, 0 }))
		{
			GetTransform().SetWorldMove({ 0, -1, 0, 0 });
		}
	}

	// 오른쪽 벽 또는 오른쪽 경사
	if (Right.CompareInt4D({ 0, 1, 0, 0 }))
	{
		WallState = STATE::RIGHT_WALL;
		GetTransform().SetWorldMove({ -1, 0, 0, 0 });
		if (false == UpRight.CompareInt4D({ 0, 1, 0, 0 }))
		{
			GetTransform().SetWorldMove({ 0, 1, 0, 0 });
		}
		else if (false == DownRight.CompareInt4D({ 0, 1, 0, 0 }))
		{
			GetTransform().SetWorldMove({ 0, -1, 0, 0 });
		}
	}

	// 특수(땅 통과, 다음 스테이지 넘어가기)
	if (Down.CompareInt4D({ 0, 0, 1, 0 }))
	{
		WallState = STATE::DOWN_PASS;
		GetTransform().SetWorldMove({ 0, 1, 0, 0 });
	}

	if (Right.CompareInt4D({ 0, 0, 1, 0 }))
	{
		WallState = STATE::RIGHT_PASS;
	}
}

void LiveActor::FallCheck(float _DeltaTime)
{
	Velocity = MoveDir * _DeltaTime * MoveSpeed;
	if (false == Down.CompareInt4D({ 0, 1, 0, 0 }) 
		&& false == DownRight.CompareInt4D({ 0, 1, 0, 0 })
		&& false == DownLeft.CompareInt4D({ 0, 1, 0, 0 })
		)
	{
		Velocity.y += Velocity.y - 9.8f * _DeltaTime * 40;
	}

}


