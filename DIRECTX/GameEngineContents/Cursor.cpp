#include "PreCompile.h"
#include "Cursor.h"
#include <GameEngineContents/ContentsCore.h>

float4 Cursor::Position = 0.0f;
float4 Cursor::PositionToActor = 0.0f;
bool Cursor::CursorValid;

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

	Boundary = ContentsCore::GetContentsWindowSize();

}

void Cursor::Update(float _DeltaTime)
{

	Position = GetLevel()->GetMainCamera()->GetMouseWorldPosition();
	PositionToActor = GetLevel()->GetMainCamera()->GetMouseWorldPositionToActor();
	GetTransform().SetWorldPosition({ Position.x, Position.y, GetDepth(ACTOR_DEPTH::CURSOR) });

	//float4 Pos = GetTransform().GetWorldPosition();
	//std::string Output = std::to_string(Pos.x) + " / " + std::to_string(Pos.y) + " / " + std::to_string(Pos.z);
	//GameEngineDebug::OutPutString(Output);
	CursorVisibleOutScreen();

}

void Cursor::End()
{

}

void Cursor::CursorVisibleOutScreen()
{
	if (PositionToActor.x > 0 && PositionToActor.x <= 1280.0f
		&& PositionToActor.y <= -64.0f && PositionToActor.y >= -784.0f)
	{
		CursorValid = true;
		ShowCursor(false);
	}
	else
	{
		CursorValid = false;
		ShowCursor(true);
	}
}
