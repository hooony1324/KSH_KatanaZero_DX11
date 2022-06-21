#include "PreCompile.h"
#include "GameEngineDevice.h"

GameEngineDevice* GameEngineDevice::Inst = new GameEngineDevice();
ID3D11Device* GameEngineDevice::Device_ = nullptr;
ID3D11DeviceContext* GameEngineDevice::Context_ = nullptr;
IDXGISwapChain* GameEngineDevice::SwapChain_ = nullptr;

GameEngineDevice::GameEngineDevice() 
{
}

GameEngineDevice::~GameEngineDevice() 
{
	if (nullptr != SwapChain_)
	{
		SwapChain_->Release();
		SwapChain_ = nullptr;
	}

	if (nullptr != Context_)
	{
		Context_->Release();
		Context_ = nullptr;
	}

	if (nullptr != Device_)
	{
		Device_->Release();
		Device_ = nullptr;
	}
}

void GameEngineDevice::Initialize() 
{
	if (nullptr == GameEngineWindow::GetHWND())
	{
		MsgBoxAssert("윈도우가 만들어지지 않았는데 디바이스를 초기화 하려고 했습니다.");
		return;
	}

	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 
	D3D_FEATURE_LEVEL Level = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	// soft

	//if (
	//	S_OK != D3D11CreateDevice(
	//		// 내가 코딩한 그래픽카드 드라이버 방법이 있나요?
	//		nullptr,
	//		// 일반적인 표준하에서 만들어지는 그래픽카드를 사용할겁니다.
	//		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
	//		nullptr,
	//		iFlag,
	//		nullptr, 
	//		0, 
	//		D3D11_SDK_VERSION,

	//	)
	//	)
	//{

	//}
}

