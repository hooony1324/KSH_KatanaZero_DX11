#include "PreCompile.h"
#include "ControlGUI.h"

#include "PlayLevel.h"
#include "Cursor.h"
#include "CharacterActor.h"
#include "BossPsychoGiant.h"
#include "Effect_Wave.h"
#include "Effect_DistortionGlitch.h"
#include "Effect_Distortion.h"
#include "Effect_PointLight.h"

#include "LiveActor.h"

ControlGUI* ControlGUI::Inst = nullptr;

ControlGUI::ControlGUI()
{
}

ControlGUI::~ControlGUI()
{

}


void ControlGUI::Initialize(GameEngineLevel* _Level)
{

}


void ControlGUI::OnGUI(GameEngineLevel* _Level, float _DeltaTime)
{
	std::string Name = "FPS : " + std::to_string(GameEngineTime::GetFPS());
	ImGui::Text(Name.c_str());
	{
		float4 Pos = Cursor::GetCursorScreenPosition();
		std::string Name = "MouseScreenPosition : " + std::to_string(Pos.x) + " : " + std::to_string(Pos.y);
		ImGui::Text(Name.c_str());
	}

	{
		float4 Pos = Cursor::GetCursorWorldPosition();
		std::string Name = "MouseCameraPosition : " + std::to_string(Pos.x) + " : " + std::to_string(Pos.y);
		ImGui::Text(Name.c_str());
	}

	{
		float4 Pos = Cursor::GetCursorPositionToActor();
		std::string Name = "MouseToActorPosition : " + std::to_string(Pos.x) + " : " + std::to_string(Pos.y);
		ImGui::Text(Name.c_str());
	}

	{
		float4 Pos = GlobalValueManager::PlayerPos;
		std::string Name = "PlayerPos : " + std::to_string(Pos.x) + " : " + std::to_string(Pos.y) + " : " + std::to_string(Pos.z);
		ImGui::Text(Name.c_str());
	}

	{
		float4 Pos = PlayLevel::PlayLevelInst->GetMainCameraActor()->GetTransform().GetWorldPosition();
		std::string Name = "MainCamActorPos : " + std::to_string(Pos.x) + " : " + std::to_string(Pos.y) + " : " + std::to_string(Pos.z);
		ImGui::Text(Name.c_str());
	}

	if (true == ImGui::Button("CollisionDebugSwtich"))
	{
		GEngine::CollisionDebugSwitch();
	}

	ImGui::SameLine();
	if (true == ImGui::Button("CollisionMapSwtich"))
	{
		GlobalValueManager::ColMap->OnOffSwitch();
	}

	ImGui::SameLine();
	if (true == ImGui::Button("WallStateDebug"))
	{
		LiveActor::SwitchWallStateDebug();
	}



	if (true == ImGui::Button("CheatMode"))
	{
		CharacterActor::CheatModeSwitch();
	}
	
	// 디버그용 레벨 체인지
	ImGui::NewLine();
	ImGui::Text("Current Level : ");
	ImGui::SameLine();
	ImGui::Text(_Level->GetNameCopy().c_str());
	if (true == ImGui::Button("TestLevel"))
	{
		GEngine::ChangeLevel("TestLevel");
	}
	
	ImGui::NewLine();
	ImGui::Text("Post Effects");
	if (true == ImGui::Button("Effect_Wave"))
	{
		Effect_Wave::GetInst()->EffectSwitch();
	}
	ImGui::SameLine();
	if (true == ImGui::Button("Effect_Distortion"))
	{
		Effect_Distortion::GetInst()->EffectSwitch();
	}
	ImGui::SameLine();
	if (true == ImGui::Button("Effect_DistortionGlitch"))
	{
		Effect_DistortionGlitch::GetInst()->EffectSwitch();
	}
	ImGui::SameLine();
	if (true == ImGui::Button("Effect_PointLight"))
	{
		Effect_PointLight::GetInst()->EffectSwitch();
	}



	ImGui::NewLine();
	ImGui::Text("Levels");
	if (true == ImGui::Button("TitleLevel"))
	{
		GEngine::ChangeLevel("TitleLevel");
	}

	ImGui::SameLine();
	if (true == ImGui::Button("PlayLevel"))
	{
		GEngine::ChangeLevel("PlayLevel");
	}

	ImGui::SameLine();
	if (true == ImGui::Button("EndingLevel"))
	{
		GEngine::ChangeLevel("EndingLevel");
	}


	// @@@ PLAYLEVEL 디버깅 @@@ //
	std::string LevelName = _Level->GetNameCopy();
	if(0 != LevelName.compare("PLAYLEVEL"))
	{
		return;
	}

	// 방 변경
	ImGui::NewLine();
	for (int i = 0; i < PlayLevel::PlayLevelInst->GetRoomsSize(); i++)
	{
		ImGui::SameLine();
		std::string RoomName = "Room " + std::to_string( i + 1 );
		if (true == ImGui::Button(RoomName.c_str()))
		{
			PlayLevel::PlayLevelInst->ChangeRoom(i);
		}
	}



	// @@@ 보스 스테이지 @@@ //
	if (false == BossPsychoGiant::GetInst()->IsUpdate())
	{
		return;
	}

	ImGui::NewLine();
	ImGui::Text("Boss Patterns");
	/*IDLE,
	SELECT,
	STAB,
	SPAWN_KNIFE1,
	SPAWN_KNIFE2,
	SPAWN_TURRET,
	SPAWN_PSYCHO,
	HURT,*/

	if (true == ImGui::Button("Pattern_Idle"))
	{
		BossPsychoGiant::GetInst()->SetSelectOff();
		return;

	}

	ImGui::SameLine();
	if (true == ImGui::Button("Pattern_Start"))
	{
		BossPsychoGiant::GetInst()->SetSelectOn();
		return;

	}


	if (true == ImGui::Button("Pattern_StabAttack"))
	{
		if (BossPsychoGiant::GetInst()->Pattern == PSYCHOGIANT_PATTERN::IDLE)
		{
			BossPsychoGiant::GetInst()->ChangePattern(PSYCHOGIANT_PATTERN::STAB);
		}
		return;
	}

	ImGui::SameLine();
	if (true == ImGui::Button("Pattern_KnifeAttack1"))
	{
		if (BossPsychoGiant::GetInst()->Pattern == PSYCHOGIANT_PATTERN::IDLE)
		{
			BossPsychoGiant::GetInst()->ChangePattern(PSYCHOGIANT_PATTERN::SPAWN_KNIFE1);
		}
		return;
	}

	ImGui::SameLine();
	if (true == ImGui::Button("Pattern_KnifeAttack2"))
	{
		if (BossPsychoGiant::GetInst()->Pattern == PSYCHOGIANT_PATTERN::IDLE)
		{
			BossPsychoGiant::GetInst()->ChangePattern(PSYCHOGIANT_PATTERN::SPAWN_KNIFE2);
		}
		return;
	}

	ImGui::SameLine();
	if (true == ImGui::Button("Pattern_TurretAttack"))
	{
		if (BossPsychoGiant::GetInst()->Pattern == PSYCHOGIANT_PATTERN::IDLE)
		{
			BossPsychoGiant::GetInst()->ChangePattern(PSYCHOGIANT_PATTERN::SPAWN_TURRET);
		}
		return;
	}

	if (true == ImGui::Button("Pattern_SpawnPsycho"))
	{
		if (BossPsychoGiant::GetInst()->Pattern == PSYCHOGIANT_PATTERN::IDLE)
		{
			BossPsychoGiant::GetInst()->ChangePattern(PSYCHOGIANT_PATTERN::SPAWN_PSYCHO);
		}
		return;
	}
	ImGui::SameLine();
	if (true == ImGui::Button("Pattern_Hurt"))
	{
		if (BossPsychoGiant::GetInst()->Pattern == PSYCHOGIANT_PATTERN::IDLE)
		{
			BossPsychoGiant::GetInst()->ChangePattern(PSYCHOGIANT_PATTERN::HURT);
		}
		return;
	}

}
