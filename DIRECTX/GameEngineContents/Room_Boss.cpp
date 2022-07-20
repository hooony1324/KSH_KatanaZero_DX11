#include "PreCompile.h"
#include "Room_Boss.h"

Room_Boss::Room_Boss() 
{
}

Room_Boss::~Room_Boss() 
{
}

void Room_Boss::Start()
{
	Background = CreateComponent<GameEngineTextureRenderer>();
	Background->SetTexture("spr_psychboss_bg_1_0.png");
	Background->ScaleToTexture();
}

void Room_Boss::Update(float _DeltaTime)
{
}

void Room_Boss::End()
{
}
