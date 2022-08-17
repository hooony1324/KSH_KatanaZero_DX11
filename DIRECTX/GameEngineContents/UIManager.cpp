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
	// ∫£¿ÃΩ∫
	GameEngineUIRenderer* Hud = CreateComponent<GameEngineUIRenderer>();
	Hud->SetTexture("spr_hud_0.png");
	Hud->GetTransform().SetLocalScale(Hud->GetCurTexture()->GetScale() * 2);
	Hud->GetTransform().SetLocalPosition({ 0, 337, 0 });


	GameEngineUIRenderer* TimerHud = CreateComponent<GameEngineUIRenderer>();
	TimerHud->SetTexture("spr_hud_timer_1.png");
	TimerHud->GetTransform().SetLocalScale(TimerHud->GetCurTexture()->GetScale() * 2);
	TimerHud->GetTransform().SetLocalPosition({ 0, 339, -1 });

	TimerBar = CreateComponent<GameEngineUIRenderer>();
	TimerBar->SetTexture("spr_timer_0.png");
	TimerBar->GetTransform().SetLocalScale(TimerBar->GetCurTexture()->GetScale() * 2);
	TimerBar->SetPivot(PIVOTMODE::LEFTTOP);
	TimerBar->GetTransform().SetLocalPosition({ 16, 341, -2 });

	RestartClickUI = CreateComponent<GameEngineUIRenderer>();
	RestartClickUI->SetTexture("spr_itwontwork.png");
	RestartClickUI->GetTransform().SetLocalScale(RestartClickUI->GetCurTexture()->GetScale());
	RestartClickUI->GetTransform().SetLocalPosition({ 0, 0, 0 });
	RestartClickUI->Off();

}

void UIManager::Update(float _DeltaTime)
{
}

void UIManager::End()
{
}
