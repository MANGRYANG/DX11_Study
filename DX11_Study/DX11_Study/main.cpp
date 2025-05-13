#include "PCH.h"
#include "Game.h"

// ������ �ڵ�
HWND g_hWnd = nullptr;
// ���� ��ü
Game g_game;

// ������ ���ν��� �Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DESTROY) {    // â�� ���� ��
		PostQuitMessage(0); // �޽��� ���� ����
    }
	return DefWindowProc(hWnd, message, wParam, lParam);    // �⺻ ������ ���ν��� ȣ��
}

// ���� �Լ�  
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    // ������ Ŭ���� ����
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
        hInstance, nullptr, nullptr, nullptr, nullptr, L"DX11WindowClass", nullptr };

	// ������ Ŭ���� ���
    RegisterClassExW(&wcex);

    // â ����
    g_hWnd = CreateWindowW(L"DX11WindowClass", L"DirectX 11 Test", 
        WS_OVERLAPPEDWINDOW, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, hInstance, nullptr);
    
	// ȭ�鿡 â ǥ�� �� ������Ʈ
    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    if (!g_game.InitD3D(g_hWnd)) { // �ʱ�ȭ ���� ��
        // ���� �޽��� �ڽ� ���
		MessageBoxW(nullptr, L"Failed to initialize Direct3D 11", L"Error", MB_OK);
        return -1;
    }

    MSG msg = {};
	while (msg.message != WM_QUIT) {    // �޽����� WM_QUIT�� �ƴ� ������ �ݺ�
		if (PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE)) { // �޽����� ���� ��
			// �޽��� ó��
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        else {
			// �޽����� ���� �� ������ �۾� ����
            g_game.Render();
        }
    }

    // ������ Ŭ���� ��� ����
    UnregisterClassW(L"DX11WindowClass", hInstance);

    return 0;
}
