#include "PreCompile.h"
#include "UIManager.h"

UIManager::UIManager() 
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
}

void UIManager::Update(float _DeltaTime)
{
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
}

void UIManager::End()
{
}
