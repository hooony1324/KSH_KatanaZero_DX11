#include "PreCompile.h"
#include "Room_Factory3.h"
#include <GameEngineCore/CoreMinimal.h>

#include "EnemyCop.h"
#include "EnemyGrunt.h"
#include "Stair.h"
#include "SplattedBlood.h"

#include "EnemyBullet.h"

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
	Background_ColMap->GetPixelData().MulColor.a = 0.5f;
	Background_ColMap->Off();

	//// 배경 설정되고 카메라 클램프 영역 세팅
	InitCameraClampArea();


	PlayerSpawnPos = float4{ 220, -920, GetDepth(ACTOR_DEPTH::PLAYER) };


	for (int i = 0; i < 2; i++)
	{
		EnemyGrunt* Grunt;
		Grunt = GetLevel()->CreateActor<EnemyGrunt>(ACTORGROUP::TIMEGROUP_ENEMY);
		Grunt->Off();
		Enemies.push_back(Grunt);
	}


	for (int i = 0; i < 5; i++)
	{
		EnemyCop* Cop;
		Cop = GetLevel()->CreateActor<EnemyCop>(ACTORGROUP::TIMEGROUP_ENEMY);
		Cop->Off();
		Enemies.push_back(Cop);
	}

	Enemies[0]->SetSpawnPos({ 1500, -640, GetDepth(ACTOR_DEPTH::ENEMY) }); // Grunt
	Enemies[1]->SetSpawnPos({ 1895, -380, GetDepth(ACTOR_DEPTH::ENEMY) }); // Grunt
	Enemies[1]->SetChaseOff();

	Enemies[2]->SetSpawnPos({ 1550, -640, GetDepth(ACTOR_DEPTH::ENEMY) });
	Enemies[2]->SetChaseOn();

	Enemies[3]->SetSpawnPos({ 655, -640, GetDepth(ACTOR_DEPTH::ENEMY) });
	Enemies[3]->SetChaseOff();

	Enemies[4]->SetSpawnPos({ 828, -380, GetDepth(ACTOR_DEPTH::ENEMY) });
	Enemies[4]->SetChaseOn();

	Enemies[5]->SetSpawnPos({ 1230, -380, GetDepth(ACTOR_DEPTH::ENEMY) });
	Enemies[5]->SetChaseOff();

	Enemies[6]->SetSpawnPos({ 1500, -380, GetDepth(ACTOR_DEPTH::ENEMY) });
	Enemies[6]->SetChaseOn();


	// 세팅 시간
	TimeLimit = true;
	SetCurTimeLimitSeconds(60.0f);

	// 계단
	StairSetting();

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
	GlobalValueManager::ClearStairs();
	for (Stair* Ptr : Stairs)
	{
		Ptr->On();
		GlobalValueManager::Stairs.push_back(Ptr);
	}

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
	GlobalValueManager::ClearStairs();

	// 적 관련
	for (EnemyActor* Enemy : Enemies)
	{
		Enemy->Off();
	}
}

void Room_Factory3::StairSetting()
{

	// 계단
	Stair* Stair1_2 = GetLevel()->CreateActor<Stair>();		// 1->2층 가는
	Stair* Stair2_Right = GetLevel()->CreateActor<Stair>();
	Stair* Stair2_3 = GetLevel()->CreateActor<Stair>();		// 2->3층 가는
	Stair* Stair3 = GetLevel()->CreateActor<Stair>();


	Stair1_2->Spawn({ 991, -930, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) }, Stair2_Right, nullptr);
	Stair2_Right->Spawn({ 1295, -640, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) }, Stair2_3, Stair1_2);
	Stair2_3->Spawn({ 560, -640, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) }, Stair3, Stair2_Right);
	Stair3->Spawn({ 290, -350, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) }, nullptr, Stair2_3);


	Stairs.push_back(Stair1_2);
	Stairs.push_back(Stair2_Right);
	Stairs.push_back(Stair2_3);
	Stairs.push_back(Stair3);

	for (auto Stair : Stairs)
	{
		Stair->Off();
	}
}
