#include "PCH.h" 
#include "Game.h"

Game::Game()
{
	// Direct3D ����̽� �ʱ�ȭ
	m_pd3dDevice = nullptr;

	// Direct3D ����̽� ���ؽ�Ʈ �ʱ�ȭ
	m_pImmediateContext = nullptr;

	// ���� ü�� �ʱ�ȭ
	m_pSwapChain = nullptr;
}
Game::~Game()
{
	// Direct3D ����
	CleanupD3D();
}

// Direct3D �ʱ�ȭ �Լ�
bool Game::InitD3D(HWND hWnd)
{
	// DXGI_SWAP_CHAIN_DESC ����ü �ʱ�ȭ
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1; // ���� ü�� ���� ����
	sd.BufferDesc.Width = SCREEN_WIDTH;  // ������ ������ �ʺ�
	sd.BufferDesc.Height = SCREEN_HEIGHT; // ������ ������ ����
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �ȼ� ����
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // ���� Ÿ������ ���
	sd.OutputWindow = hWnd; // �������� ������ �ڵ�
	sd.SampleDesc.Count = 1; // ��Ƽ ���ø� ��Ȱ��ȭ
	sd.Windowed = TRUE; // â ���

	HRESULT hr = D3D11CreateDeviceAndSwapChain( //  Direct3D ����̽��� ���� ü�� ����
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
		D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, nullptr, &m_pImmediateContext
	);

	return SUCCEEDED(hr);   // ���� ���� ��ȯ
}

// Direct3D ���� �Լ�
void Game::CleanupD3D()
{
	// Direct3D ���ҽ� ����
	if (m_pImmediateContext) {
		m_pImmediateContext->ClearState();
		m_pImmediateContext->Release();
		m_pImmediateContext = nullptr;
	}

	// ���� ü�� ����
	if (m_pSwapChain) {
		m_pSwapChain->Release();
		m_pSwapChain = nullptr;
	}

	// ����̽� ����
	if (m_pd3dDevice) {
		m_pd3dDevice->Release();
		m_pd3dDevice = nullptr;
	}
}

// Render �ڵ�  
void Game::Render()
{
	// ���� ü�ο��� ���� Ÿ�� ����(�� ����) ��������  
	ID3D11Texture2D* pBackBuffer = nullptr;
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (FAILED(hr) || pBackBuffer == nullptr)
	{
		// ���� ó��
		OutputDebugString(L"���� ü�ο��� �� ���۸� �������� ���߽��ϴ�.\n");
		return;
	}

	// ���� Ÿ�� �� ����  
	ID3D11RenderTargetView* pRenderTargetView = nullptr;
	hr = m_pd3dDevice->
		CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);

	if (FAILED(hr) || pRenderTargetView == nullptr)
	{
		// ���� ó�� 
		OutputDebugString(L"���� Ÿ�� �並 �������� ���߽��ϴ�.\n");
		pBackBuffer->Release();
		return;
	}

	// ���� Ÿ�� �並 ��� ������� ����
	m_pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

	// �� ��Ʈ ����
	D3D11_VIEWPORT viewport = {};
	viewport.Width = SCREEN_WIDTH; // ����Ʈ �ʺ�
	viewport.Height = SCREEN_HEIGHT; // ����Ʈ ����
	viewport.MinDepth = 0.0f; // �ּ� ����
	viewport.MaxDepth = 1.0f; // �ִ� ����
	viewport.TopLeftX = 0; // ����Ʈ ���� X ��ǥ
	viewport.TopLeftY = 0; // ����Ʈ ���� Y ��ǥ

    m_pImmediateContext->RSSetViewports(1, &viewport);

	// ���� Ÿ���� ������ ������ �ʱ�ȭ
	const float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // ��� ���  
	m_pImmediateContext->ClearRenderTargetView(pRenderTargetView, clearColor);

    // ���� Ÿ�� �� ����  
    pRenderTargetView->Release();  
    pBackBuffer->Release();  
	 
	// ���� �������� ȭ�鿡 ��� 
	m_pSwapChain->Present(1, 0);
} 
