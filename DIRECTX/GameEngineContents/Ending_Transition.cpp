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
	Transition->GetTransform().SetLocalScale({ 1280, 1440 });
}

void Ending_Transition::Update(float _DeltaTime)
{
	Transition->GetColorData().MulColor.a -= _DeltaTime / 2;
}

void Ending_Transition::End()
{
}
