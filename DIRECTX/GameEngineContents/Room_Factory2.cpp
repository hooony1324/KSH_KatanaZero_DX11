#include "PreCompile.h"
#include "Room_Factory2.h"

#include "Fan.h"
#include "EnemyGrunt.h"

Room_Factory2::Room_Factory2() 
{
}

Room_Factory2::~Room_Factory2() 
{
}

void Room_Factory2::Start()
{
	EmptyRoomInit();

	Background->SetTexture("room_factory_3.png");
	Background->ScaleToTexture();
	Background->SetPivot(PIVOTMODE::LEFTTOP);
	Background->GetTransform().SetWorldMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_0) });
	Background->Off();

	Background_ColMap->SetTexture("room_factory_3_colmap.png");
	Background_ColMap->ScaleToTexture();
	Background_ColMap->SetPivot(PIVOTMODE::LEFTTOP);
	Background_ColMap->GetTransform().SetWorldMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) });
	Background_ColMap->GetPixelData().MulColor.a = 0.5f;
	Background_ColMap->Off();

	InitCameraClampArea();

	// 스폰위치, 지형지물 등
	PlayerSpawnPos = float4{ 156, -1147, GetDepth(ACTOR_DEPTH::PLAYER) };

	FanGuard = CreateComponent<GameEngineTextureRenderer>();
	FanGuard->SetTexture("spr_fan_fg_0.png");
	FanGuard->GetTransform().SetLocalScale(float4{ 79, 112, 1 } * 2.0f);
	FanGuard->GetTransform().SetWorldPosition({ 1423.5f, -368, GetDepth(ACTOR_DEPTH::FOREGROUND) });
	FanGuard->Off();

	// Fan
	PtrFan = GetLevel()->CreateActor<Fan>();
	PtrFan->Off();

	// 몬스터
	EnemyGrunt* Grunt = GetLevel()->CreateActor<EnemyGrunt>(ACTORGROUP::TIMEGROUP_ENEMY);
	Grunt->SetSpawnPos({ 737, -720, GetDepth(ACTOR_DEPTH::ENEMY) });
	Grunt->Off();
	Enemies.push_back(Grunt);

	// 세팅 시간
	TimeLimit = true;
	SetCurTimeLimitSeconds(30.0f);

	Off();

	Stairs.resize(0);
}

void Room_Factory2::OnEvent()
{
	GlobalValueManager::ColMap = Background_ColMap;
	Background->On();
	FanGuard->On();

	// 지형지물
	PtrFan->On();
	PtrFan->GetTransform().SetWorldPosition({ 1430, -375, GetDepth(ACTOR_DEPTH::DOOR) });

	// 적 관련
	for (EnemyActor* Enemy : Enemies)
	{
		Enemy->On();

		Enemy->Respawn();
	}
}

void Room_Factory2::OffEvent()
{
	Background->Off();
	Background_ColMap->Off();

	if (nullptr != GlobalValueManager::ColMap)
	{
		GlobalValueManager::ColMap->Off();
	}


	// 지형지물
	PtrFan->Off();
	FanGuard->Off();

	// 적 관련
	for (EnemyActor* Enemy : Enemies)
	{
		Enemy->Off();
	}
}

void Room_Factory2::Update(float _DeltaTime)
{
}

void Room_Factory2::End()
{
}

