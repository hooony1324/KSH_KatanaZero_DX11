#include "PreCompile.h"
#include "Room_Boss.h"

#include "BossPsychoGiant.h"

Room_Boss::Room_Boss() 
{
}

Room_Boss::~Room_Boss() 
{
}

void Room_Boss::Start()
{
	EmptyRoomInit();

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

	// 배경 설정되고 카메라 클램프 영역 세팅
	InitCameraClampArea();

	// 스폰위치, 지형지물 등
	PlayerSpawnPos = float4{ 631, -670, GetDepth(ACTOR_DEPTH::PLAYER) };

	// 보스
	BossGiant = GetLevel()->CreateActor<BossPsychoGiant>();
	BossGiant->GetTransform().SetWorldPosition({ 637, -541, GetDepth(ACTOR_DEPTH::BOSS) });
	BossGiant->Off();

	// 세팅 시간
	TimeLimit = false;

	Off();
}

void Room_Boss::OnEvent()
{



}

void Room_Boss::Update(float _DeltaTime)
{
}

void Room_Boss::End()
{
}


void Room_Boss::Setting()
{
	// 맵 관련
	Background->On();
	GlobalValueManager::ColMap = Background_ColMap;

	BossGiant->On();
}

void Room_Boss::Clear()
{
	// 맵 관련
	Background->Off();
	if (nullptr != GlobalValueManager::ColMap)
	{
		GlobalValueManager::ColMap->Off();
	}

	BossGiant->Off();

}
