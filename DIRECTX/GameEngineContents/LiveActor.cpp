#include "PreCompile.h"
#include "LiveActor.h"
#include <GameEngineCore/CoreMinimal.h>

LiveActor::LiveActor()
	: FrameDataRenderer(nullptr)
	, IsReverse(false)
{
}

LiveActor::~LiveActor() 
{
	for (auto& Val : CapturedDataList)
	{
		Val->Texture = nullptr;
		delete Val;
	}
	CapturedDataList.clear();
}


