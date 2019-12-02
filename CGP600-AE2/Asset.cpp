#include "Asset.h"

// Structure of the constant buffer used the asset
struct ASSET_CONSTANT_BUFFER
{
	// Matrix to store the world location, rotation, scale
	// and how it's viewed and projected from camera's POV
	XMMATRIX WorldViewProjection; // 64 bytes

	// Light vectors
	XMVECTOR directional_light_vector; // 16 bytes
	XMVECTOR directional_light_colour; // 16 bytes
	XMVECTOR ambient_light_colour; // 16 bytes


}; // 112 bytes


Asset::Asset()
{
	
}

Asset::~Asset()
{

}

void Asset::InitialiseAsset(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale)
{
	// DirectX 11 renderer components are initialised
	m_pD3DDevice = D3DDevice;
	m_pImmediateContext = ImmediateContext;

	// The scaling values are being initialised from the parameters
	m_scale_x = x_scale;
	m_scale_y = y_scale;
	m_scale_z = z_scale;

	// The collider component is initialised
	collider = new Collider3D();
	
	// The light component is initialised
	m_light = new Light();

	// The light vectors of the light component
	// are being initialised
	m_light->SetDirectionalLightPos(-1.0f, 1.0f, 1.0f, 0.0f);
	m_light->SetDirectionalLightColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetAmbientLightColour(0.3f, 0.3f, 0.3f, 1.0f);

	// The creation of the asset function is called
	LoadObjModel(assetFile, textureFile);

}


int Asset::LoadObjModel(char* assetFile, char* textureFile)
{
	// The asset object is being initialised
	m_pObject = new ObjFileModel(assetFile, m_pD3DDevice, m_pImmediateContext);

	if (m_pObject->filename == "FILE NOT LOADED") return S_FALSE;

	HRESULT hr = S_OK;

	// Create Constant Buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; // Can use UpdateSubresource() to update
	constant_buffer_desc.ByteWidth = sizeof(ASSET_CONSTANT_BUFFER); // MUST be a multiple of 16, calculate from CB struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Use as a constant buffer

	hr = m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);

	ID3DBlob *VS, *PS, *error;
	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS, &error, 0);


	if (error != 0) // Check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))
		{
			return hr;
		};
	}

	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS, &error, 0);

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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

	// Calculating collider box and sphere values
	collider->CalculateColliderCentre(m_pObject);
	collider->CalculateDimensions(m_pObject);
	collider->CalculateRadius(m_pObject);

}

void Asset::Draw(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection)
{
	// Matrix to transpose the light vector
	// towards the asset position
	XMMATRIX transpose;

	// Creating a constant buffer reference object
	ASSET_CONSTANT_BUFFER asset_cb_values;

	// By multiplying these matrices, we get to place the
	// asset in the scene correctly
	asset_cb_values.WorldViewProjection = (*world) * (*view) * (*projection);

	// Transposing the world matrix
	transpose = XMMatrixTranspose(*world);

	// Setting the light vector values in
	// the constant buffer from the light component
	asset_cb_values.directional_light_colour = m_light->GetDirectionalLightColour();
	asset_cb_values.ambient_light_colour = m_light->GetAmbientLightColour();
	asset_cb_values.directional_light_vector = XMVector3Transform(m_light->GetDirectionalLightPos(), transpose);
	asset_cb_values.directional_light_vector = XMVector3Normalize(asset_cb_values.directional_light_vector);

	// DirectX 11 rendering functions
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &asset_cb_values, 0, 0);

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture0);

	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);

	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw the object into the scene
	m_pObject->Draw();

}

void Asset::ReleaseAll()
{
	// Clearing up on exit

	// Deleting the asset object
	delete m_pObject;

	// Releasing all the DirectX 11 shaders, texture, sampler
	// renderers and other components
	if (m_pTexture0) m_pTexture0->Release();
	if (m_pSampler0) m_pSampler0->Release();
	if (m_pInputLayout) m_pInputLayout->Release();
	if (m_pVShader) m_pVShader->Release();
	if (m_pPShader) m_pPShader->Release();
	if (m_pConstantBuffer) m_pConstantBuffer->Release();

}


float Asset::GetXScale()
{
	return m_scale_x;
}

float Asset::GetYScale()
{
	return m_scale_y;
}

float Asset::GetZScale()
{
	return m_scale_z;
}

