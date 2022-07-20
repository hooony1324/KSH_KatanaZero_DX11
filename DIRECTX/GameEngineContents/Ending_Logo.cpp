#include "PreCompile.h"
#include "Ending_Logo.h"

Ending_Logo::Ending_Logo() 
{
}

Ending_Logo::~Ending_Logo() 
{
}

void Ending_Logo::Start()
{
	GameEngineTextureRenderer* Logo = CreateComponent<GameEngineTextureRenderer>();
	Logo->SetTexture("spr_ending_title_0.png");
	Logo->GetTransform().SetLocalScale({ 236, 24 });
}

void Ending_Logo::Update(float _DeltaTime)
{
}

void Ending_Logo::End()
{
}