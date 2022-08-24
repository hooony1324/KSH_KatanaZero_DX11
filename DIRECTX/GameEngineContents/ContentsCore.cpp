#include "PreCompile.h"
#include "ContentsCore.h"
#pragma comment(lib, "GameEngineBase.lib")

#include "TitleLevel.h"
#include "PlayLevel.h"
#include "EndingLevel.h"
#include "TestLevel.h"

float4 ContentsCore::ContentsWindowSize;

ContentsCore::ContentsCore() 
{
}

ContentsCore::~ContentsCore() 
{
}

void ContentsCore::Start()
{
	// ���������������� �߰� �׽�Ʈ
	{
		GameEngineRenderingPipeLine* NewPipe = GameEngineRenderingPipeLine::Create("UserCustom");
		NewPipe->SetVertexShader("UserCustom.hlsl");
		NewPipe->SetPixelShader("UserCustom.hlsl");
	}


	// < Resource Load >
	TextureLoad();
	SoundLoad();

	// < Level Laod >
	CreateLevel<TitleLevel>("TitleLevel");
	CreateLevel<PlayLevel>("PlayLevel");
	CreateLevel<EndingLevel>("EndingLevel");
	CreateLevel<TestLevel>("TestLevel");
	ChangeLevel("TestLevel");

	// Input Key
	GameEngineInput::GetInst()->CreateKey("MouseLeft", VK_LBUTTON);
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
	GameEngineInput::GetInst()->CreateKey("Numpad5", VK_NUMPAD5);
	GameEngineInput::GetInst()->CreateKey("Numpad6", VK_NUMPAD6);


	GameEngineInput::GetInst()->CreateKey("M", 'M');	// �浹�� On/Off
	GameEngineInput::GetInst()->CreateKey("T", 'T');	// Transition Test
	GameEngineInput::GetInst()->CreateKey("C", 'C');	// �浹ü On/Off
	GameEngineInput::GetInst()->CreateKey("R", 'R');	// �����


	//GameEngineGUI::CreateGUIWindow<GameEngineStatusWindow>("EngineStatus", nullptr);

}

void ContentsCore::Update(float _DeltaTime)
{
	// ����׿� ���� ü����
	if (true == GameEngineInput::GetInst()->IsDown("Numpad1"))
	{
		ChangeLevel("TitleLevel");
	}

	if (true == GameEngineInput::GetInst()->IsDown("Numpad2"))
	{
		ChangeLevel("PlayLevel");
	}

	if (true == GameEngineInput::GetInst()->IsDown("Numpad3"))
	{
		ChangeLevel("EndingLevel");
	}

}

void ContentsCore::End()
{
}

void ContentsCore::TextureLoad()
{

	GameEngineDirectory Dir;
	Dir.MoveParentToExitsChildDirectory("Resources");
	Dir.Move("Resources");
	Dir.Move("Texture");

	// ���� �ؽ�ó
	{
		GameEngineDirectory TexDir{Dir};
		std::vector<std::string> ResourceDirs = { "Player", "Enemy", "Room", "Title", "Ending", "UI", "FX", "Boss"};
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

	// �ִϸ��̼�(����)
	{
		GameEngineDirectory TexDir{ Dir };
		std::vector<std::string> ResourceDirs = { "Player", "Enemy", "Title", "Ending", "FX", "Room", "UI", "Boss" };
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