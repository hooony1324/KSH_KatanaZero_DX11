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
	if (CapturedDataList.size() != 0)
	{
		RemoveCapturedData();
	}
}

void LiveActor::RemoveCapturedData()
{
	for (auto& Val : CapturedDataList)
	{
		Val->Texture = nullptr;
		delete Val;
	}
	CapturedDataList.clear();
}

void LiveActor::PlayReverseCapturedData()
{
	if (CapturedDataList.size() == 0)
	{
		return;
	}

	FrameCapturedData* Data = CapturedDataList.back();

	GetTransform().SetWorldPosition(Data->Position);
	FrameDataRenderer->SetTexture(Data->Texture);
	FrameDataRenderer->GetTransform().SetLocalScale(Data->TextureScale);

	// 역재생은 바로 메모리 해제
	CapturedDataList.pop_back();
	delete Data;
}


