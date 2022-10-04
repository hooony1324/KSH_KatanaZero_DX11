#include "PreCompile.h"
#include "ContentsCore.h"
#pragma comment(lib, "GameEngineBase.lib")

#include "ControlGUI.h"
#include "TitleLevel.h"
#include "PlayLevel.h"
#include "EndingLevel.h"
#include "TestLevel.h"
#include <GameEngineCore/GameEngineBlend.h>

float4 ContentsCore::ContentsWindowSize;

ContentsCore::ContentsCore() 
{
}

ContentsCore::~ContentsCore() 
{
}

void ContentsCore::Start()
{
	// Debug GUI
	ControlGUI::Inst = GameEngineGUI::CreateGUIWindow<ControlGUI>("ControlGUI", nullptr);
	ControlGUI::Inst->Off();

	// < Resource Load >
	ShaderCompile();
	PipelineLoad();
	TextureLoad();
	SoundLoad();

	// < Level Laod >
	CreateLevel<TitleLevel>("TitleLevel");
	CreateLevel<PlayLevel>("PlayLevel");
	CreateLevel<EndingLevel>("EndingLevel");
	CreateLevel<TestLevel>("TestLevel");
	ChangeLevel("TitleLevel");

	// Input Key
	GameEngineInput::GetInst()->CreateKey("MouseLeft", VK_LBUTTON);
	GameEngineInput::GetInst()->CreateKey("MouseMid", VK_MBUTTON);
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


	GameEngineInput::GetInst()->CreateKey("M", 'M');	// 충돌맵 On/Off
	GameEngineInput::GetInst()->CreateKey("T", 'T');	// Transition Test
	GameEngineInput::GetInst()->CreateKey("C", 'C');	// 충돌체 On/Off
	GameEngineInput::GetInst()->CreateKey("R", 'R');	// 역재생


	//GameEngineGUI::CreateGUIWindow<GameEngineStatusWindow>("EngineStatus", nullptr);


}

void ContentsCore::Update(float _DeltaTime)
{
	GameEngineTime::SetLimitFrame(144);

	// 디버그용 레벨 체인지
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

void ContentsCore::ShaderCompile()
{
	// 커스텀 쉐이더 파일 컴파일
	GameEngineDirectory Dir;

	Dir.MoveParentToExitsChildDirectory("Resources");
	Dir.Move("Resources");
	Dir.Move("Shader");

	std::vector<GameEngineFile> Shaders = Dir.GetAllFile("hlsl");

	for (size_t i = 0; i < Shaders.size(); i++)
	{
		GameEngineShader::AutoCompile(Shaders[i].GetFullPath());
	}

}

void ContentsCore::PipelineLoad()
{
	// 커스텀 렌더링파이프라인 추가
	{
		GameEngineRenderingPipeLine* NewPipe = GameEngineRenderingPipeLine::Create("TextureMask");
		NewPipe->SetVertexShader("TextureMask.hlsl");
		NewPipe->SetPixelShader("TextureMask.hlsl");
	}

	{
		GameEngineRenderingPipeLine* NewPipe = GameEngineRenderingPipeLine::Create("Distortion");
		NewPipe->SetInputAssembler1VertexBuffer("FullRect");
		NewPipe->SetInputAssembler2IndexBuffer("FullRect");
		NewPipe->SetVertexShader("Distortion.hlsl");
		NewPipe->SetPixelShader("Distortion.hlsl");
	}

	{
		GameEngineRenderingPipeLine* NewPipe = GameEngineRenderingPipeLine::Create("Wave");
		NewPipe->SetVertexShader("Wave.hlsl");
		NewPipe->SetPixelShader("Wave.hlsl");
		NewPipe->SetInputAssembler1VertexBuffer("FullRect");
		NewPipe->SetInputAssembler2IndexBuffer("FullRect");
	}

	{
		GameEngineRenderingPipeLine* NewPipe = GameEngineRenderingPipeLine::Create("DistortionGlitch");
		NewPipe->SetVertexShader("DistortionGlitch.hlsl");
		NewPipe->SetPixelShader("DistortionGlitch.hlsl");
		NewPipe->SetInputAssembler1VertexBuffer("FullRect");
		NewPipe->SetInputAssembler2IndexBuffer("FullRect");
	}

	{
		GameEngineRenderingPipeLine* NewPipe = GameEngineRenderingPipeLine::Create("PointLight");
		NewPipe->SetVertexShader("PointLight.hlsl");
		NewPipe->SetPixelShader("PointLight.hlsl");
		NewPipe->SetInputAssembler1VertexBuffer("FullRect");
		NewPipe->SetInputAssembler2IndexBuffer("FullRect");
	}

	{
		GameEngineRenderingPipeLine* NewPipe = GameEngineRenderingPipeLine::Create("RimLight");
		NewPipe->SetVertexShader("RimLight.hlsl");
		NewPipe->SetPixelShader("RimLight.hlsl");
		NewPipe->SetInputAssembler1VertexBuffer("FullRect");
		NewPipe->SetInputAssembler2IndexBuffer("FullRect");
	}

	{
		GameEngineRenderingPipeLine* NewPipe = GameEngineRenderingPipeLine::Create("TextureWave");
		NewPipe->SetVertexShader("TextureWave.hlsl");
		NewPipe->SetPixelShader("TextureWave.hlsl");
		NewPipe->SetInputAssembler1VertexBuffer("FullRect");
		NewPipe->SetInputAssembler2IndexBuffer("FullRect");
	}

	{
		GameEngineRenderingPipeLine* NewPipe = GameEngineRenderingPipeLine::Create("TextureConcave");
		NewPipe->SetVertexShader("TextureConcave.hlsl");
		NewPipe->SetPixelShader("TextureConcave.hlsl");
	}


	// Blend Resource
	{
		D3D11_BLEND_DESC Desc = { 0 };

		Desc.AlphaToCoverageEnable = FALSE;
		Desc.IndependentBlendEnable = FALSE;
		Desc.RenderTarget[0].BlendEnable = true;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		// 알파쪽만 따로 처리하는 옵션
		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		GameEngineBlend::Create("NoBlackBlend", Desc);
	}
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

	// 애니메이션(폴더)
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

	Dir.Move("UI");
	GameEngineFont::Load(Dir.GetFullPath() + "\font_korean.png");

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
	

	{
		GameEngineDirectory SoundDir{Dir};
		std::vector<std::string> ResourceDirs = { "BGM", "Boss", "Player"};
		for (const std::string& Val : ResourceDirs)
		{
			SoundDir.Move(Val);

			std::vector<GameEngineFile> Files = SoundDir.GetAllFile();
			for (auto& File : Files)
			{
				GameEngineSound::LoadRessource(File);
			}

			SoundDir.MoveParentToExitsChildDirectory(Val);
		}
	}
}