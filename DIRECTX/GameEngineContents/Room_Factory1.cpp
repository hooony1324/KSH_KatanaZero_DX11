#include "PreCompile.h"
#include "Room_Factory1.h"

#include "Door.h"
#include "EnemyGrunt.h"
#include "EnemyCop.h"

Room_Factory1::Room_Factory1() 
{
}

Room_Factory1::~Room_Factory1() 
{
}

void Room_Factory1::Start()
{
	EmptyRoomInit();

	Background->SetTexture("room_factory_2.png");
	Background->ScaleToTexture();
	Background->SetPivot(PIVOTMODE::LEFTTOP);
	Background->GetTransform().SetLocalMove({0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_0)});
	Background->Off();

	Background_ColMap->SetTexture("room_factory_2_colmap.png");
	Background_ColMap->ScaleToTexture();
	Background_ColMap->SetPivot(PIVOTMODE::LEFTTOP);
	Background_ColMap->GetTransform().SetLocalMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) });
	Background_ColMap->Off();

	// 배경 설정되고 카메라 클램프 영역 세팅
	InitCameraClampArea();


	PlayerSpawnPos = float4{ 220, -620, GetDepth(ACTOR_DEPTH::PLAYER) };

	PtrDoor = GetLevel()->CreateActor<Door>(ACTORGROUP::TIMEGROUP);
	PtrDoor->GetTransform().SetWorldMove({ 575, -320 });

	// 적 소환 리스트 1400, -310
	Grunt = GetLevel()->CreateActor<EnemyGrunt>(ACTORGROUP::TIMEGROUP_ENEMY);
	Grunt->SetSpawnPos({ 1400, -310, GetDepth(ACTOR_DEPTH::ENEMY) });
	Grunt->Off();
	Enemies.push_back(Grunt);
	
	// 400, -330
	Cop = GetLevel()->CreateActor<EnemyCop>(ACTORGROUP::TIMEGROUP_ENEMY);
	Cop->SetSpawnPos({ 400, -330, GetDepth(ACTOR_DEPTH::ENEMY) });
	Cop->Off();
	Enemies.push_back(Cop);


	// 세팅 시간
	TimeLimit = true;
	SetCurTimeLimitSeconds(300.0f);

	Off();
}

void Room_Factory1::Setting()
{
	// 맵 관련
	Background->On();
	GlobalValueManager::ColMap = Background_ColMap;

	// 아래층 계단
	StairSetting();

	// 지형 소환
	PtrDoor->Close();

	// 적 관련
	for (EnemyActor* Enemy : Enemies)
	{
		if (false == Enemy->IsUpdate())
		{
			Enemy->On();
		}
		
		Enemy->Respawn();
	}


}

void Room_Factory1::Clear()
{
	// 맵 관련
	Background->Off();
	if (nullptr != GlobalValueManager::ColMap)
	{
		GlobalValueManager::ColMap->Off();
	}
	
	// 지형 관련
	PtrDoor->Off();

	// 계단
	GlobalValueManager::ClearStairs();

	// 적 관련
	for (EnemyActor* Enemy : Enemies)
	{
		Enemy->Off();
	}
}

// 계단이 있는 방이면 세팅해주어야 함
void Room_Factory1::StairSetting()
{
	GlobalValueManager::ClearStairs();

	{
		GameEngineCollision* DownStair = CreateComponent<GameEngineCollision>();
		DownStair->GetTransform().SetLocalScale({ 20, 20, GetDepth(ACTOR_DEPTH::COLLISION) });
		DownStair->GetTransform().SetWorldPosition({ 1130, -355, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) });
		DownStair->ChangeOrder(COLLISIONGROUP::STAIR);
		DownStair->SetDebugSetting(CollisionType::CT_AABB2D, { 0, 1, 0, 0.25f });
		GlobalValueManager::Collision_DownStairs.push_back(DownStair);
	}

	{
		GameEngineCollision* UpStair = CreateComponent<GameEngineCollision>();
		UpStair->GetTransform().SetLocalScale({ 20, 20, GetDepth(ACTOR_DEPTH::COLLISION) });
		UpStair->GetTransform().SetWorldPosition({ 737, -645, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) });
		UpStair->SetDebugSetting(CollisionType::CT_AABB2D, { 0, 0, 1, 0.25f });
		UpStair->ChangeOrder(COLLISIONGROUP::STAIR);
		GlobalValueManager::Collision_UpStairs.push_back(UpStair);
	}

}

void Room_Factory1::Update(float _DeltaTime)
{

}

void Room_Factory1::End()
{

}


