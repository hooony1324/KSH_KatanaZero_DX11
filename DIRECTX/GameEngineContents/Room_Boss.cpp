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

	// bg > floor > fg
	Background_Mid = CreateComponent<GameEngineTextureRenderer>();
	Background_Mid->SetTexture("spr_psychboss_bg_1_0.png");
	Background_Mid->GetTransform().SetLocalScale({1280, 734, 1});
	Background_Mid->GetTransform().SetLocalMove({ 633, -560, GetDepth(ACTOR_DEPTH::BOSSMIDGROUND) });
	Background_Mid->Off();

	Background_Front = CreateComponent<GameEngineTextureRenderer>();
	Background_Front->SetTexture("spr_psychboss_fg_0.png");
	Background_Front->GetTransform().SetLocalScale({1350, 774, 1});
	Background_Front->GetTransform().SetLocalMove({ 633, -545, GetDepth(ACTOR_DEPTH::BOSSFOREGROUND) });
	Background_Front->Off();

	Background_Floor = CreateComponent<GameEngineTextureRenderer>();
	Background_Floor->SetTexture("spr_psychboss_floor_0.png");
	Background_Floor->GetTransform().SetLocalScale({1072, 140, 1});
	Background_Floor->GetTransform().SetLocalMove({ 640, -852, GetDepth(ACTOR_DEPTH::BOSSFLOOR) });
	Background_Floor->Off();

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

	Background_Mid->On();
	Background_Front->On();
	Background_Floor->On();


	// 보스
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

	Background_Mid->Off();
	Background_Front->Off();
	Background_Floor->Off();

	// 보스
	BossGiant->Off();

}
