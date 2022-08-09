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

	Background_Collision->SetTexture("room_factory_2_colmap.png");
	Background_Collision->ScaleToTexture();
	Background_Collision->SetPivot(PIVOTMODE::LEFTTOP);
	Background_Collision->GetTransform().SetLocalMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) });
	Background_Collision->Off();
	
	// 배경 설정되고 카메라 클램프 영역 세팅
	InitCameraClampArea();


	PlayerSpawnPos = float4{ 220, -620, GetDepth(ACTOR_DEPTH::PLAYER) };

	// 적 소환 리스트 1400, -310
	Grunt = GetLevel()->CreateActor<EnemyGrunt>();
	Grunt->SetSpawnPos({ 1400, -310, GetDepth(ACTOR_DEPTH::ENEMY) });
	Enemies.push_back(Grunt);

	Cop = GetLevel()->CreateActor<EnemyCop>();
	Cop->SetSpawnPos({ 400, -330, GetDepth(ACTOR_DEPTH::ENEMY) });
	Enemies.push_back(Cop);
}

void Room_Factory1::Setting()
{
	// 맵 관련
	Background->On();
	GlobalValueManager::ColMap = Background_Collision;

	// 지형 소환
	PtrDoor = GetLevel()->CreateActor<Door>();
	PtrDoor->GetTransform().SetWorldMove({ 575, -320 });
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
	GlobalValueManager::ColMap->Off();
	GlobalValueManager::ColMap = nullptr;
	
	// 지형 관련
	PtrDoor->Off();

	// 적 관련
	for (EnemyActor* Enemy : Enemies)
	{
		Enemy->Off();
	}
}

void Room_Factory1::Update(float _DeltaTime)
{
	
}

void Room_Factory1::End()
{

}


