#include "PreCompile.h"
#include "ContentsCore.h"
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
	// < Resource Load >
	TextureLoad();
	SoundLoad();

	// < Level Laod >
	CreateLevel<TestLevel>("TestLevel");
	CreateLevel<TitleLevel>("TitleLevel");
	CreateLevel<PlayLevel>("PlayLevel");
	ChangeLevel("TestLevel");



}

void ContentsCore::Update(float _DeltaTime)
{
}

void ContentsCore::End()
{
	GameEngineSound::AllResourcesDestroy();
}

void ContentsCore::TextureLoad()
{

	GameEngineDirectory Dir;
	Dir.MoveParentToExitsChildDirectory("Resources");
	Dir.Move("Resources");
	Dir.Move("Texture");

	// 단일 텍스처
	{
		GameEngineDirectory TexDir{Dir};
		std::vector<std::string> ResourceDirs = { "Room", "Title" };
		for (const std::string& Val : ResourceDirs)
		{
			TexDir.Move(Val);
			std::vector<GameEngineFile> Files = TexDir.GetAllFile();

			for (size_t i = 0; i < Files.size(); i++)
			{
				GameEngineTexture::Load(Files[i].GetFullPath());
			}

			TexDir.MoveParentToExitsChildDirectory(Val);
		}
	}

	// 애니메이션
	{
		// Player
		GameEngineDirectory PlayerDir{ Dir };
		PlayerDir.Move("Player");

		std::vector<std::string> ResourceDirs = {
			"attack", "fall", "idle", "idle_to_run", "jump", "roll", "run", "run_to_idle" };

		for (const std::string& Path : ResourceDirs)
		{
			GameEngineDirectory AnimDir{ PlayerDir };
			AnimDir.Move(Path);
			GameEngineFolderTexture::Load(AnimDir.GetFullPath());
		}
	}
}

void ContentsCore::SoundLoad()
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