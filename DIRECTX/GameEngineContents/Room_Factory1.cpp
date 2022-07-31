#include "PreCompile.h"
#include "Room_Factory1.h"
#include "Door.h"

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


	// ������ġ, �������� ��
	PtrDoor = GetLevel()->CreateActor<Door>();
	PtrDoor->GetTransform().SetWorldMove({ 575, -320 });
	PtrDoor->Off();

	PlayerSpawnPos = float4{ 220, -550, GetDepth(ACTOR_DEPTH::PLAYER) };
}

void Room_Factory1::Setting()
{
	Background->On();
	GlobalValueManager::ColMap = Background_Collision;

	// ���� ��ȯ
	PtrDoor->Close();
	PtrDoor->On();


	// �� ��ȯ

}

void Room_Factory1::Clear()
{
	Background->Off();
	PtrDoor->Off();
	GlobalValueManager::ColMap->Off();
	GlobalValueManager::ColMap = nullptr;
}

void Room_Factory1::Update(float _DeltaTime)
{
	
}

void Room_Factory1::End()
{
}


