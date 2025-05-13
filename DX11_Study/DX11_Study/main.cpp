#include "PCH.h"
#include "Game.h"

// 윈도우 핸들
HWND g_hWnd = nullptr;
// 게임 객체
Game g_game;

// 윈도우 프로시저 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DESTROY) {    // 창이 닫힐 때
		PostQuitMessage(0); // 메시지 루프 종료
    }
	return DefWindowProc(hWnd, message, wParam, lParam);    // 기본 윈도우 프로시저 호출
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
        WS_OVERLAPPEDWINDOW, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, hInstance, nullptr);
    
	// 화면에 창 표시 및 업데이트
    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    if (!g_game.InitD3D(g_hWnd)) { // 초기화 실패 시
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
            g_game.Render();
        }
    }

    // 윈도우 클래스 등록 해제
    UnregisterClassW(L"DX11WindowClass", hInstance);

    return 0;
}
