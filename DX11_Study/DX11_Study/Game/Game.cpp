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

	return SUCCEEDED(hr);   // 성공 여부 반환
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

void Game::Render()
{
	// Render 코드
	
}
