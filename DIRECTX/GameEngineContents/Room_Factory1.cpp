#include "PreCompile.h"
#include "Room_Factory1.h"

#include "Door.h"
#include "EnemyGrunt.h"
#include "EnemyCop.h"
#include "PointLight.h"

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
	Background_ColMap->GetPixelData().MulColor.a = 0.5f;
	Background_ColMap->Off();

	// ��� �����ǰ� ī�޶� Ŭ���� ���� ����
	InitCameraClampArea();


	PlayerSpawnPos = float4{ 220, -645, GetDepth(ACTOR_DEPTH::PLAYER) };

	PtrDoor = GetLevel()->CreateActor<Door>(ACTORGROUP::TIMEGROUP);
	PtrDoor->GetTransform().SetWorldMove({ 608, -320 });
	PtrDoor->Off();

	// �� ��ȯ
	Grunt = GetLevel()->CreateActor<EnemyGrunt>(ACTORGROUP::TIMEGROUP_ENEMY);
	Grunt->SetSpawnPos({ 1400, -320, GetDepth(ACTOR_DEPTH::ENEMY) });
	Grunt->Off();
	Enemies.push_back(Grunt);
	
	Cop = GetLevel()->CreateActor<EnemyCop>(ACTORGROUP::TIMEGROUP_ENEMY);
	Cop->SetSpawnPos({ 400, -360, GetDepth(ACTOR_DEPTH::ENEMY) });
	Cop->Off();
	Enemies.push_back(Cop);

	// ����
	// ���
	StairSetting();
	
	// ��
	PointLight* Light = GetLevel()->CreateActor<PointLight>();
	Light->GetTransform().SetWorldPosition({ 400, -600, GetDepth(ACTOR_DEPTH::BACKGROUND_4) });

	// ���� �ð�
	TimeLimit = true;
	SetCurTimeLimitSeconds(60.0f);

	Off();
}

void Room_Factory1::OnEvent()
{
	// �� ����
	Background->On();
	GlobalValueManager::ColMap = Background_ColMap;

	// ���� ��ȯ
	PtrDoor->On();

	GlobalValueManager::ClearStairs();
	for (Stair* Ptr : Stairs)
	{
		Ptr->On();
		GlobalValueManager::Stairs.push_back(Ptr);
	}

	// �� ����
	for (EnemyActor* Enemy : Enemies)
	{
		Enemy->On();
		Enemy->Respawn();
	}
}

void Room_Factory1::OffEvent()
{
	// �� ����
	Background->Off();
	if (nullptr != GlobalValueManager::ColMap)
	{
		GlobalValueManager::ColMap->Off();
	}

	// ���� ����
	PtrDoor->Off();

	// ���
	GlobalValueManager::ClearStairs();

	// �� ����
	for (EnemyActor* Enemy : Enemies)
	{
		Enemy->Off();
	}
}

// ����� �ִ� ���̸� �������־�� ��
void Room_Factory1::StairSetting()
{
	// 1�� 2��
	Stair* Stair1 = GetLevel()->CreateActor<Stair>();
	Stair* Stair2 = GetLevel()->CreateActor<Stair>();

	Stair1->Spawn({ 737, -645, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) }, Stair2, nullptr);
	Stair2->Spawn({ 1100, -355,  GetDepth(ACTOR_DEPTH::COLLISION) }, nullptr, Stair1);
		
	Stair1->Off();
	Stair2->Off();

	Stairs.push_back(Stair1);
	Stairs.push_back(Stair2);
}

void Room_Factory1::Update(float _DeltaTime)
{

}

void Room_Factory1::End()
{

}


