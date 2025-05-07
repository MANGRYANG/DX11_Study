#pragma once

class Game
{
private:
	// Direct3D ����̽�
	ID3D11Device* m_pd3dDevice;
	// Direct3D ����̽� ���ؽ�Ʈ
	ID3D11DeviceContext* m_pImmediateContext;
	// ���� ü��
	IDXGISwapChain* m_pSwapChain;

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
};