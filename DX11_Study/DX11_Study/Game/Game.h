#pragma once
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = nullptr; } }

class Game
{
private:
	// Direct3D 디바이스
	ID3D11Device* m_pd3dDevice;
	// Direct3D 디바이스 컨텍스트
	ID3D11DeviceContext* m_pImmediateContext;
	// 스왑 체인
	IDXGISwapChain* m_pSwapChain;

private:
	// 백 버퍼
	ID3D11Texture2D* m_pBackBuffer;
	// 렌더 타겟 뷰
	ID3D11RenderTargetView* m_pRenderTargetView;
	
public:
	// 생성자와 소멸자
	Game();
	~Game();

public:
	// Direct3D 초기화 및 정리 함수
	bool InitD3D(HWND hWnd);
	// Direct3D 정리 함수
	void CleanupD3D();
	// 렌더링 함수
	void Render();

public:
	// 백 버퍼를 가져오는 함수
	bool GetBackBuffer();
	// 렌더 타겟 뷰를 생성하는 함수
	bool CreateRenderTargetView();
	// 렌더 타겟을 설정하는 함수
	void SetRenderTarget();
	// 뷰 포트 설정 함수
	void SetViewport();
	// 배경색을 설정하는 함수
	void SetBackgroundColor(float r, float g, float b, float a);
	// 버퍼 스왑 함수
	void BufferSwap();
};