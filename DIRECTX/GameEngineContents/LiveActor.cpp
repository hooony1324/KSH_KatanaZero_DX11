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

void LiveActor::PixelCheck(GameEngineTextureRenderer* _ColMap, float4 _Pos)
{
	auto val = GlobalValueManager::ColMap;
	float4 PlayerPos = GetTransform().GetWorldPosition();
	float4 Pixel = GlobalValueManager::ColMap->GetCurTexture()->GetPixel(PlayerPos.x, -PlayerPos.y);
	if (Pixel.CompareInt3D({ 0, 0, 0 }))
	{
		int a = 0;
	}
}


