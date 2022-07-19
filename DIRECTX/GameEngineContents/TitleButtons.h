#pragma once

// Ό³Έν :
class TitleButtons
{
public:
	// constrcuter destructer
	TitleButtons();
	~TitleButtons();

	// delete Function
	TitleButtons(const TitleButtons& _Other) = delete;
	TitleButtons(TitleButtons&& _Other) noexcept = delete;
	TitleButtons& operator=(const TitleButtons& _Other) = delete;
	TitleButtons& operator=(TitleButtons&& _Other) noexcept = delete;

protected:

private:

};

