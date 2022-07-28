#include "PreCompile.h"
#include "TitleButtons.h"
#include <GameEngineCore/GEngine.h>

#include <GameEngineBase/GameEngineWindow.h>



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
	Renderer_Background = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Background->SetTexture("spr_title_background_black_alpha.png");
	Renderer_Background->GetTransform().SetLocalScale({ 530, 227, 1, 0.2f });
	Renderer_Background->GetTransform().SetLocalPosition({ 0, 0, 0 });

	Renderer_Buttons = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Buttons->SetTexture("TitleButtons.png");
	Renderer_Buttons->ScaleToTexture();
	Renderer_Buttons->GetTransform().SetLocalPosition({ 0, 0, -1 });

	// Selector
	Renderer_Selector = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Selector->SetTexture("spr_title_background_white.png");
	Renderer_Selector->GetTransform().SetLocalScale({ 510, 42, 1 });
	Renderer_Selector->GetTransform().SetLocalPosition({ 0, 0, -2 });

	float Depth = static_cast<float>(TITLE_DEPTH::BUTTONS);
	ButtonsPos.push_back({ 0, -84 , Depth - 2 });
	ButtonsPos.push_back({ 0, -44 , Depth - 2 });
	ButtonsPos.push_back({ 0, -4 , Depth - 2 });
	ButtonsPos.push_back({ 0, 36 , Depth - 2 });
	ButtonsPos.push_back({ 0, 76 , Depth - 2 });
	ButtonIndex = 2;
	Renderer_Selector->GetTransform().SetLocalPosition(ButtonsPos[ButtonIndex]);

	// 초기세팅
	IsPlayLevelChange = false;

	// Lerp 
	LerpStart({ 0, -1000, Depth}, { 0, -200, Depth }, 0.9f);

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
