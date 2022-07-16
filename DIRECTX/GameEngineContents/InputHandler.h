#pragma once

// Ό³Έν :
class InputHandler
{
public:
	




private:

	InputHandler();
	~InputHandler();

	// delete Function
	InputHandler(const InputHandler& _Other) = delete;
	InputHandler(InputHandler&& _Other) noexcept = delete;
	InputHandler& operator=(const InputHandler& _Other) = delete;
	InputHandler& operator=(InputHandler&& _Other) noexcept = delete;
};

