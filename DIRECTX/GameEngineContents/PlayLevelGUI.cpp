#include "PreCompile.h"
#include "PlayLevelGUI.h"
#include "Cursor.h"
#include "CharacterActor.h"

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
		float4 Pos = Cursor::GetCursorPosition();
		std::string Name = "MousePosition : " + std::to_string(Pos.x) + " : " + std::to_string(Pos.y);
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



}
