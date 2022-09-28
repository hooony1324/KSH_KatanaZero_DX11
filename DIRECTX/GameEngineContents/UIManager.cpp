#include "PreCompile.h"
#include "UIManager.h"

UIManager::UIManager() 
	: InitialUIUpdate(false)
{
}

UIManager::~UIManager() 
{
}

void UIManager::Start()
{
	// 베이스
	GameEngineUIRenderer* Hud = CreateComponent<GameEngineUIRenderer>();
	Hud->SetTexture("spr_hud_0.png");
	Hud->GetTransform().SetLocalScale(Hud->GetCurTexture()->GetScale() * 2);
	Hud->SetSamplingModePoint();
	Hud->GetTransform().SetLocalPosition({ 0, 337, 0 });

	GameEngineUIRenderer* BatteryHud = CreateComponent<GameEngineUIRenderer>();
	BatteryHud->SetTexture("spr_hud_battery_0.png");
	BatteryHud->GetTransform().SetLocalScale(BatteryHud->GetCurTexture()->GetScale() * 2);
	BatteryHud->SetSamplingModePoint();
	BatteryHud->GetTransform().SetLocalPosition({ -560, 339, -1 });

	GameEngineUIRenderer* ShiftIcon = CreateComponent<GameEngineUIRenderer>();
	ShiftIcon->SetTexture("spr_keyboard_shift_0.png");
	ShiftIcon->GetTransform().SetLocalScale(ShiftIcon->GetCurTexture()->GetScale() * 2);
	ShiftIcon->SetSamplingModePoint();
	ShiftIcon->GetTransform().SetLocalPosition({ -450, 339, -1 });

	GameEngineUIRenderer* TimerHud = CreateComponent<GameEngineUIRenderer>();
	TimerHud->SetTexture("spr_hud_timer_1.png");
	TimerHud->GetTransform().SetLocalScale(TimerHud->GetCurTexture()->GetScale() * 2);
	TimerHud->GetTransform().SetLocalPosition({ 0, 339, -1 });

	GameEngineUIRenderer* SubweaponHud = CreateComponent<GameEngineUIRenderer>();
	SubweaponHud->SetTexture("spr_hud_subweapon_0.png");
	SubweaponHud->GetTransform().SetLocalScale(SubweaponHud->GetCurTexture()->GetScale() * 2);
	SubweaponHud->GetTransform().SetLocalPosition({ 560, 337, -1 });

	GameEngineUIRenderer* KatanaIcon = CreateComponent<GameEngineUIRenderer>();
	KatanaIcon->SetTexture("spr_katanaicons_0.png");
	KatanaIcon->GetTransform().SetLocalScale(KatanaIcon->GetCurTexture()->GetScale() * 2);
	KatanaIcon->GetTransform().SetLocalPosition({ 530, 337, -2 });

	GameEngineUIRenderer* ItemIcon = CreateComponent<GameEngineUIRenderer>();
	ItemIcon->SetTexture("spr_itemicons_0.png");
	ItemIcon->GetTransform().SetLocalScale(ItemIcon->GetCurTexture()->GetScale() * 2);
	ItemIcon->GetTransform().SetLocalPosition({ 590, 337, -2 });

	GameEngineUIRenderer* MouseLeftIcon = CreateComponent<GameEngineUIRenderer>();
	MouseLeftIcon->SetTexture("spr_left_click_1.png");
	MouseLeftIcon->GetTransform().SetLocalScale(MouseLeftIcon->GetCurTexture()->GetScale() * 2);
	MouseLeftIcon->SetSamplingModePoint();
	MouseLeftIcon->GetTransform().SetLocalPosition({ 545, 315, -3 });

	GameEngineUIRenderer* MouseRightIcon = CreateComponent<GameEngineUIRenderer>();
	MouseRightIcon->SetTexture("spr_right_click_1.png");
	MouseRightIcon->GetTransform().SetLocalScale(MouseRightIcon->GetCurTexture()->GetScale() * 2);
	MouseRightIcon->SetSamplingModePoint();
	MouseRightIcon->GetTransform().SetLocalPosition({ 610, 315, -3 });


	TimerBar = CreateComponent<GameEngineUIRenderer>();
	TimerBar->SetTexture("spr_timer_0.png");
	TimerBar->GetTransform().SetLocalScale(TimerBar->GetCurTexture()->GetScale() * 2);
	TimerBar->SetPivot(PIVOTMODE::LEFT);
	ScaleTimerBar = TimerBar->GetTransform().GetLocalScale();
	TimerBar->GetTransform().SetLocalPosition({ -78, 343, -2 });

	RestartClickUI = CreateComponent<GameEngineUIRenderer>();
	RestartClickUI->SetTexture("spr_itwontwork.png");
	RestartClickUI->GetTransform().SetLocalScale(RestartClickUI->GetCurTexture()->GetScale());
	RestartClickUI->GetTransform().SetLocalPosition({ 0, 0, 0 });
	RestartClickUI->Off();

	float PaddingX = 2.0f;
	for (int i = 0; i < 11; i++)
	{
		GameEngineUIRenderer* Battery = CreateComponent<GameEngineUIRenderer>();
		Battery->SetTexture("spr_battery_part_0.png");
		float4 Scale = Battery->GetCurTexture()->GetScale() * 2;
		Battery->GetTransform().SetLocalScale(Scale);
		Battery->GetTransform().SetLocalPosition({ -611 + i * Scale.x + i * PaddingX, 340, -1 });
		SlowBatteries.push_back(Battery);
	}

	CurSlowBatteryIndex = 10;

	// 룸 최초시작 UI
	MouseClick = CreateComponent<GameEngineUIRenderer>();
	MouseClick->CreateFrameAnimationFolder("clicking", FrameAnimation_DESC{ "leftmouseclick", 0.8f });
	MouseClick->SetScaleRatio(2.0f);
	MouseClick->SetScaleModeImage();
	MouseClick->SetSamplingModePoint();
	MouseClick->ChangeFrameAnimation("clicking");
	MouseClick->GetTransform().SetLocalPosition({ 140, -90});
	MouseClick->Off();

	ClickToStart = CreateComponent<GameEngineUIRenderer>();
	ClickToStart->CreateFrameAnimationFolder("clicktostart", FrameAnimation_DESC{ "clicktostart", 0.8f });
	ClickToStart->SetScaleModeImage();
	ClickToStart->SetSamplingModePoint();
	ClickToStart->ChangeFrameAnimation("clicktostart");
	ClickToStart->GetTransform().SetLocalPosition({ 0, -90, 0 });
	ClickToStart->Off();

	// 재생중인 곡
	SongTitleBG = CreateComponent<GameEngineUIRenderer>();
	SongTitleBG->SetTexture("spr_songtitleBG.png");
	SongTitleBG->ScaleToTexture();
	SongTitleBG->GetTransform().SetLocalPosition({ 400, -300, 10 });
	SongTitleBG->Off();
	SongTitleBG->ShaderResources.SetConstantBufferLink("AtlasData", SongTitleBGData);
	SongTitleBGData.FrameData.PosX = 0.0f;
	SongTitleBGData.FrameData.PosY = 0.0f;
	SongTitleBGData.FrameData.SizeX = 1.0f;
	SongTitleBGData.FrameData.SizeY = 1.0f;
	SongTitleBGData.PivotPos = float4::ZERO;


	PlayingSongText1 = CreateComponent<GameEngineUIRenderer>();
	PlayingSongText1->SetTexture("spr_playingsong1.png");
	PlayingSongText1->ScaleToTexture();
	PlayingSongText1->SetSamplingModePoint();
	PlayingSongText1->GetTransform().SetLocalPosition({ 440, -300 });
	PlayingSongText1->GetPixelData().MulColor.a = 0.0f;
	PlayingSongText1->Off();

	PlayingSongText2 = CreateComponent<GameEngineUIRenderer>();
	PlayingSongText2->SetTexture("spr_playingsong2.png");
	PlayingSongText2->ScaleToTexture();
	PlayingSongText2->SetSamplingModePoint();
	PlayingSongText2->GetTransform().SetLocalPosition({ 390, -300, 9});
	PlayingSongText2->GetPixelData().MulColor.a = 0.0f;
	PlayingSongText2->Off();

	PlayingSongTextUnderline = CreateComponent<GameEngineUIRenderer>();
	PlayingSongTextUnderline->SetTexture("spr_songunderline.png");
	PlayingSongTextUnderline->ScaleToTexture();
	PlayingSongTextUnderline->SetSamplingModePoint();
	PlayingSongTextUnderline->GetTransform().SetLocalPosition({ 405, -300 });
	PlayingSongTextUnderline->Off();
	PlayingSongTextUnderline->ShaderResources.SetConstantBufferLink("AtlasData", SongUnderlineData);
	SongUnderlineData.FrameData.PosX = 0.0f;
	SongUnderlineData.FrameData.PosY = 0.0f;
	SongUnderlineData.FrameData.SizeX = 1.0f;
	SongUnderlineData.FrameData.SizeY = 1.0f;
	SongUnderlineData.PivotPos = float4::ZERO;

	SongTitleText = CreateComponent<GameEngineUIRenderer>();
	SongTitleText->SetTexture("spr_songtitletext.png");
	SongTitleText->ScaleToTexture();
	SongTitleText->SetSamplingModePoint();
	SongTitleText->GetTransform().SetLocalPosition({ 400, -300 });
	SongTitleText->GetPixelData().MulColor.a = 0.0f;
	SongTitleText->Off();
	// ~ 재생중인 곡


	StageName1 = CreateComponent<GameEngineUIRenderer>();
	StageName1->SetTexture("spr_stagename1.png");
	StageName1->GetTransform().SetLocalPosition({ 200, 0, 0 });
	StageName1->ScaleToTexture();
	StageName1->Off();

	StageName2 = CreateComponent<GameEngineUIRenderer>();
	StageName2->SetTexture("spr_stagename2.png");
	StageName2->GetTransform().SetLocalPosition({ -200, 0, 9 });
	StageName2->ScaleToTexture();
	StageName2->Off();

	StageNameBG = CreateComponent<GameEngineUIRenderer>();
	StageNameBG->SetTexture("None.png");
	StageNameBG->GetTransform().SetLocalScale({ 1280, 100 });
	StageNameBG->GetTransform().SetLocalPosition({ 0, 0, 10 });
	StageNameBG->GetPixelData().MulColor.a = 0.1f;
	StageNameBG->Off();

	InitalRoomUI.push_back(MouseClick);
	InitalRoomUI.push_back(ClickToStart);
	InitalRoomUI.push_back(SongTitleBG);
	InitalRoomUI.push_back(PlayingSongText1);
	InitalRoomUI.push_back(PlayingSongText2);
	InitalRoomUI.push_back(PlayingSongTextUnderline);
	InitalRoomUI.push_back(SongTitleText);
	InitalRoomUI.push_back(StageName1);
	InitalRoomUI.push_back(StageName2);
	InitalRoomUI.push_back(StageNameBG);

	InitialUIManager.CreateStateMember("SongTitle",
		std::bind(&UIManager::SongTitleUpdate, this, std::placeholders::_1, std::placeholders::_2),
		std::bind(&UIManager::SongTitleStart, this, std::placeholders::_1));

	InitialUIManager.CreateStateMember("StageName",
		std::bind(&UIManager::StageNameUpdate, this, std::placeholders::_1, std::placeholders::_2),
		std::bind(&UIManager::StageNameStart, this, std::placeholders::_1));

	InitialUIManager.ChangeState("SongTitle");
}

