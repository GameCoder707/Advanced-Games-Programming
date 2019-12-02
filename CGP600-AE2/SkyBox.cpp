#include "SkyBox.h"

struct SKY_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection; // 64 bytes
								
};


SkyBox::SkyBox()
{

}

void SkyBox::ReleaseAll()
{
	// Clearing up on exit

	// Deleting the asset object
	delete m_pSkyBox;

	// Releasing all the DirectX 11 shaders, texture, sampler
	// renderers and other components
	if (m_pTexture0) m_pTexture0->Release();
	if (m_pSampler0) m_pSampler0->Release();
	if (m_pInputLayout) m_pInputLayout->Release();
	if (m_pVShader) m_pVShader->Release();
	if (m_pPShader) m_pPShader->Release();
	if (m_pRasterSolid) m_pRasterSolid->Release();
	if (m_pRasterSkyBox) m_pRasterSkyBox->Release();
	if (m_pDepthWriteSolid) m_pDepthWriteSolid->Release();
	if (m_pDepthWriteSkyBox) m_pDepthWriteSkyBox->Release();
	if (m_pSkyConstantBuffer) m_pSkyConstantBuffer->Release();

}


void SkyBox::InitialiseSkyBox(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile)
{
	// DirectX 11 renderer components are initialised
	m_pD3DDevice = D3DDevice;
	m_pImmediateContext = ImmediateContext;

	// The creation of the asset function is called
	LoadObjModel(assetFile, textureFile);

}

SkyBox::~SkyBox()
{
	

}

int SkyBox::LoadObjModel(char* assetFile, char* textureFile)
{
	// The asset object is being initialised
	m_pSkyBox = new ObjFileModel(assetFile, m_pD3DDevice, m_pImmediateContext);

	if (m_pSkyBox->filename == "FILE NOT LOADED") return S_FALSE;

	HRESULT hr = S_OK;

	// Create Constant Buffer

	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; // Can use UpdateSubresource() to update
	constant_buffer_desc.ByteWidth = sizeof(SKY_CONSTANT_BUFFER); // MUST be a multiple of 16, calculate from CB struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Use as a constant buffer

	hr = m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pSkyConstantBuffer);

	ID3DBlob *VS, *PS, *error;

	hr = D3DX11CompileFromFile("skyshader.hlsl", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0) // Check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))
		{
			return hr;
		};
	}

	hr = D3DX11CompileFromFile("skyshader.hlsl", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))
		{
			return hr;
		};
	}

	// Creating vertex and pixel shaders

	hr = m_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPShader);

	if (FAILED(hr))
	{
		return hr;
	}

	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);

	// Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	hr = m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);

	if (FAILED(hr))
	{
		return hr;
	}

	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice, textureFile, NULL, NULL, &m_pTexture0, NULL);

	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	m_pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler0);

	// Creating a rasterizer using a rasterizer
	// description
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FrontCounterClockwise = FALSE;
	rasterDesc.DepthBias = 0;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = TRUE;
	rasterDesc.ScissorEnable = FALSE;
	rasterDesc.MultisampleEnable = FALSE;
	rasterDesc.AntialiasedLineEnable = FALSE;

	// Creating 2 Rasterising states
	hr = m_pD3DDevice->CreateRasterizerState(&rasterDesc, &m_pRasterSolid);

	rasterDesc.CullMode = D3D11_CULL_FRONT;
	hr = m_pD3DDevice->CreateRasterizerState(&rasterDesc, &m_pRasterSkyBox);

	// Creating a depth stencil state using
	// a description
	D3D11_DEPTH_STENCIL_DESC depthDesc;
	depthDesc.DepthEnable = TRUE;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDesc.StencilEnable = FALSE;

	hr = m_pD3DDevice->CreateDepthStencilState(&depthDesc, &m_pDepthWriteSolid);

	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = m_pD3DDevice->CreateDepthStencilState(&depthDesc, &m_pDepthWriteSkyBox);

	

}


void SkyBox::DrawSkyBox(XMMATRIX* view, XMMATRIX* projection, XMVECTOR position, float nearClip)
{

	XMMATRIX world = XMMatrixIdentity();

	// Scales with the camera's near clipping plane
	world *= XMMatrixScaling(nearClip + 1.0f, nearClip + 1.0f, nearClip + 1.0f);

	// Moves along the camera
	world *= XMMatrixTranslation(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position));

	// Creating a constant buffer reference object
	SKY_CONSTANT_BUFFER sky_cb_values;

	// By multiplying these matrices, we get to place the
	// skybox in the scene correctly
	sky_cb_values.WorldViewProjection = (world) * (*view) * (*projection);


	// DirectX 11 rendering functions
	m_pImmediateContext->UpdateSubresource(m_pSkyConstantBuffer, 0, 0, &sky_cb_values, 0, 0);

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pSkyConstantBuffer);

	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture0);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler0);

	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);

	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pImmediateContext->RSSetState(m_pRasterSkyBox);
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthWriteSkyBox, 1);
	

	// Draw the object into the scene
	m_pSkyBox->Draw();

	m_pImmediateContext->RSSetState(m_pRasterSolid);
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthWriteSolid, 1);

}