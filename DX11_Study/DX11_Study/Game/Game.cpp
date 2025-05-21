#include "PCH.h" 
#include "Game.h"

Game::Game()
{
}
Game::~Game()
{
}

// Direct3D �ʱ�ȭ �Լ�
bool Game::InitD3D(HWND hWnd)
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
		D3D11_SDK_VERSION, 
		&sd, 
		m_pSwapChain.ReleaseAndGetAddressOf(),
		m_pd3dDevice.ReleaseAndGetAddressOf(),
		nullptr, 
		m_pImmediateContext.ReleaseAndGetAddressOf()
	);

	if (FAILED(hr) || !m_pd3dDevice || !m_pImmediateContext || !m_pSwapChain)
	{
		OutputDebugString(L"Direct3D �ʱ�ȭ�� �����߽��ϴ�.\n");
		return false;
	}

	// �� ���� ��������
	GetBackBuffer();
	// ���� Ÿ�� �� ����
	CreateRenderTargetView();
	// ����Ʈ ����
	SetViewport();

	CreateGeometry();
	CreateConstantBuffer();
	CreateVS(); // ���� ���̴� 
	CreateInputLayout(); // �Է� ���̾ƿ� ����
	CreatePS(); // �ȼ� ���̴� ����

	return true;
}

// Render �ڵ�  
void Game::Render()
{
	Game::RenderBegin();

	// IA -> VS -> RS -> PS -> OM
	
	// Input Assembler
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// ���� ���� ���ε�
	m_pImmediateContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	// �ε��� ���� ���ε�
	m_pImmediateContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	// ������Ƽ�� Ÿ�� ���� (�ﰢ�� ����Ʈ)
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// �Է� ���̾ƿ� ���ε�
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
	// ���� Ÿ�� �� ����
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
	// ���� ü�ο��� ���� Ÿ�� ����(�� ����) ��������
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)m_pBackBuffer.GetAddressOf());

	if (FAILED(hr) || m_pBackBuffer == nullptr)
	{
		// ���� ó��
		OutputDebugString(L"���� ü�ο��� �� ���۸� �������� ���߽��ϴ�.\n");
		return false;
	}

	return true;
}

bool Game::CreateRenderTargetView()
{
	// ���� Ÿ�� �� ����
	HRESULT hr = m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer.Get(), nullptr, m_pRenderTargetView.ReleaseAndGetAddressOf());

	if (FAILED(hr) || m_pRenderTargetView == nullptr)
	{
		// ���� ó�� 
		OutputDebugString(L"���� Ÿ�� �並 �������� ���߽��ϴ�.\n");
		return false;
	}

	return true;
}

// �� ��Ʈ ���� �Լ�
void Game::SetViewport()
{
	// �� ��Ʈ ����
	m_viewport.Width = static_cast<FLOAT>(SCREEN_WIDTH); // ����Ʈ �ʺ�
	m_viewport.Height = static_cast<FLOAT>(SCREEN_HEIGHT); // ����Ʈ ����
	m_viewport.MinDepth = 0.0f; // �ּ� ����
	m_viewport.MaxDepth = 1.0f; // �ִ� ����
	m_viewport.TopLeftX = 0; // ����Ʈ ���� X ��ǥ
	m_viewport.TopLeftY = 0; // ����Ʈ ���� Y ��ǥ
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

	// �ε��� �迭
	m_indices.resize(36);
	m_indices = 
	{
		0,1,2, 0,2,3
	};

	// ���� ���� ����
	D3D11_BUFFER_DESC bufferDesc = {};
	// GPU�� ���� �� �ֵ��� ����
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = (uint32) (sizeof(Vertex) * m_vertices.size());

	D3D11_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pSysMem = m_vertices.data();

	HRESULT hr = m_pd3dDevice->CreateBuffer(&bufferDesc, &subResourceData, m_vertexBuffer.ReleaseAndGetAddressOf());

	if (FAILED(hr) || m_vertexBuffer == nullptr)
	{
		OutputDebugString(L"���� ���� ������ �����߽��ϴ�.\n");
		return false;
	}

	// �ε��� ���� ����
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = (uint32)(sizeof(UINT) * m_indices.size());
	subResourceData.pSysMem = m_indices.data();
	
	hr = m_pd3dDevice->CreateBuffer(&bufferDesc, &subResourceData, m_indexBuffer.ReleaseAndGetAddressOf());

	if (FAILED(hr) || m_indexBuffer == nullptr)
	{
		OutputDebugString(L"�ε��� ���� ������ �����߽��ϴ�.\n");
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
		OutputDebugString(L"���̴� �ε忡 �����߽��ϴ�.\n");
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
		m_vertexShaderBlob->GetBufferSize(), nullptr, m_vertexShader.ReleaseAndGetAddressOf()); // ���� ���̴� ����

	if (FAILED(hr) || m_vertexShaderBlob == nullptr)
	{
		OutputDebugString(L"���� ���̴� ������ �����߽��ϴ�.\n");
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
		m_pixelShaderBlob->GetBufferSize(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()); // �ȼ� ���̴� ����

	if (FAILED(hr) || m_pixelShaderBlob == nullptr)
	{
		OutputDebugString(L"�ȼ� ���̴� ������ �����߽��ϴ�.\n");
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
		OutputDebugString(L"��� ���� ������ �����߽��ϴ�.\n");
		return false;
	}

	return true;
}