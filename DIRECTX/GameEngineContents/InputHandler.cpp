#include "PreCompile.h"
#include "InputHandler.h"
#include <GameEngineBase/GameEngineInput.h>

InputHandler::InputHandler()
{
	if (GameEngineInput::GetInst()->IsKey("W"))
	{
		return;
	}

	GameEngineInput::GetInst()->CreateKey("W", 'W');
	GameEngineInput::GetInst()->CreateKey("A", 'A');
	GameEngineInput::GetInst()->CreateKey("S", 'S');
	GameEngineInput::GetInst()->CreateKey("D", 'D');
	GameEngineInput::GetInst()->CreateKey("SpaceBar", VK_SPACE);


	GameEngineInput::GetInst()->CreateKey( "Numpad0", VK_NUMPAD0);
	GameEngineInput::GetInst()->CreateKey( "Numpad1", VK_NUMPAD1);
	GameEngineInput::GetInst()->CreateKey( "Numpad2", VK_NUMPAD2);
	GameEngineInput::GetInst()->CreateKey( "Numpad3", VK_NUMPAD3);
	GameEngineInput::GetInst()->CreateKey( "Numpad4", VK_NUMPAD4);
	GameEngineInput::GetInst()->CreateKey( "Numpad5", VK_NUMPAD5);
	GameEngineInput::GetInst()->CreateKey( "Numpad6", VK_NUMPAD6);
	GameEngineInput::GetInst()->CreateKey( "Numpad7", VK_NUMPAD7);
	GameEngineInput::GetInst()->CreateKey( "Numpad8", VK_NUMPAD9);
	GameEngineInput::GetInst()->CreateKey( "Numpad9", VK_NUMPAD9);

	GameEngineInput::GetInst()->CreateKey("F1", VK_F1);
	GameEngineInput::GetInst()->CreateKey("F2", VK_F2);
	GameEngineInput::GetInst()->CreateKey("F3", VK_F3);
	GameEngineInput::GetInst()->CreateKey("F4", VK_F4);
	GameEngineInput::GetInst()->CreateKey("F5", VK_F5);
	GameEngineInput::GetInst()->CreateKey("F6", VK_F6);
	GameEngineInput::GetInst()->CreateKey("F7", VK_F7);
	GameEngineInput::GetInst()->CreateKey("F8", VK_F8);
	GameEngineInput::GetInst()->CreateKey("F9", VK_F9);
	GameEngineInput::GetInst()->CreateKey("F10", VK_F10);
	GameEngineInput::GetInst()->CreateKey("F11", VK_F11);
	GameEngineInput::GetInst()->CreateKey("F12", VK_F12);
	
}

InputHandler::~InputHandler() 
{
}

