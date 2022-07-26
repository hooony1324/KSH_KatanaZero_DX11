#include "PreCompile.h"
#include "LiveActor.h"
#include <GameEngineCore/CoreMinimal.h>

LiveActor::LiveActor() 
	: Renderer_Character(nullptr)
	, MoveDir(float4::ZERO)
	, Pixel_Down({0, -32})
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
	float4 Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x, -(CharacterPos.y + 32) );
	float4 Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x, -(CharacterPos.y - 32) );
	float4 Left = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x - 34, -(CharacterPos.y) );
	float4 Right = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.x + 34, -(CharacterPos.y) );
	
	if (Down.CompareInt4D({ 0, 1, 0, 0 }))
	{
		WallState = STATE::DOWN_WALL;
		GetTransform().SetWorldMove({ 0, 1, 0, 0 });
		GameEngineDebug::OutPutString("Down 충돌중");
	}

	if (Down.CompareInt4D({ 0, 0, 1, 0 }))
	{
		WallState = STATE::DOWN_PASS;
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
	}

	if (Right.CompareInt4D({ 0, 1, 0, 0 }))
	{
		WallState = STATE::RIGHT_WALL;
		GetTransform().SetWorldMove({ -1, 0, 0, 0 });
		GameEngineDebug::OutPutString("Right 충돌중");
	}

	if (Right.CompareInt4D({ 0, 0, 1, 0 }))
	{
		WallState = STATE::RIGHT_PASS;
	}
}


