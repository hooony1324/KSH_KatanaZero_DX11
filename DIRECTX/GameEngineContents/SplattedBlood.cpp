#include "PreCompile.h"
#include "SplattedBlood.h"
#include <GameEngineCore/CoreMinimal.h>
#include <GameEngineBase/GameEngineRandom.h>
void SplattedBlood::SpawnRandomBlood(float _Degree)
{
	int Index = GameEngineRandom::MainRandom.RandomInt(0, 4);
	Renderer->SetFolderTextureToIndex("spr_splattedblood", Index);
	Renderer->SetScaleRatio(2.0f);
	Renderer->ScaleToTexture();
	Renderer->GetPixelData().MulColor.a = 0.6f;

	Renderer->GetTransform().SetLocalRotation({ 0, 0, _Degree - 40.0f });
}

SplattedBlood::SplattedBlood()
{
}

SplattedBlood::~SplattedBlood() 
{
}

void SplattedBlood::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();

	Renderer->SetFolderTextureToIndex("spr_splattedblood", 0);
	Renderer->SetScaleRatio(3.0f);
	Renderer->ScaleToTexture();
	Renderer->GetPixelData().MulColor.a = 0.6f;
	
}

void SplattedBlood::Update(float _DeltaTime)
{
	
}

void SplattedBlood::End()
{
}
