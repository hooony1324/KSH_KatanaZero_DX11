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
	GetTransform().SetLocalScale({2, 2, 1 });


}

void Cursor::Update(float _DeltaTime)
{
	Position = GetLevel()->GetMainCamera()->GetMouseWorldPositionToActor();
	PositionToActor = GetLevel()->GetMainCamera()->GetMouseWorldPosition();
	GetTransform().SetWorldPosition({ PositionToActor.x, PositionToActor.y });

	
}

void Cursor::End()
{

}