void UIManager::Update(float _DeltaTime)
{

	// 룸 최초시작 UI
	if (true == InitialUIUpdate)
	{
		InitialUIManager.Update(_DeltaTime);
	}

	if (RestartClickUI->IsUpdate())
	{
		RestartClickUI->GetPixelData().MulColor.a = 1.0f + sinf(GetAccTime() * 200);
	}




	// 슬로우 배터리
	int NewTimeBatteryIndex = GlobalValueManager::SlowEnergy - 1;
	
	if (CurSlowBatteryIndex == NewTimeBatteryIndex)
	{
		return;
	}

	CurSlowBatteryIndex = NewTimeBatteryIndex;

	for (int i = 0; i < 11; i++)
	{
		// 0 ~ i번까지 푸른색
		if (i <= CurSlowBatteryIndex)
		{
			SlowBatteries[i]->GetPixelData().MulColor.r = 1.0f;
			SlowBatteries[i]->GetPixelData().MulColor.g = 1.0f;
			SlowBatteries[i]->GetPixelData().MulColor.b = 1.0f;
		}
		// 나머지 붉은색
		else
		{
			SlowBatteries[i]->GetPixelData().MulColor.r = 1.0f;
			SlowBatteries[i]->GetPixelData().MulColor.g = 0.0f;
			SlowBatteries[i]->GetPixelData().MulColor.b = 0.0f;
		}
	}
	// ~슬로우 배터리

}

