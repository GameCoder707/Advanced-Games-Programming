#pragma once

#include "objfilemodel.h"

class SkyBox
{
	// DirectX 11 components for rendering objects into the scene
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;

	// To store the model from the Asset_Details script
	ObjFileModel* m_pSkyBox;

	// DirectX 11 Shaders, Buffers, Texture and Sampler Objects
	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader*  m_pPShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pSkyConstantBuffer;
	ID3D11ShaderResourceView* m_pTexture0;
	ID3D11SamplerState* m_pSampler0;

	// This is for before drawing the skybox
	ID3D11RasterizerState*   m_pRasterSolid = 0;
	ID3D11DepthStencilState* m_pDepthWriteSolid = 0;

	// This is for after drawing the skybox
	ID3D11RasterizerState*   m_pRasterSkyBox = 0;
	ID3D11DepthStencilState* m_pDepthWriteSkyBox = 0;


public:

	// Class constructor and destructor
	SkyBox();
	~SkyBox();

	// Function to initialise the DirectX 11 renderer 
	// objects, collider and light components
	void InitialiseSkyBox(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile);

	// Function to create the model
	int LoadObjModel(char* assetFile, char* textureFile);

	// Function to release DirectX 11 objects
	void ReleaseAll();

	// Function to draw the asset into the scene
	void DrawSkyBox(XMMATRIX* view, XMMATRIX* projection, XMVECTOR position, float nearClip);

};
