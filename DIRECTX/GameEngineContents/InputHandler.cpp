#include "PreCompile.h"
#include "InputHandler.h"
#include <GameEngineBase/GameEngineInput.h>

STATE_PLAYER InputHandler::HandleInput()
{
	if (true == GameEngineInput::GetInst()->IsPress("PlayerUp"))
	{
		return STATE_PLAYER::JUMP;
	}

	return STATE_PLAYER::IDLE;
}

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler() 
{
}

