#include "PreCompile.h"
#include "PlayLevelGUI.h"
#include "Cursor.h"
#include "CharacterActor.h"

#include "BossPsychoGiant.h"

PlayLevelGUI::PlayLevelGUI() 
{
}

PlayLevelGUI::~PlayLevelGUI() 
{
}


void PlayLevelGUI::Initialize(GameEngineLevel* _Level)
{

}

void PlayLevelGUI::OnGUI(GameEngineLevel* _Level, float _DeltaTime)
{
	int FPS = static_cast<int>(1.0f / _DeltaTime);
	std::string Name = "FPS : " + std::to_string(FPS);
	ImGui::Text(Name.c_str());

	{
		float4 Pos = Cursor::GetCursorScreenPosition();
		std::string Name = "MouseScreenPosition : " + std::to_string(Pos.x) + " : " + std::to_string(Pos.y);
		ImGui::Text(Name.c_str());
	}

	{
		float4 Pos = Cursor::GetCursorWorldPosition();
		std::string Name = "MouseWorldPosition : " + std::to_string(Pos.x) + " : " + std::to_string(Pos.y);
		ImGui::Text(Name.c_str());
	}

	{
		float4 Pos = Cursor::GetCursorPositionToActor();
		std::string Name = "MouseToActorPosition : " + std::to_string(Pos.x) + " : " + std::to_string(Pos.y);
		ImGui::Text(Name.c_str());
	}



	if (true == ImGui::Button("CollisionDebugSwtich"))
	{
		GEngine::CollisionDebugSwitch();

	}

	ImGui::SameLine();
	if (true == ImGui::Button("CheatMode"))
	{
		CharacterActor::CheatModeSwitch();
	}


	

	// 보스 스테이지
	if (false == BossPsychoGiant::GetInst()->IsUpdate())
	{
		return;
	}

	ImGui::Text("");
	ImGui::Text("Boss Patterns");


	if (true == ImGui::Button("Pattern_StabAttack"))
	{


	}

	ImGui::SameLine();
	if (true == ImGui::Button("Pattern_KnifeAttack1"))
	{


	}

	ImGui::SameLine();
	if (true == ImGui::Button("Pattern_KnifeAttack2"))
	{

	}

	ImGui::SameLine();
	if (true == ImGui::Button("Pattern_TurretAttack"))
	{

	}

}
