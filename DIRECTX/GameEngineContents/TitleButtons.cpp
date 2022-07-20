#include "PreCompile.h"
#include "TitleButtons.h"
#include <GameEngineCore/GEngine.h>

#include <GameEngineBase/GameEngineWindow.h>

const float LERPIN_DIST = 800.0f;

TitleButtons::TitleButtons() 
	: Renderer_Background(nullptr)
	, Renderer_Buttons(nullptr)
	, Renderer_Selector(nullptr)
{
}

TitleButtons::~TitleButtons() 
{
}

void TitleButtons::Start()
{
	Renderer_Background = CreateComponent<GameEngineDefaultRenderer>();
	Renderer_Background->GetTransform().SetLocalScale({ 530, 227 });
	Renderer_Background->SetPipeLine("Color");
	Color_Background = float4{ 0, 0, 0, 0.5f };
	Renderer_Background->ShaderResources.SetConstantBufferLink("ResultColor", Color_Background);

	Renderer_Buttons = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Buttons->SetTexture("TitleButtons.png");
	Renderer_Buttons->ScaleToTexture();

	// Selector
	Renderer_Selector = CreateComponent<GameEngineDefaultRenderer>();
	Renderer_Selector->GetTransform().SetLocalScale({ 510, 42 });
	Renderer_Selector->SetPipeLine("Color");
	Color_Selector = float4{ 1, 1, 1, 0.2f };
	Renderer_Selector->ShaderResources.SetConstantBufferLink("ResultColor", Color_Selector);

	ButtonsPos.push_back({ 0, -84 });
	ButtonsPos.push_back({ 0, -44 });
	ButtonsPos.push_back({ 0, -4 });
	ButtonsPos.push_back({ 0, 36 });
	ButtonsPos.push_back({ 0, 76 });
	ButtonIndex = 2;
	Renderer_Selector->GetTransform().SetLocalPosition(ButtonsPos[ButtonIndex]);

	// 초기세팅
	IsPlayLevelChange = false;

	// Lerp 
	LerpStart({ 0, -1000 }, { 0, -200 }, 0.9f);

	// 화면 밑 그림자
	GameEngineDefaultRenderer* Renderer_Shadow = CreateComponent<GameEngineDefaultRenderer>();
	Renderer_Shadow->GetTransform().SetLocalScale({ 1280, 1000 });
	Renderer_Shadow->GetTransform().SetLocalPosition({ 0, -650 });
	Renderer_Shadow->SetPipeLine("Color");
	Color_Shadow = float4{ 0, 0, 0, 1 };
	Renderer_Shadow->ShaderResources.SetConstantBufferLink("ResultColor", Color_Shadow);
}

void TitleButtons::OnEvent()
{

}

void TitleButtons::Update(float _DeltaTime)
{
	LerpUpdate(_DeltaTime);

	CheckInput();
}

void TitleButtons::End()
{
}

void TitleButtons::CheckInput()
{
	if (true == GameEngineInput::GetInst()->IsDown("W"))
	{
		SoundPlayer = GameEngineSound::SoundPlayControl("sound_menubeep_1.wav");
		SoundPlayer.Volume(0.3f);

		if (ButtonIndex >= 4)
		{
			return;
		}

		ButtonIndex++;
		Renderer_Selector->GetTransform().SetLocalPosition(ButtonsPos[ButtonIndex]);
	}

	if (true == GameEngineInput::GetInst()->IsDown("S"))
	{
		SoundPlayer = GameEngineSound::SoundPlayControl("sound_menubeep_1.wav");
		SoundPlayer.Volume(0.3f);

		if (ButtonIndex <= 0)
		{
			return;
		}

		ButtonIndex--;
		Renderer_Selector->GetTransform().SetLocalPosition(ButtonsPos[ButtonIndex]);
	}

	if (true == GameEngineInput::GetInst()->IsDown("SpaceBar"))
	{

		switch (ButtonIndex)
		{
		case 4:
			SoundPlayer = GameEngineSound::SoundPlayControl("sound_menubeep_2.wav");
			SoundPlayer.Volume(0.3f);
			IsPlayLevelChange = true;
			break;
		case 0:
			SoundPlayer = GameEngineSound::SoundPlayControl("sound_menubeep_2.wav");
			SoundPlayer.Volume(0.3f);
			GameEngineWindow::GetInst()->Off();
			break;
		//case 1:
		//	break;
		//case 2:
		//	break;
		//case 3:
		//	break;

		default:
			break;
		}
	}
}
