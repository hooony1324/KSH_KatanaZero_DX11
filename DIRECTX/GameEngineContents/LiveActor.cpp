#include "PreCompile.h"
#include "LiveActor.h"
#include <GameEngineCore/CoreMinimal.h>

LiveActor::LiveActor()
	: FrameDataRenderer(nullptr)
{
}

LiveActor::~LiveActor() 
{
	std::list<FrameCapturedData*>::iterator StartIt = CapturedDataList.begin();
	std::list<FrameCapturedData*>::iterator EndIt = CapturedDataList.end();

	for (; StartIt != EndIt; ++StartIt)
	{
		if (*StartIt != nullptr)
		{
			delete *StartIt;
			*StartIt = nullptr;
		}
	}
}


