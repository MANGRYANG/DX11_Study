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

private:
	// 정점 구조체 배열
	std::vector<Vertex> m_vertices;
	// 인덱스 배열
	std::vector<UINT> m_indices;
	// 정점 버퍼
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer = nullptr;
	// 인덱스 버퍼
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer = nullptr;
	// 입력 레이아웃
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;
	// 셰이더
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> m_vertexShaderBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> m_pixelShaderBlob = nullptr;

	// 상수 버퍼
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

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
	bool CreateGeometry();
	// 입력 레이아웃 생성 함수
	void CreateInputLayout();
	// 셰이더 로드 함수
	bool LoadShader(const std::wstring& path, const std::string& name, const std::string& version, Microsoft::WRL::ComPtr<ID3DBlob>& blob);

	// 셰이더 생성 함수
	bool CreateVS();
	bool CreatePS();

	// 상수 버퍼 생성 함수
	bool CreateConstantBuffer();
};