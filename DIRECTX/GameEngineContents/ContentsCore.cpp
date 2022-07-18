#include "PreCompile.h"
#include "ContentsCore.h"
#pragma comment(lib, "GameEngineBase.lib")

#include "TestLevel.h"
#include "TitleLevel.h"
#include "PlayLevel.h"
#include "EndingLevel.h"

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
	CreateLevel<EndingLevel>("EndingLevel");
	ChangeLevel("TitleLevel");

	// Input Key
	GameEngineInput::GetInst()->CreateKey("W", 'W');
	GameEngineInput::GetInst()->CreateKey("A", 'A');
	GameEngineInput::GetInst()->CreateKey("S", 'S');
	GameEngineInput::GetInst()->CreateKey("D", 'D');
	GameEngineInput::GetInst()->CreateKey("SpaceBar", VK_SPACE);
	GameEngineInput::GetInst()->CreateKey("Shift", VK_SHIFT);

	GameEngineInput::GetInst()->CreateKey("Numpad0", VK_NUMPAD0);
	GameEngineInput::GetInst()->CreateKey("Numpad1", VK_NUMPAD1);
	GameEngineInput::GetInst()->CreateKey("Numpad2", VK_NUMPAD2);
	GameEngineInput::GetInst()->CreateKey("Numpad3", VK_NUMPAD3);
	GameEngineInput::GetInst()->CreateKey("Numpad4", VK_NUMPAD4);

}

void ContentsCore::Update(float _DeltaTime)
{
	// 디버그용 레벨 체인지
	if (true == GameEngineInput::GetInst()->IsDown("Numpad0"))
	{
		ChangeLevel("TitleLevel");
	}

	if (true == GameEngineInput::GetInst()->IsDown("Numpad1"))
	{
		ChangeLevel("PlayLevel");
	}

	if (true == GameEngineInput::GetInst()->IsDown("Numpad2"))
	{
		ChangeLevel("EndingLevel");
	}

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
		std::vector<std::string> ResourceDirs = { "Player", "Enemy", "Room", "Title", "Ending"};
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

	// 애니메이션(폴더)
	{
		GameEngineDirectory TexDir{ Dir };
		std::vector<std::string> ResourceDirs = { "Player", "Enemy", "Title", "Ending"};
		for (const std::string& Val : ResourceDirs)
		{
			TexDir.Move(Val);
			for (GameEngineDirectory Path : TexDir.GetAllDirectory())
			{
				GameEngineFolderTexture::Load(Path.GetFullPath());
			}

			TexDir.MoveParentToExitsChildDirectory(Val);
		}
	}
}

void ContentsCore::SoundLoad()
{
	GameEngineDirectory Dir;
	Dir.MoveParentToExitsChildDirectory("Resources");
	Dir.Move("Resources");
	Dir.Move("Sound");

	std::vector<GameEngineFile> Files = Dir.GetAllFile();

	for (auto& File : Files)
	{
		GameEngineSound::LoadRessource(File);
	}
}