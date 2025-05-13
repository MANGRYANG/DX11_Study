#include "PCH.h" 
#include "Game.h"

Game::Game()
{
}
Game::~Game()
{
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
		D3D11_SDK_VERSION, &sd, m_pSwapChain.GetAddressOf(), m_pd3dDevice.GetAddressOf(), nullptr, m_pImmediateContext.GetAddressOf()
	);

	if (SUCCEEDED(hr) && m_pd3dDevice != nullptr && m_pImmediateContext != nullptr && m_pSwapChain != nullptr)
	{
		return true; // ���������� �ʱ�ȭ��
	}
	else
	{
		// ���� ó��
		OutputDebugString(L"Direct3D �ʱ�ȭ�� �����߽��ϴ�.\n");
		return false; // �ʱ�ȭ ����
	}
}

// Render �ڵ�  
void Game::Render()
{
	// �� ���� ��������
	if (!Game::GetBackBuffer())
	{
		return;
	}

	// ���� Ÿ�� �� ����
	if (!Game::CreateRenderTargetView())
	{
		return;
	}

	Game::RenderBegin();
	Game::RenderEnd();
}

void Game::RenderBegin()
{
	// ���� Ÿ�� �� ����
	m_pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), nullptr);
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
	m_pImmediateContext->RSSetViewports(1, &m_viewport);
}

void Game::RenderEnd()
{
	m_pSwapChain->Present(0, 0);
}

bool Game::GetBackBuffer()
{
	// ���� ü�ο��� ���� Ÿ�� ����(�� ����) ��������
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)m_pBackBuffer.GetAddressOf());

	if (FAILED(hr) || m_pBackBuffer == nullptr)
	{
		// ���� ó��
		OutputDebugString(L"���� ü�ο��� �� ���۸� �������� ���߽��ϴ�.\n");
		return false;
	}

	return true;
}

bool Game::CreateRenderTargetView()
{
	// ���� Ÿ�� �� ����
	HRESULT hr = m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf());

	if (FAILED(hr) || m_pRenderTargetView == nullptr)
	{
		// ���� ó�� 
		OutputDebugString(L"���� Ÿ�� �並 �������� ���߽��ϴ�.\n");
		return false;
	}

	return true;
}

// �� ��Ʈ ���� �Լ�
void Game::SetViewport()
{
	// �� ��Ʈ ����
	m_viewport.Width = static_cast<FLOAT>(SCREEN_WIDTH); // ����Ʈ �ʺ�
	m_viewport.Height = static_cast<FLOAT>(SCREEN_HEIGHT); // ����Ʈ ����
	m_viewport.MinDepth = 0.0f; // �ּ� ����
	m_viewport.MaxDepth = 1.0f; // �ִ� ����
	m_viewport.TopLeftX = 0; // ����Ʈ ���� X ��ǥ
	m_viewport.TopLeftY = 0; // ����Ʈ ���� Y ��ǥ
}