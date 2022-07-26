#include "PreCompile.h"
#include "Cursor.h"
#include <GameEngineCore/CoreMinimal.h>

float4 Cursor::Position = 0.0f;
float4 Cursor::PositionToActor = 0.0f;

Cursor::Cursor() 
{
}

Cursor::~Cursor() 
{
}

void Cursor::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_cursor_0.png");
	Renderer->ScaleToTexture();
	Renderer->GetTransform().SetLocalPosition({ 0, 0, GetDepth(ACTOR_DEPTH::CURSOR) });
	GetTransform().SetLocalScale({1.5f, 1.5f, 1 });

}

void Cursor::Update(float _DeltaTime)
{
	Position = GetLevel()->GetMainCamera()->GetMouseWorldPositionToActor();
	PositionToActor = GetLevel()->GetMainCamera()->GetMouseWorldPosition();
	GetTransform().SetWorldPosition({ Position.x, Position.y });

	
}

void Cursor::End()
{

}
