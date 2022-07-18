#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class Ending_Background : public GameEngineActor
{
public:
	// constrcuter destructer
	Ending_Background();
	~Ending_Background();

	// delete Function
	Ending_Background(const Ending_Background& _Other) = delete;
	Ending_Background(Ending_Background&& _Other) noexcept = delete;
	Ending_Background& operator=(const Ending_Background& _Other) = delete;
	Ending_Background& operator=(Ending_Background&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
private:

};

