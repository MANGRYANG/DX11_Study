#include "PCH.h"

// ������ �ڵ�
HWND g_hWnd = nullptr;

// DirectX 11 ����̽� ��ü
ID3D11Device* g_pd3dDevice = nullptr;
// ������ �������� �����ϴ� ����̽� ���ؽ�Ʈ
ID3D11DeviceContext* g_pImmediateContext = nullptr;
// ���� ü�� ��ü
IDXGISwapChain* g_pSwapChain = nullptr;

// ������ ���ν��� �Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DESTROY) {    // â�� ���� ��
		PostQuitMessage(0); // �޽��� ���� ����
    }
	return DefWindowProc(hWnd, message, wParam, lParam);    // �⺻ ������ ���ν��� ȣ��
}

// Direct3D �ʱ�ȭ �Լ�
bool InitD3D(HWND hWnd)
{
	// DXGI_SWAP_CHAIN_DESC ����ü �ʱ�ȭ
    DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1; // ���� ü�� ���� ����
    sd.BufferDesc.Width = SCREEN_WIDTH;  // ������ ������ �ʺ�
	sd.BufferDesc.Height = SCREEN_HEIGHT; // ������ ������ ����
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �ȼ� ����
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // ���� Ÿ������ ���
	sd.OutputWindow = hWnd; // �������� ������ �ڵ�
	sd.SampleDesc.Count = 1; // ��Ƽ ���ø� ��Ȱ��ȭ
	sd.Windowed = TRUE; // â ���

	HRESULT hr = D3D11CreateDeviceAndSwapChain( //  Direct3D ����̽��� ���� ü�� ����
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
        D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, nullptr, &g_pImmediateContext
    );

	return SUCCEEDED(hr);   // ���� ���� ��ȯ
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
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, nullptr, nullptr, hInstance, nullptr);
    
	// ȭ�鿡 â ǥ�� �� ������Ʈ
    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    if (!InitD3D(g_hWnd)) { // �ʱ�ȭ ���� ��
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
			

        }
    }

	// Direct3D ���ҽ� ����
    if (g_pImmediateContext) g_pImmediateContext->Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();

    // ������ Ŭ���� ��� ����
    UnregisterClassW(L"DX11WindowClass", hInstance);

    return 0;
}
