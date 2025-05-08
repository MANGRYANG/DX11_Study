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

	// ���� Ÿ�� �� �ʱ�ȭ
	m_pRenderTargetView = nullptr;

	// �� ���� �ʱ�ȭ
	m_pBackBuffer = nullptr;
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

	if (SUCCEEDED(hr) && m_pd3dDevice != nullptr && m_pImmediateContext != nullptr && m_pSwapChain != nullptr)
	{
		return true; // ���������� �ʱ�ȭ��
	}
	else
	{
		// ���� ó��
		OutputDebugString(L"Direct3D �ʱ�ȭ�� �����߽��ϴ�.\n");
		Game::CleanupD3D(); // �ʱ�ȭ ���� �� ����
		return false; // �ʱ�ȭ ����
	}
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
	// �� ���� ��������
	if (!Game::GetBackBuffer())
	{
		Game::CleanupD3D();
		return;
	}

	// ���� Ÿ�� �� ����
	if (!Game::CreateRenderTargetView())
	{
		Game::CleanupD3D();
		return;
	}
	
	Game::SetRenderTarget(); // ���� Ÿ�� ����
	
	Game::SetViewport(); // ����Ʈ ����

	Game::SetBackgroundColor(1, 1, 1, 1); // ���� ����

	// ���� Ÿ�� ��� ���� Ÿ�� ���ۿ� �����Ͽ� �����ǹǷ�, ���� Ÿ�� �並 ���� �����ؾ� ��
	SAFE_RELEASE(m_pRenderTargetView); // ���� Ÿ�� �� ����
	SAFE_RELEASE(m_pBackBuffer); // �� ���� ����
	 
	Game::BufferSwap(); // ���� ����
}

bool Game::GetBackBuffer()
{
	// ���� ü�ο��� ���� Ÿ�� ����(�� ����) ��������
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pBackBuffer);

	if (FAILED(hr) || m_pBackBuffer == nullptr)
	{
		// ���� ó��
		OutputDebugString(L"���� ü�ο��� �� ���۸� �������� ���߽��ϴ�.\n");
		SAFE_RELEASE(m_pBackBuffer);
		return false;
	}

	return true;
}

bool Game::CreateRenderTargetView()
{
	// ���� Ÿ�� �� ����
	HRESULT hr = m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer, nullptr, &m_pRenderTargetView);

	if (FAILED(hr) || m_pRenderTargetView == nullptr)
	{
		// ���� ó�� 
		OutputDebugString(L"���� Ÿ�� �並 �������� ���߽��ϴ�.\n");
		SAFE_RELEASE(m_pRenderTargetView);
		return false;
	}

	return true;
}

// ���� Ÿ���� �����ϴ� �Լ�
void Game::SetRenderTarget()
{
	// ���� Ÿ�� �� ����
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
}

// �� ��Ʈ ���� �Լ�
void Game::SetViewport()
{
	// �� ��Ʈ ����
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<FLOAT>(SCREEN_WIDTH); // ����Ʈ �ʺ�
	viewport.Height = static_cast<FLOAT>(SCREEN_HEIGHT); // ����Ʈ ����
	viewport.MinDepth = 0.0f; // �ּ� ����
	viewport.MaxDepth = 1.0f; // �ִ� ����
	viewport.TopLeftX = 0; // ����Ʈ ���� X ��ǥ
	viewport.TopLeftY = 0; // ����Ʈ ���� Y ��ǥ

	m_pImmediateContext->RSSetViewports(1, &viewport);
}

// ������ �����ϴ� �Լ�
void Game::SetBackgroundColor(float r, float g, float b, float a)
{
	// ���� Ÿ���� ������ ������ �ʱ�ȭ
	const float clearColor[4] = { r, g, b, a };
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
}

void Game::BufferSwap()
{
	// ���� ü�� ���� ����
	m_pSwapChain->Present(0, 0);
}
