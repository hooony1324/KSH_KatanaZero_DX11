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

	Background_ColMap->SetTexture("room_factory_3_colmap.png");
	Background_ColMap->ScaleToTexture();
	Background_ColMap->SetPivot(PIVOTMODE::LEFTTOP);
	Background_ColMap->GetTransform().SetWorldMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) });
	Background_ColMap->Off();

	InitCameraClampArea();

	// 스폰위치, 지형지물 등
	PlayerSpawnPos = float4{ 156, -1147, GetDepth(ACTOR_DEPTH::PLAYER) };

	FanGuard = CreateComponent<GameEngineTextureRenderer>();
	FanGuard->SetTexture("spr_fan_fg_0.png");
	FanGuard->GetTransform().SetLocalScale(float4{ 79, 112, 1 } * 2.0f);
	FanGuard->GetTransform().SetWorldPosition({ 1423.5f, -368, GetDepth(ACTOR_DEPTH::FOREGROUND) });
	FanGuard->Off();

	// 세팅 시간
	TimeLimit = true;
	SetCurTimeLimitSeconds(30.0f);
}

void Room_Factory2::Update(float _DeltaTime)
{
}

void Room_Factory2::End()
{
}

void Room_Factory2::Setting()
{
	GlobalValueManager::ColMap = Background_ColMap;
	Background->On();
	FanGuard->On();

	// 지형지물
	PtrFan = GetLevel()->CreateActor<Fan>(ACTORGROUP::TIMEGROUP_FAN);
	PtrFan->GetTransform().SetWorldPosition({ 1430, -375, GetDepth(ACTOR_DEPTH::BACKGROUND_1) });

}

void Room_Factory2::Clear()
{
	Background->Off();
	Background_ColMap->Off();

	if (nullptr != GlobalValueManager::ColMap)
	{
		GlobalValueManager::ColMap->Off();
	}
	

	// 지형지물
	PtrFan->Death();
	FanGuard->Off();
}
