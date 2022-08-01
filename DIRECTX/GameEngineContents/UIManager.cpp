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
	GameEngineUIRenderer* TimerUI = CreateComponent<GameEngineUIRenderer>();
	TimerUI->SetTexture("spr_hud_timer_1.png");
	TimerUI->ScaleToTexture();
	TimerUI->GetTransform().SetLocalPosition({ 0, 350, GetDepth(ACTOR_DEPTH::UI)});

}

void UIManager::Update(float _DeltaTime)
{
}

void UIManager::End()
{
}