void UIManager::End()
{
}

void UIManager::SongTitleStart(const StateInfo& _Info)
{
	SongTitleBG->On();
	PlayingSongText1->On();
	PlayingSongText2->On();
	PlayingSongTextUnderline->On();
	SongTitleText->On();

	SongTitleBGData.FrameData.PosX = 1.0f;
	SongUnderlineData.FrameData.PosX = 1.0f;
}

void UIManager::SongTitleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (SongTitleBGData.FrameData.PosX > 0.0f)
	{
		SongTitleBGData.FrameData.PosX = GameEngineMath::Lerp(SongTitleBGData.FrameData.PosX, 0.0f, _DeltaTime * 2.0f);
	}

	if (SongTitleBGData.FrameData.PosX <= 0.05f)
	{
		SongTitleBGData.FrameData.PosX = 0.0f;
	}

	// 재생 중인 곡 진하게
	if (SongTitleBGData.FrameData.PosX <= 0.1f)
	{
		PlayingSongText1->GetPixelData().MulColor.a = GameEngineMath::Lerp(PlayingSongText1->GetPixelData().MulColor.a, 1.0f, _DeltaTime * 2.0f);
		PlayingSongText1->GetTransform().SetLocalPosition(
			float4::Lerp(PlayingSongText1->GetTransform().GetLocalPosition(), float4{ 400, -300 }, _DeltaTime * 2.0f));

		PlayingSongText2->GetPixelData().MulColor.a = GameEngineMath::Lerp(PlayingSongText2->GetPixelData().MulColor.a, 1.0f, _DeltaTime * 2.0f);
		PlayingSongText2->GetTransform().SetLocalPosition(
			float4::Lerp(PlayingSongText2->GetTransform().GetLocalPosition(), float4{ 400, -300 }, _DeltaTime * 2.0f));

	}

	// 밑줄
	if (PlayingSongText1->GetPixelData().MulColor.a > 0.7f)
	{
		SongUnderlineData.FrameData.PosX = GameEngineMath::Lerp(SongUnderlineData.FrameData.PosX, 0.0f, _DeltaTime * 5.0f);
		SongTitleText->GetPixelData().MulColor.a = GameEngineMath::Lerp(SongTitleText->GetPixelData().MulColor.a, 1.0f, _DeltaTime * 2.0f);
	}

	if (SongTitleText->GetPixelData().MulColor.a >= 0.9f)
	{
		SongTitleText->GetPixelData().MulColor.a = 1.0f;
		InitialUIManager.ChangeState("StageName");
		return;
	}
}

