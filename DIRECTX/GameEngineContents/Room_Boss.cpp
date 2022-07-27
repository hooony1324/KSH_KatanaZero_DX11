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
	EmptyRoomInit();

	//Background = CreateComponent<GameEngineTextureRenderer>();
	Background->SetTexture("spr_psychboss_background_0.png");
	Background->GetTransform().SetLocalScale({ 1280, 720 });
	Background->GetTransform().SetWorldPosition({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_0)});
	Background->Off();

	Background_Collision->SetTexture("room_factory_3_colmap.png");
	Background_Collision->ScaleToTexture();
	Background_Collision->SetPivot(PIVOTMODE::LEFTTOP);
	Background_Collision->GetTransform().SetWorldMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) });
	Background_Collision->Off();

	// 스폰위치, 지형지물 등
	PlayerSpawnPos = float4{ 40, -560, GetDepth(ACTOR_DEPTH::PLAYER) };
}

void Room_Boss::Update(float _DeltaTime)
{
}

void Room_Boss::End()
{
}

void Room_Boss::Setting()
{
	Background->On();
	Background_Collision->On();
}

void Room_Boss::Clear()
{
	Background->Off();
	Background_Collision->Off();
}
