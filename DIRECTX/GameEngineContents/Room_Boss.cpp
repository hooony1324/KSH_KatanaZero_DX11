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
	Background->SetTexture("spr_psychboss_background.png");
	Background->GetTransform().SetLocalScale({ 1280, 720 });
	Background->SetPivot(PIVOTMODE::LEFTTOP);
	Background->GetTransform().SetLocalMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_0)});
	Background->Off();

	Background_ColMap->SetTexture("spr_psychboss_background_colmap.png");
	Background_ColMap->ScaleToTexture();
	Background_ColMap->SetPivot(PIVOTMODE::LEFTTOP);
	Background_ColMap->GetTransform().SetLocalMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) });
	Background_ColMap->Off();

	// ��� �����ǰ� ī�޶� Ŭ���� ���� ����
	InitCameraClampArea();

	// ������ġ, �������� ��
	PlayerSpawnPos = float4{ 220, -620, GetDepth(ACTOR_DEPTH::PLAYER) };
}

void Room_Boss::Update(float _DeltaTime)
{
}

void Room_Boss::End()
{
}

void Room_Boss::Setting()
{
	// �� ����
	Background->On();
	GlobalValueManager::ColMap = Background_ColMap;

}

void Room_Boss::Clear()
{
	// �� ����
	Background->Off();
	if (nullptr != GlobalValueManager::ColMap)
	{
		GlobalValueManager::ColMap->Off();
	}


}