float VibrationSumTime;
void UIManager::StageNameStart(const StateInfo& _Info)
{
	StageName1->On();
	StageName2->On();
	StageNameBG->On();
	MouseClick->On();
	ClickToStart->On();
	
	VibrationSumTime = 0.0f;
}

void UIManager::StageNameUpdate(float _DeltaTime, const StateInfo& _Info)
{
	StageNameBG->GetPixelData().MulColor.a = GameEngineMath::Lerp(StageNameBG->GetPixelData().MulColor.a, 0.7f, _DeltaTime * 3.0f);
	
	// 깜빡거림
	PlayingSongText1->GetPixelData().MulColor.a = 1.0f + sinf(_Info.StateTime * 200.0f);

	StageName1->GetTransform().SetLocalPosition(float4::Lerp(StageName1->GetTransform().GetLocalPosition(), {0,0,0}, _DeltaTime * 5.0f));
	StageName2->GetTransform().SetLocalPosition(float4::Lerp(StageName2->GetTransform().GetLocalPosition(), { 0,0,9 }, _DeltaTime * 5.0f));

	if (_Info.StateTime > 1.0f)
	{
		Vibration(VibrationSumTime, StageName1);
		Vibration(VibrationSumTime, StageName2);

		VibrationSumTime += _DeltaTime;
	}
}

void UIManager::Vibration(float _RendererSumTime, GameEngineUIRenderer* _Renderer)
{
	float ShakeX = sinf(_RendererSumTime * 150.0f);
	_Renderer->GetTransform().SetWorldMove({ ShakeX * 1.0f, 0, 0 });
}
