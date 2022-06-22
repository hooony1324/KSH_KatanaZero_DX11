#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineWindow.h>

// 그래픽카드에 연결

// 설명 :
class GameEngineDevice
{
private:
	static GameEngineDevice* Inst;

public:
	static GameEngineDevice& GetInst()
	{
		return *Inst;
	}

	static void Destroy()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

public:
	~GameEngineDevice();

	// delete Function
	GameEngineDevice(const GameEngineDevice& _Other) = delete;
	GameEngineDevice(GameEngineDevice&& _Other) noexcept = delete;
	GameEngineDevice& operator=(const GameEngineDevice& _Other) = delete;
	GameEngineDevice& operator=(GameEngineDevice&& _Other) noexcept = delete;

	static void Initialize();
	static void DeviceCreate();
	static void CreateSwapChain();

protected:

private:
	// 다이렉트 11로 오면서 업무분담을 하게 인터페이스를 변경했다.

	// 다이렉트 9때는 디바이스밖에 없었는데.
	// 리소스와 메모리 분야를 맡습니다.
	// 그래픽카드에 저장될 텍스처
	static ID3D11Device* Device_;

	// 그래픽카드에 연산을 명령할 수 있게 되는데.
	// 랜더링과 관련된 모든 명령은 이걸로 내린다.
	static ID3D11DeviceContext* Context_;

	// api의 백버퍼
	static IDXGISwapChain* SwapChain_;

	GameEngineDevice();
};

