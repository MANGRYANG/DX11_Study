#pragma once

class Game
{
private:
	// Direct3D 디바이스
	Microsoft::WRL::ComPtr<ID3D11Device> m_pd3dDevice = nullptr;
	// Direct3D 디바이스 컨텍스트
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pImmediateContext = nullptr;
	// 스왑 체인
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;

private:
	// 백 버퍼
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pBackBuffer = nullptr;
	// 렌더 타겟 뷰
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
	// 뷰 포트
	D3D11_VIEWPORT m_viewport = {};

private:
	const float clearColor[4] = { 1, 1, 1, 1 };
	std::vector<Vertex> m_vertices;
	std::vector<UINT> m_indices;

public:
	// 생성자와 소멸자
	Game();
	~Game();

public:
	// Direct3D 초기화 및 정리 함수
	bool InitD3D(HWND hWnd);
	// 렌더링 관련 함수
	void Render();
	void RenderBegin();
	void RenderEnd();

public:
	// 백 버퍼를 가져오는 함수
	bool GetBackBuffer();
	// 렌더 타겟 뷰를 생성하는 함수
	bool CreateRenderTargetView();
	// 뷰 포트 관련 값들을 설정하는 함수
	void SetViewport();

public:
	// 삼각형 생성 함수
	void CreateGeometry();
};