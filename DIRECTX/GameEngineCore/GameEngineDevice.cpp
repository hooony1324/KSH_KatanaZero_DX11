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
		MsgBoxAssert("�����찡 ��������� �ʾҴµ� ����̽��� �ʱ�ȭ �Ϸ��� �߽��ϴ�.");
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
	//		// ���� �ڵ��� �׷���ī�� ����̹� ����� �ֳ���?
	//		nullptr,
	//		// �Ϲ����� ǥ���Ͽ��� ��������� �׷���ī�带 ����Ұ̴ϴ�.
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

