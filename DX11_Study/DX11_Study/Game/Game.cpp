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

// Render 코드  
void Game::Render()
{
	// 스왑 체인에서 렌더 타겟 버퍼(백 버퍼) 가져오기  
	ID3D11Texture2D* pBackBuffer = nullptr;
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (FAILED(hr) || pBackBuffer == nullptr)
	{
		// 예외 처리
		OutputDebugString(L"스왑 체인에서 백 버퍼를 가져오지 못했습니다.\n");
		return;
	}

	// 렌더 타겟 뷰 생성  
	ID3D11RenderTargetView* pRenderTargetView = nullptr;
	hr = m_pd3dDevice->
		CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);

	if (FAILED(hr) || pRenderTargetView == nullptr)
	{
		// 예외 처리 
		OutputDebugString(L"렌더 타겟 뷰를 생성하지 못했습니다.\n");
		pBackBuffer->Release();
		return;
	}

	// 렌더 타겟 뷰를 출력 대상으로 설정
	m_pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

	// 뷰 포트 설정
	D3D11_VIEWPORT viewport = {};
	viewport.Width = SCREEN_WIDTH; // 뷰포트 너비
	viewport.Height = SCREEN_HEIGHT; // 뷰포트 높이
	viewport.MinDepth = 0.0f; // 최소 깊이
	viewport.MaxDepth = 1.0f; // 최대 깊이
	viewport.TopLeftX = 0; // 뷰포트 시작 X 좌표
	viewport.TopLeftY = 0; // 뷰포트 시작 Y 좌표

    m_pImmediateContext->RSSetViewports(1, &viewport);

	// 렌더 타겟을 지정한 색으로 초기화
	const float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 흰색 배경  
	m_pImmediateContext->ClearRenderTargetView(pRenderTargetView, clearColor);

    // 렌더 타겟 뷰 해제  
    pRenderTargetView->Release();  
    pBackBuffer->Release();  
	 
	// 버퍼 스왑으로 화면에 출력 
	m_pSwapChain->Present(1, 0);
} 
