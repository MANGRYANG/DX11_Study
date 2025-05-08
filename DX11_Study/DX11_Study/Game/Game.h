#pragma once
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = nullptr; } }

class Game
{
private:
	// Direct3D ����̽�
	ID3D11Device* m_pd3dDevice;
	// Direct3D ����̽� ���ؽ�Ʈ
	ID3D11DeviceContext* m_pImmediateContext;
	// ���� ü��
	IDXGISwapChain* m_pSwapChain;

private:
	// �� ����
	ID3D11Texture2D* m_pBackBuffer;
	// ���� Ÿ�� ��
	ID3D11RenderTargetView* m_pRenderTargetView;
	
public:
	// �����ڿ� �Ҹ���
	Game();
	~Game();

public:
	// Direct3D �ʱ�ȭ �� ���� �Լ�
	bool InitD3D(HWND hWnd);
	// Direct3D ���� �Լ�
	void CleanupD3D();
	// ������ �Լ�
	void Render();

public:
	// �� ���۸� �������� �Լ�
	bool GetBackBuffer();
	// ���� Ÿ�� �並 �����ϴ� �Լ�
	bool CreateRenderTargetView();
	// ���� Ÿ���� �����ϴ� �Լ�
	void SetRenderTarget();
	// �� ��Ʈ ���� �Լ�
	void SetViewport();
	// ������ �����ϴ� �Լ�
	void SetBackgroundColor(float r, float g, float b, float a);
	// ���� ���� �Լ�
	void BufferSwap();
};