#include "PreCompile.h"
#include "UIManager.h"
#include <GameEngineCore/CoreMinimal.h>

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
	Hud->GetTransform().SetLocalPosition({ 0, 330, 0 });


	GameEngineUIRenderer* TimerHud = CreateComponent<GameEngineUIRenderer>();
	TimerHud->SetTexture("spr_hud_timer_1.png");
	TimerHud->GetTransform().SetLocalScale(TimerHud->GetCurTexture()->GetScale() * 2);
	TimerHud->GetTransform().SetLocalPosition({ 0, 330, -1 });

	GameEngineUIRenderer* Timer = CreateComponent<GameEngineUIRenderer>();
	Timer->SetTexture("spr_timer_0.png");
	Timer->GetTransform().SetLocalScale(Timer->GetCurTexture()->GetScale() * 2);
	Timer->GetTransform().SetLocalPosition({ 0, 330, -2 });
}

void UIManager::Update(float _DeltaTime)
{
}

void UIManager::End()
{
}
