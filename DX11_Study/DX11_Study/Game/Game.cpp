#include "PCH.h" 
#include "Game.h"

Game::Game()
{
}
Game::~Game()
{
}

// Direct3D 초기화 함수
bool Game::InitD3D(HWND hWnd)
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
		D3D11_SDK_VERSION, 
		&sd, 
		m_pSwapChain.ReleaseAndGetAddressOf(),
		m_pd3dDevice.ReleaseAndGetAddressOf(),
		nullptr, 
		m_pImmediateContext.ReleaseAndGetAddressOf()
	);

	if (FAILED(hr) || !m_pd3dDevice || !m_pImmediateContext || !m_pSwapChain)
	{
		OutputDebugString(L"Direct3D 초기화에 실패했습니다.\n");
		return false;
	}

	// 백 버퍼 가져오기
	GetBackBuffer();
	// 렌더 타겟 뷰 생성
	CreateRenderTargetView();
	// 뷰포트 설정
	SetViewport();

	CreateGeometry();
	CreateConstantBuffer();
	CreateVS(); // 정점 셰이더 
	CreateInputLayout(); // 입력 레이아웃 생성
	CreatePS(); // 픽셀 셰이더 생성

	return true;
}

// Render 코드  
void Game::Render()
{
	Game::RenderBegin();

	// IA -> VS -> RS -> PS -> OM
	
	// Input Assembler
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// 정점 버퍼 바인딩
	m_pImmediateContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	// 인덱스 버퍼 바인딩
	m_pImmediateContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	// 프리미티브 타입 설정 (삼각형 리스트)
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 입력 레이아웃 바인딩
	m_pImmediateContext->IASetInputLayout(m_inputLayout.Get());

	// Vertex Shader
	m_pImmediateContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	
	// Rasterizer

	// Pixel Shader
	m_pImmediateContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	// Output Merger

	// Draw Call
	m_pImmediateContext->DrawIndexed(static_cast<UINT>(m_indices.size()), 0, 0);

	Game::RenderEnd();
}

void Game::RenderBegin()
{
	// 렌더 타겟 뷰 설정
	m_pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), nullptr);
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
	m_pImmediateContext->RSSetViewports(1, &m_viewport);
}

void Game::RenderEnd()
{
	m_pSwapChain->Present(0, 0);
}

bool Game::GetBackBuffer()
{
	// 스왑 체인에서 렌더 타겟 버퍼(백 버퍼) 가져오기
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)m_pBackBuffer.GetAddressOf());

	if (FAILED(hr) || m_pBackBuffer == nullptr)
	{
		// 예외 처리
		OutputDebugString(L"스왑 체인에서 백 버퍼를 가져오지 못했습니다.\n");
		return false;
	}

	return true;
}

bool Game::CreateRenderTargetView()
{
	// 렌더 타겟 뷰 생성
	HRESULT hr = m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer.Get(), nullptr, m_pRenderTargetView.ReleaseAndGetAddressOf());

	if (FAILED(hr) || m_pRenderTargetView == nullptr)
	{
		// 예외 처리 
		OutputDebugString(L"렌더 타겟 뷰를 생성하지 못했습니다.\n");
		return false;
	}

	return true;
}

// 뷰 포트 설정 함수
void Game::SetViewport()
{
	// 뷰 포트 설정
	m_viewport.Width = static_cast<FLOAT>(SCREEN_WIDTH); // 뷰포트 너비
	m_viewport.Height = static_cast<FLOAT>(SCREEN_HEIGHT); // 뷰포트 높이
	m_viewport.MinDepth = 0.0f; // 최소 깊이
	m_viewport.MaxDepth = 1.0f; // 최대 깊이
	m_viewport.TopLeftX = 0; // 뷰포트 시작 X 좌표
	m_viewport.TopLeftY = 0; // 뷰포트 시작 Y 좌표
}

