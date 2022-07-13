#include "PreCompile.h"
#include "ContentsCore.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>
#include <GameEngineCore/GameEngineTexture.h>
#include <GameEngineBase/GameEngineSound.h>
#pragma comment(lib, "GameEngineBase.lib")

#include "TestLevel.h"
#include "TitleLevel.h"
#include "PlayLevel.h"

ContentsCore::ContentsCore() 
{
}

ContentsCore::~ContentsCore() 
{
}

void ContentsCore::Start()
{
	// < Level Laod >
	CreateLevel<TestLevel>("TestLevel");
	CreateLevel<TitleLevel>("TitleLevel");
	CreateLevel<PlayLevel>("PlayLevel");
	ChangeLevel("TitleLevel");

	// < Resource Load >
	// Image Load
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExitsChildDirectory("Resources");
		Dir.Move("Resources");
		Dir.Move("Image");

		std::vector<std::string> ResourceDirs = { "Enemy", "Player", "Room"};
		
		for (const std::string& Val : ResourceDirs)
		{
			Dir.Move(Val);
			std::vector<GameEngineFile> Files = Dir.GetAllFile(".png");

			for (size_t i = 0; i < Files.size(); i++)
			{
				GameEngineTexture::Load(Files[i].GetFullPath());
			}

			Dir.MoveParentToExitsChildDirectory(Val);
		}
	}
	
	// Sound Load
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExitsChildDirectory("Resources");
		Dir.Move("Resources");
		Dir.Move("Sound");

		std::vector<GameEngineFile> Files = Dir.GetAllFile(".wav");

		for (auto& File : Files)
		{
			GameEngineSound::LoadRessource(File);
		}
	}



}

void ContentsCore::Update(float _DeltaTime)
{
}

void ContentsCore::End()
{
}
