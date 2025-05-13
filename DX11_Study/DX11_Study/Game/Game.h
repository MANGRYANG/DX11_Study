#pragma once

class Game
{
private:
	// Direct3D ����̽�
	Microsoft::WRL::ComPtr<ID3D11Device> m_pd3dDevice = nullptr;
	// Direct3D ����̽� ���ؽ�Ʈ
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pImmediateContext = nullptr;
	// ���� ü��
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;

private:
	// �� ����
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pBackBuffer = nullptr;
	// ���� Ÿ�� ��
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
	// �� ��Ʈ
	D3D11_VIEWPORT m_viewport = {};

private:
	const float clearColor[4] = { 1, 1, 1, 1 };
	std::vector<Vertex> m_vertices;
	std::vector<UINT> m_indices;

public:
	// �����ڿ� �Ҹ���
	Game();
	~Game();

public:
	// Direct3D �ʱ�ȭ �� ���� �Լ�
	bool InitD3D(HWND hWnd);
	// ������ ���� �Լ�
	void Render();
	void RenderBegin();
	void RenderEnd();

public:
	// �� ���۸� �������� �Լ�
	bool GetBackBuffer();
	// ���� Ÿ�� �並 �����ϴ� �Լ�
	bool CreateRenderTargetView();
	// �� ��Ʈ ���� ������ �����ϴ� �Լ�
	void SetViewport();

public:
	// �ﰢ�� ���� �Լ�
	void CreateGeometry();
};