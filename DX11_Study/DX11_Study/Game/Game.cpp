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

void Game::Render()
{
	// Render �ڵ�
	
}
