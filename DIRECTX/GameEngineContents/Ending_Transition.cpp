#include "PreCompile.h"
#include "Ending_Transition.h"
#include <GameEngineCore/CoreMinimal.h>

Ending_Transition::Ending_Transition() 
{
}

Ending_Transition::~Ending_Transition() 
{
}

void Ending_Transition::Start()
{
	Transition = CreateComponent<GameEngineTextureRenderer>();
	Transition->SetTexture("spr_title_background_black.png");
	Transition->GetTransform().SetLocalScale({ 1280, 1440, 1 });
}

void Ending_Transition::Update(float _DeltaTime)
{
	float Alpha = GameEngineMath::LerpLimit(1, 0, GetAccTime() * 0.5f);

	Transition->GetPixelData().MulColor.a = Alpha ;
}

void Ending_Transition::End()
{
}
