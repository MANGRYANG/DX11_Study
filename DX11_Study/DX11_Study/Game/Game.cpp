#include "PCH.h" 
#include "Game.h"

Game::Game()
{
}
Game::~Game()
{
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
		D3D11_SDK_VERSION, 
		&sd, 
		m_pSwapChain.GetAddressOf(), 
		m_pd3dDevice.GetAddressOf(), 
		nullptr, 
		m_pImmediateContext.GetAddressOf()
	);

	if (FAILED(hr) || !m_pd3dDevice || !m_pImmediateContext || !m_pSwapChain)
	{
		OutputDebugString(L"Direct3D 초기화에 실패했습니다.\n");
		return false;
	}

	// 백 버퍼 가져오기
	GetBackBuffer();
	// 렌더 타겟 뷰 생성
	CreateRenderTargetView();
	// 뷰포트 설정
	SetViewport();

	return true;
}

// Render 코드  
void Game::Render()
{
	Game::RenderBegin();

	// VS -> RS -> PS -> OM

	Game::RenderEnd();
}

void Game::RenderBegin()
{
	// 렌더 타겟 뷰 설정
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
	// 스왑 체인에서 렌더 타겟 버퍼(백 버퍼) 가져오기
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)m_pBackBuffer.GetAddressOf());

	if (FAILED(hr) || m_pBackBuffer == nullptr)
	{
		// 예외 처리
		OutputDebugString(L"스왑 체인에서 백 버퍼를 가져오지 못했습니다.\n");
		return false;
	}

	return true;
}

bool Game::CreateRenderTargetView()
{
	// 렌더 타겟 뷰 생성
	HRESULT hr = m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf());

	if (FAILED(hr) || m_pRenderTargetView == nullptr)
	{
		// 예외 처리 
		OutputDebugString(L"렌더 타겟 뷰를 생성하지 못했습니다.\n");
		return false;
	}

	return true;
}

// 뷰 포트 설정 함수
void Game::SetViewport()
{
	// 뷰 포트 설정
	m_viewport.Width = static_cast<FLOAT>(SCREEN_WIDTH); // 뷰포트 너비
	m_viewport.Height = static_cast<FLOAT>(SCREEN_HEIGHT); // 뷰포트 높이
	m_viewport.MinDepth = 0.0f; // 최소 깊이
	m_viewport.MaxDepth = 1.0f; // 최대 깊이
	m_viewport.TopLeftX = 0; // 뷰포트 시작 X 좌표
	m_viewport.TopLeftY = 0; // 뷰포트 시작 Y 좌표
}

void Game::CreateGeometry()
{
	m_vertices.resize(3); // 삼각형을 구성할 세 개의 정점

	// 첫 번째 정점: 위치와 색상 설정
	m_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f); // 화면 중앙 좌측 하단
	m_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f); // 빨간색

	// 두 번째 정점: 위치와 색상 설정
	m_vertices[1].position = Vec3(0.f, 0.5f, 0.f); // 화면 중앙 상단
	m_vertices[1].color = Color(0.f, 1.f, 0.f, 1.f); // 녹색

	// 세 번째 정점: 위치와 색상 설정
	m_vertices[2].position = Vec3(0.5f, -0.5f, 0.f); // 화면 중앙 우측 하단
	m_vertices[2].color = Color(0.f, 0.f, 1.f, 1.f); // 파란색

	// 세 번째 정점: 위치와 색상 설정
	m_vertices[2].position = Vec3(0.5f, -0.5f, 0.f); // 화면 중앙 우측 하단
	m_vertices[2].color = Color(0.f, 0.f, 1.f, 1.f); // 파란색
}
