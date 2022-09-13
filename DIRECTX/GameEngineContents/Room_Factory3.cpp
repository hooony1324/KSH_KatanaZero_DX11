#include "PreCompile.h"
#include "Room_Factory3.h"
#include <GameEngineCore/CoreMinimal.h>

#include "EnemyCop.h"
#include "EnemyGrunt.h"

Room_Factory3::Room_Factory3() 
{
}

Room_Factory3::~Room_Factory3() 
{
}

void Room_Factory3::Start()
{
	EmptyRoomInit();

	Background->SetTexture("room_factory_4.png");
	Background->ScaleToTexture();
	Background->SetPivot(PIVOTMODE::LEFTTOP);
	Background->GetTransform().SetLocalMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_0) });
	Background->Off();

	Background_ColMap->SetTexture("room_factory_4_colmap.png");
	Background_ColMap->ScaleToTexture();
	Background_ColMap->SetPivot(PIVOTMODE::LEFTTOP);
	Background_ColMap->GetTransform().SetLocalMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) });
	Background_ColMap->Off();

	//// 배경 설정되고 카메라 클램프 영역 세팅
	InitCameraClampArea();


	PlayerSpawnPos = float4{ 220, -920, GetDepth(ACTOR_DEPTH::PLAYER) };

	//// 적 소환 리스트 1400, -310
	//Grunt = GetLevel()->CreateActor<EnemyGrunt>(ACTORGROUP::TIMEGROUP_ENEMY);
	//Grunt->SetSpawnPos({ 1400, -320, GetDepth(ACTOR_DEPTH::ENEMY) });
	//Grunt->Off();
	//Enemies.push_back(Grunt);

	//// 400, -330
	//Cop = GetLevel()->CreateActor<EnemyCop>(ACTORGROUP::TIMEGROUP_ENEMY);
	//Cop->SetSpawnPos({ 400, -360, GetDepth(ACTOR_DEPTH::ENEMY) });
	//Cop->Off();
	//Enemies.push_back(Cop);


	// 세팅 시간
	TimeLimit = true;
	SetCurTimeLimitSeconds(60.0f);

	Off();
}

void Room_Factory3::Update(float _DeltaTime)
{
}

void Room_Factory3::End()
{
}

void Room_Factory3::OnEvent()
{
	GlobalValueManager::ColMap = Background_ColMap;
	Background->On();

	// 지형지물


	// 적 관련
	for (EnemyActor* Enemy : Enemies)
	{
		Enemy->On();

		Enemy->Respawn();
	}
}

void Room_Factory3::OffEvent()
{
	Background->Off();
	Background_ColMap->Off();

	if (nullptr != GlobalValueManager::ColMap)
	{
		GlobalValueManager::ColMap->Off();
	}


	// 지형지물


	// 적 관련
	for (EnemyActor* Enemy : Enemies)
	{
		Enemy->Off();
	}
}
