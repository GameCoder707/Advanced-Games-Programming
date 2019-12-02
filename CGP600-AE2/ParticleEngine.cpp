#include "ParticleEngine.h"

const int VERTCOUNT = 6;

// Structure of the constant buffer used the asset
struct PARTICLE_CONSTANT_BUFFER
{
	// Matrix to store the world location, rotation, scale
	// and how it's viewed and projected from camera's POV
	XMMATRIX WorldViewProjection; // 64 bytes
	XMFLOAT4 color; // 16


}; // 80 bytes

XMFLOAT3 vertices[VERTCOUNT] =
{
	XMFLOAT3(-1.0f, -1.0f, 0.0f),
	XMFLOAT3(-1.0f, 1.0f, 0.0f),
	XMFLOAT3(1.0f, 1.0f, 0.0f),
	XMFLOAT3(-1.0f, -1.0f, 0.0f),
	XMFLOAT3(1.0f, 1.0f, 0.0f),
	XMFLOAT3(1.0f, -1.0f, 0.0f),
};


ParticleEngine::ParticleEngine(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, int count)
{
	srand(time(NULL));

	m_pD3DDevice = D3DDevice;
	m_pImmediateContext = ImmediateContext;

	for (int i = 0; i < count; i++)
	{
		m_free.push_back(new Particle());
	}

	//m_timePrevious = float(timeGetTime());
	//m_timeNow = float(timeGetTime());
	//m_deltaTime = 0.0f;
	m_untilParticle = 3.0f;

}

ParticleEngine::~ParticleEngine()
{
	// Clearing up on exit

	// Releasing all the DirectX 11 shaders, texture, sampler
	// renderers and other components
	if (m_pTexture0) m_pTexture0->Release();
	if (m_pSampler0) m_pSampler0->Release();
	if (m_pInputLayout) m_pInputLayout->Release();
	if (m_pVShader) m_pVShader->Release();
	if (m_pPShader) m_pPShader->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pD3DDevice) m_pD3DDevice->Release();

}


int ParticleEngine::InitialiseParticle()
{
	

	HRESULT hr = S_OK;

	// Create Constant Buffer
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(buffer_desc));

	buffer_desc.Usage = D3D11_USAGE_DYNAMIC; // Can use UpdateSubresource() to update
	buffer_desc.ByteWidth = sizeof(XMFLOAT3) * VERTCOUNT; // MUST be a multiple of 16, calculate from CB struct
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Use as a vertex buffer
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // edit from cpu side

	hr = m_pD3DDevice->CreateBuffer(&buffer_desc, NULL, &m_pVertexBuffer);

	if (FAILED(hr))
	{
		return S_FALSE;
	}

	// copy vertices into buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	// mapping = locking buffer, allows writing
	m_pImmediateContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, sizeof(vertices));

	// unmap = unlock the buffer
	m_pImmediateContext->Unmap(m_pVertexBuffer, NULL);

	ID3DBlob *VS, *PS, *error;
	hr = D3DX11CompileFromFile("particle_shader.hlsl", 0, 0, "PVS", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0) // Check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))
		{
			return hr;
		};
	}

	hr = D3DX11CompileFromFile("particle_shader.hlsl", 0, 0, "PPS", "ps_4_0", 0, 0, 0, &PS, &error, 0);

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
	};

	hr = m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);

	if (FAILED(hr))
	{
		return hr;
	}

	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	//D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice, textureFile, NULL, NULL, &m_pTexture0, NULL);

	/*D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;*/

	//m_pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler0);


}

void ParticleEngine::Draw(XMMATRIX* view, XMMATRIX* projection, XMFLOAT3* position, float deltaTime)
{
	
	//
	//m_untilParticle -= deltaTime;

	//if (m_untilParticle <= 0)
	//{

	//	if (m_free.size() != 0)
	//	{

	//		m_active.push_back(new Particle());
	//		m_free.pop_back();
	//		
	//		m_active[m_active.size() - 1]->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f);
	//		m_active[m_active.size() - 1]->gravity = 0.1f;
	//		m_active[m_active.size() - 1]->position = XMFLOAT3(0.0f, 2.0f, 0.0);
	//		m_active[m_active.size() - 1]->velocity = XMFLOAT3(0.f, 0.f, 0.f);


	//	}

	//	m_untilParticle = 3.0f;
	//}


	//for(int i = 0; i < m_active.size(); i++)
	//{
	//	//(*it)->velocity.y -= (*it)->gravity;
	//	//(*it)->position.y += (*it)->velocity.y;

	//	m_active[i]->velocity.y -= m_active[i]->gravity;
	//	m_active[i]->position.y += m_active[i]->velocity.y;


	//	DrawOne(m_active[i], view, projection, position);

	//	if (m_active[i]->position.y < 0)
	//	{
	//		m_free.push_back(new Particle());
	//		m_active.erase(m_active.begin() + i);

	//	}

	//}

	Particle test;
	test.color = XMFLOAT4(1.0f, 0.0f, 0.3f, 1.f);
	test.gravity = 1;
	test.position = XMFLOAT3(0.0f, 5.0f, 1.0f);
	test.velocity = XMFLOAT3(0.f, 0.f, 0.f);

	DrawOne(&test, view, projection, position);

}

void ParticleEngine::DrawOne(Particle* one, XMMATRIX* view, XMMATRIX* projection, XMFLOAT3* position)
{
	D3D11_MAPPED_SUBRESOURCE ms;
	m_pImmediateContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// Lock the buffer to allow writing
	memcpy(ms.pData, vertices, sizeof(vertices));						// Copy the data -  only upload those that are used
	m_pImmediateContext->Unmap(m_pVertexBuffer, NULL);

	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	XMMATRIX world = XMMatrixIdentity();

	world *= XMMatrixScaling(1.f, 1.f, 1.f);
	world *= XMMatrixRotationRollPitchYaw(0.0f, 0.0, 0.0f);
	world *= XMMatrixTranslation(one->position.x, one->position.y, one->position.z);

	PARTICLE_CONSTANT_BUFFER particle_CB;

	particle_CB.WorldViewProjection = world * (*view) * (*projection);
	particle_CB.color = one->color;

	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, 0, &particle_CB, 0, 0);

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pVertexBuffer);

	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture0);

	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);

	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	// Select which primitive type to use // 03-01
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw the vertex buffer to the back buffer //  03-01
	m_pImmediateContext->Draw(VERTCOUNT, 0);

}

float ParticleEngine::RandomBetweenFloats(float min, float max)
{
	float random = float(rand()) / float(RAND_MAX);
	float diff = max - min;
	float randValue = random * diff;

	return min + randValue;
}

float ParticleEngine::RandomZeroToOne()
{
	return RandomBetweenFloats(0, 1);
}

float ParticleEngine::RandomNegOneToPosOne()
{
	return RandomBetweenFloats(-1, 1);
}

