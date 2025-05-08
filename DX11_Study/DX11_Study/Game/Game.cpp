#include "PCH.h" 
#include "Game.h"

Game::Game()
{
	// Direct3D 디바이스 초기화
	m_pd3dDevice = nullptr;

	// Direct3D 디바이스 컨텍스트 초기화
	m_pImmediateContext = nullptr;

	// 스왑 체인 초기화
	m_pSwapChain = nullptr;

	// 렌더 타겟 뷰 초기화
	m_pRenderTargetView = nullptr;

	// 백 버퍼 초기화
	m_pBackBuffer = nullptr;
}
Game::~Game()
{
	// Direct3D 정리
	CleanupD3D();
}

// Direct3D 초기화 함수
bool Game::InitD3D(HWND hWnd)
{
	// DXGI_SWAP_CHAIN_DESC 구조체 초기화
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1; // 스왑 체인 버퍼 개수
	sd.BufferDesc.Width = SCREEN_WIDTH;  // 렌더링 버퍼의 너비
	sd.BufferDesc.Height = SCREEN_HEIGHT; // 렌더링 버퍼의 높이
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 픽셀 포맷
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // 렌더 타겟으로 사용
	sd.OutputWindow = hWnd; // 렌더링할 윈도우 핸들
	sd.SampleDesc.Count = 1; // 멀티 샘플링 비활성화
	sd.Windowed = TRUE; // 창 모드

	HRESULT hr = D3D11CreateDeviceAndSwapChain( //  Direct3D 디바이스와 스왑 체인 생성
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
		D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, nullptr, &m_pImmediateContext
	);

	if (SUCCEEDED(hr) && m_pd3dDevice != nullptr && m_pImmediateContext != nullptr && m_pSwapChain != nullptr)
	{
		return true; // 성공적으로 초기화됨
	}
	else
	{
		// 예외 처리
		OutputDebugString(L"Direct3D 초기화에 실패했습니다.\n");
		Game::CleanupD3D(); // 초기화 실패 시 정리
		return false; // 초기화 실패
	}
}

// Direct3D 정리 함수
void Game::CleanupD3D()
{
	// Direct3D 리소스 해제
	if (m_pImmediateContext) {
		m_pImmediateContext->ClearState();
		m_pImmediateContext->Release();
		m_pImmediateContext = nullptr;
	}

	// 스왑 체인 해제
	if (m_pSwapChain) {
		m_pSwapChain->Release();
		m_pSwapChain = nullptr;
	}

	// 디바이스 해제
	if (m_pd3dDevice) {
		m_pd3dDevice->Release();
		m_pd3dDevice = nullptr;
	}
}

// Render 코드  
void Game::Render()
{
	// 백 버퍼 가져오기
	if (!Game::GetBackBuffer())
	{
		Game::CleanupD3D();
		return;
	}

	// 렌더 타겟 뷰 생성
	if (!Game::CreateRenderTargetView())
	{
		Game::CleanupD3D();
		return;
	}
	
	Game::SetRenderTarget(); // 렌더 타겟 설정
	
	Game::SetViewport(); // 뷰포트 설정

	Game::SetBackgroundColor(1, 1, 1, 1); // 배경색 설정

	// 렌더 타겟 뷰는 렌더 타겟 버퍼에 의존하여 생성되므로, 렌더 타겟 뷰를 먼저 해제해야 함
	SAFE_RELEASE(m_pRenderTargetView); // 렌더 타겟 뷰 해제
	SAFE_RELEASE(m_pBackBuffer); // 백 버퍼 해제
	 
	Game::BufferSwap(); // 버퍼 스왑
}

bool Game::GetBackBuffer()
{
	// 스왑 체인에서 렌더 타겟 버퍼(백 버퍼) 가져오기
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pBackBuffer);

	if (FAILED(hr) || m_pBackBuffer == nullptr)
	{
		// 예외 처리
		OutputDebugString(L"스왑 체인에서 백 버퍼를 가져오지 못했습니다.\n");
		SAFE_RELEASE(m_pBackBuffer);
		return false;
	}

	return true;
}

bool Game::CreateRenderTargetView()
{
	// 렌더 타겟 뷰 생성
	HRESULT hr = m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer, nullptr, &m_pRenderTargetView);

	if (FAILED(hr) || m_pRenderTargetView == nullptr)
	{
		// 예외 처리 
		OutputDebugString(L"렌더 타겟 뷰를 생성하지 못했습니다.\n");
		SAFE_RELEASE(m_pRenderTargetView);
		return false;
	}

	return true;
}

// 렌더 타겟을 설정하는 함수
void Game::SetRenderTarget()
{
	// 렌더 타겟 뷰 설정
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
}

// 뷰 포트 설정 함수
void Game::SetViewport()
{
	// 뷰 포트 설정
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<FLOAT>(SCREEN_WIDTH); // 뷰포트 너비
	viewport.Height = static_cast<FLOAT>(SCREEN_HEIGHT); // 뷰포트 높이
	viewport.MinDepth = 0.0f; // 최소 깊이
	viewport.MaxDepth = 1.0f; // 최대 깊이
	viewport.TopLeftX = 0; // 뷰포트 시작 X 좌표
	viewport.TopLeftY = 0; // 뷰포트 시작 Y 좌표

	m_pImmediateContext->RSSetViewports(1, &viewport);
}

// 배경색을 설정하는 함수
void Game::SetBackgroundColor(float r, float g, float b, float a)
{
	// 렌더 타겟을 지정한 색으로 초기화
	const float clearColor[4] = { r, g, b, a };
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
}

void Game::BufferSwap()
{
	// 스왑 체인 버퍼 스왑
	m_pSwapChain->Present(0, 0);
}
