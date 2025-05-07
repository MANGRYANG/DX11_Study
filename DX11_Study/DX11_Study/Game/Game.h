#pragma once

class Game
{
private:
	// Direct3D 디바이스
	ID3D11Device* m_pd3dDevice;
	// Direct3D 디바이스 컨텍스트
	ID3D11DeviceContext* m_pImmediateContext;
	// 스왑 체인
	IDXGISwapChain* m_pSwapChain;

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
};