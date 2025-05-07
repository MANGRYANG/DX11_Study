#include "PCH.h"

// 윈도우 핸들
HWND g_hWnd = nullptr;

// DirectX 11 디바이스 객체
ID3D11Device* g_pd3dDevice = nullptr;
// 실제로 렌더링을 수행하는 디바이스 컨텍스트
ID3D11DeviceContext* g_pImmediateContext = nullptr;
// 스왑 체인 객체
IDXGISwapChain* g_pSwapChain = nullptr;

// 윈도우 프로시저 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DESTROY) {    // 창이 닫힐 때
		PostQuitMessage(0); // 메시지 루프 종료
    }
	return DefWindowProc(hWnd, message, wParam, lParam);    // 기본 윈도우 프로시저 호출
}

// Direct3D 초기화 함수
bool InitD3D(HWND hWnd)
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
        D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, nullptr, &g_pImmediateContext
    );

	return SUCCEEDED(hr);   // 성공 여부 반환
}

// 메인 함수
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    // 윈도우 클래스 정의
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
        hInstance, nullptr, nullptr, nullptr, nullptr, L"DX11WindowClass", nullptr };

	// 윈도우 클래스 등록
    RegisterClassExW(&wcex);

    // 창 생성
    g_hWnd = CreateWindowW(L"DX11WindowClass", L"DirectX 11 Test", 
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, nullptr, nullptr, hInstance, nullptr);
    
	// 화면에 창 표시 및 업데이트
    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    if (!InitD3D(g_hWnd)) { // 초기화 실패 시
        // 에러 메시지 박스 출력
		MessageBoxW(nullptr, L"Failed to initialize Direct3D 11", L"Error", MB_OK);
        return -1;
    }

    MSG msg = {};
	while (msg.message != WM_QUIT) {    // 메시지가 WM_QUIT이 아닐 때까지 반복
		if (PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE)) { // 메시지가 있을 때
			// 메시지 처리
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        else {
			// 메시지가 없을 때 렌더링 작업 수행
			

        }
    }

	// Direct3D 리소스 해제
    if (g_pImmediateContext) g_pImmediateContext->Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();

    // 윈도우 클래스 등록 해제
    UnregisterClassW(L"DX11WindowClass", hInstance);

    return 0;
}
