#pragma once

// ���� :
class InputHandler
{
public:
	STATE_PLAYER HandleInput();

public:
	// constrcuter destructer
	InputHandler();
	~InputHandler();

	// delete Function
	InputHandler(const InputHandler& _Other) = delete;
	InputHandler(InputHandler&& _Other) noexcept = delete;
	InputHandler& operator=(const InputHandler& _Other) = delete;
	InputHandler& operator=(InputHandler&& _Other) noexcept = delete;

protected:

private:

};

