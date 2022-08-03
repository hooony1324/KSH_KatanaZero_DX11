#include "PreCompile.h"
#include "Room_Factory2.h"

#include "Fan.h"

Room_Factory2::Room_Factory2() 
{
}

Room_Factory2::~Room_Factory2() 
{
}

void Room_Factory2::Start()
{
	EmptyRoomInit();

	//Background = CreateComponent<GameEngineTextureRenderer>();
	Background->SetTexture("room_factory_3.png");
	Background->ScaleToTexture();
	Background->SetPivot(PIVOTMODE::LEFTTOP);
	Background->GetTransform().SetWorldMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_0) });
	Background->Off();

	Background_Collision->SetTexture("room_factory_3_colmap.png");
	Background_Collision->ScaleToTexture();
	Background_Collision->SetPivot(PIVOTMODE::LEFTTOP);
	Background_Collision->GetTransform().SetWorldMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) });
	Background_Collision->Off();

	InitCameraClampArea();

	// 스폰위치, 지형지물 등
	PlayerSpawnPos = float4{ 156, -1147, GetDepth(ACTOR_DEPTH::PLAYER) };

	FanGuard = CreateComponent<GameEngineTextureRenderer>();
	FanGuard->SetTexture("spr_fan_fg_0.png");
	FanGuard->GetTransform().SetLocalScale(float4{ 79, 112, 1 } * 2.0f);
	FanGuard->GetTransform().SetWorldPosition({ 1423.5f, -368, GetDepth(ACTOR_DEPTH::FOREGROUND) });
	FanGuard->Off();
}

void Room_Factory2::Update(float _DeltaTime)
{
}

void Room_Factory2::End()
{
}

void Room_Factory2::Setting()
{
	GlobalValueManager::ColMap = Background_Collision;
	Background->On();
	FanGuard->On();

	// 지형지물
	PtrFan = GetLevel()->CreateActor<Fan>();
	PtrFan->GetTransform().SetWorldPosition({ 1430, -375, GetDepth(ACTOR_DEPTH::BACKGROUND_1) });
}

void Room_Factory2::Clear()
{
	Background->Off();
	Background_Collision->Off();
	GlobalValueManager::ColMap->Off();
	GlobalValueManager::ColMap = nullptr;

	// 지형지물
	PtrFan->Death();
	FanGuard->Off();
}
