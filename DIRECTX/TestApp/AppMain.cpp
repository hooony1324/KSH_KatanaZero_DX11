#include <Windows.h>

#pragma comment(lib, "GameEngineBase.lib")
#pragma comment(lib, "GameEngineCore.lib")
#pragma comment(lib, "GameEngineContents.lib")

#include <GameEngineContents/TestContentsCore.h>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	//GameEngineDirectory Dir;

	//if (false == Dir.MoveParentToExitsChildDirectory("Resources"))
	//{
	//	MsgBoxAssert("��θ� ã�� ���߽��ϴ�.");
	//	return 0;
	//}

	//Dir.Move("Resources");
	//Dir.Move("Sound");

	//std::vector<GameEngineFile> Files = Dir.GetAllFile();

	//// Range for
	//// Files Ư���� ���۷����Ͱ� �Ǿ������� 
	//for (GameEngineFile& File : Files)
	//{
	//	GameEngineSound::LoadRessource(File);
	//}

	//GameEngineSound::SoundPlayOneShot("BGM.mp3");

	//while (true)
	//{
	//	GameEngineSound::Update();
	// 
	//}


	GameEngineCore::Start<TestContentsCore>();


	return 0;
}