bool Game::CreateGeometry()
{
	m_vertices.resize(8);

	m_vertices[0].position = Vec3(0.5f, 0.5f, 0.f);
	m_vertices[0].normal = Vec3(-1.f, 1.f, -1.f);
	m_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);

	m_vertices[1].position = Vec3(0.5f, -0.5f, 0.f);
	m_vertices[1].normal = Vec3(1.f, 1.f, -1.f);
	m_vertices[1].color = Color(0.f, 0.f, 1.f, 1.f);

	m_vertices[2].position = Vec3(-0.5f, -0.5f, 0.f);
	m_vertices[2].normal = Vec3(1.f, -1.f, -1.f);
	m_vertices[2].color = Color(0.f, 1.f, 0.f, 1.f);

	m_vertices[3].position = Vec3(-0.5f, 0.5f, 0.f);
	m_vertices[3].normal = Vec3(-1.f, -1.f, -1.f);
	m_vertices[3].color = Color(0.f, 0.f, 1.f, 1.f);

	// 인덱스 배열
	m_indices.resize(36);
	m_indices = 
	{
		0,1,2, 0,2,3
	};

	// 정점 버퍼 생성
	D3D11_BUFFER_DESC bufferDesc = {};
	// GPU만 읽을 수 있도록 설정
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = (uint32) (sizeof(Vertex) * m_vertices.size());

	D3D11_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pSysMem = m_vertices.data();

	HRESULT hr = m_pd3dDevice->CreateBuffer(&bufferDesc, &subResourceData, m_vertexBuffer.ReleaseAndGetAddressOf());

	if (FAILED(hr) || m_vertexBuffer == nullptr)
	{
		OutputDebugString(L"정점 버퍼 생성에 실패했습니다.\n");
		return false;
	}

	// 인덱스 버퍼 생성
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = (uint32)(sizeof(UINT) * m_indices.size());
	subResourceData.pSysMem = m_indices.data();
	
	hr = m_pd3dDevice->CreateBuffer(&bufferDesc, &subResourceData, m_indexBuffer.ReleaseAndGetAddressOf());

	if (FAILED(hr) || m_indexBuffer == nullptr)
	{
		OutputDebugString(L"인덱스 버퍼 생성에 실패했습니다.\n");
		return false;
	}

	return true;
}

void Game::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[3] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_pd3dDevice->CreateInputLayout(
		inputLayoutDesc,
		ARRAYSIZE(inputLayoutDesc),
		m_vertexShaderBlob->GetBufferPointer(),
		m_vertexShaderBlob->GetBufferSize(),
		m_inputLayout.ReleaseAndGetAddressOf()
	);
}

bool Game::LoadShader(const std::wstring& path, const std::string& name, const std::string& version, Microsoft::WRL::ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	
	HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlags,
		0,
		blob.GetAddressOf(),
		nullptr
	);

	if (FAILED(hr) || blob == nullptr)
	{
		OutputDebugString(L"셰이더 로드에 실패했습니다.\n");
		return false;
	}
	else {
		return true;
	}
}

bool Game::CreateVS()
{
	Game::LoadShader(L"Shaders/VertexShader.hlsl", "VS_Main", "vs_5_0", m_vertexShaderBlob);

	HRESULT hr = m_pd3dDevice->CreateVertexShader(m_vertexShaderBlob->GetBufferPointer(),
		m_vertexShaderBlob->GetBufferSize(), nullptr, m_vertexShader.ReleaseAndGetAddressOf()); // 정점 셰이더 생성

	if (FAILED(hr) || m_vertexShaderBlob == nullptr)
	{
		OutputDebugString(L"정점 셰이더 생성에 실패했습니다.\n");
		return false;
	}
	else {
		return true;
	}
}

bool Game::CreatePS()
{
	Game::LoadShader(L"Shaders/PixelShader.hlsl", "PS_Main", "ps_5_0", m_pixelShaderBlob);

	HRESULT hr = m_pd3dDevice->CreatePixelShader(m_pixelShaderBlob->GetBufferPointer(),
		m_pixelShaderBlob->GetBufferSize(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()); // 픽셀 셰이더 생성

	if (FAILED(hr) || m_pixelShaderBlob == nullptr)
	{
		OutputDebugString(L"픽셀 셰이더 생성에 실패했습니다.\n");
		return false;
	}
	else {
		return true;
	}
}

bool Game::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = m_pd3dDevice->CreateBuffer(&cbDesc, nullptr, m_constantBuffer.ReleaseAndGetAddressOf());

	if (FAILED(hr)) {
		OutputDebugString(L"상수 버퍼 생성에 실패했습니다.\n");
		return false;
	}

	return true;
}