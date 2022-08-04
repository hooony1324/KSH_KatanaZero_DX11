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
	Renderer = CreateComponent<GameEngineUIRenderer>();
	Renderer->SetTexture("spr_cursor_0.png");
	Renderer->ScaleToTexture();

	GetTransform().SetLocalScale({1.5f, 1.5f, 1 });


	ShowCursor(false);
}

void Cursor::Update(float _DeltaTime)
{
	Position = GetLevel()->GetMainCamera()->GetMouseWorldPositionToActor();
	PositionToActor = GetLevel()->GetMainCamera()->GetMouseWorldPosition();
	GetTransform().SetWorldPosition({ PositionToActor.x, PositionToActor.y, GetDepth(ACTOR_DEPTH::CURSOR) });

	//float4 Pos = GetTransform().GetWorldPosition();
	//std::string Output = std::to_string(Pos.x) + " / " + std::to_string(Pos.y) + " / " + std::to_string(Pos.z);
	//GameEngineDebug::OutPutString(Output);
}

void Cursor::End()
{

}
