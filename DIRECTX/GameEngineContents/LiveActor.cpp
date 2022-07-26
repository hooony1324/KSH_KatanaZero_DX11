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
