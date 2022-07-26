#include "PreCompile.h"
#include "TitleGraphic.h"
#include <GameEngineCore/CoreMinimal.h>

const float LERPIN_DIST = 125.0f;

TitleGraphic::TitleGraphic() 
{
}

TitleGraphic::~TitleGraphic() 
{
}

void TitleGraphic::Start()
{
	Renderer_L = CreateComponent<GameEngineTextureRenderer>();
	Renderer_L->SetTexture("spr_titlegraphic_1.png");
	Renderer_L->ScaleToTexture();
	Renderer_L->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(TITLE_DEPTH::GRAPHIC) });

	Renderer_R = CreateComponent<GameEngineTextureRenderer>();
	Renderer_R->SetTexture("spr_titlegraphic_2.png");
	Renderer_R->ScaleToTexture();
	Renderer_R->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(TITLE_DEPTH::GRAPHIC) });

	Renderer_Logo = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Logo->SetTexture("spr_titlegraphic_big2_0.png");
	Renderer_Logo->ScaleToTexture();
	Renderer_Logo->GetTransform().SetLocalPosition({ 0, 50, 0 });
	Renderer_Logo->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(TITLE_DEPTH::GRAPHIC)-1 });

	GetTransform().SetLocalScale({ 1.3f, 1.3f, 1 });


	// Lerp
	LerpStart({ 0, -100 }, { 0, 25 }, 0.5f);
}

void TitleGraphic::Update(float _DeltaTime)
{
	LerpUpdate(_DeltaTime);
}

void TitleGraphic::End()
{
}
