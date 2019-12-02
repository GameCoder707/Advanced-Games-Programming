#pragma once

// Including the Colliding box and sphere class header and light class header files
// It is because the assets use both for collisions and lighting calculations
#include "Collider3D.h"
#include "Light.h"

class Asset
{
private:

	// DirectX 11 components for rendering objects into the scene
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;

	// To store the model from the Asset_Details script
	ObjFileModel* m_pObject;

	// Light component used by the asset
	Light* m_light;
	
	// DirectX 11 Shaders, Buffers, Texture and Sampler Objects
	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader*  m_pPShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11ShaderResourceView* m_pTexture0;
	ID3D11SamplerState* m_pSampler0;

	// Scaling values of the assets
	float m_scale_x, m_scale_y, m_scale_z;

public:

	// The collision box and sphere component
	// possessed by the asset
	Collider3D* collider;

	// Asset class constructor and destructor
	Asset();
	~Asset();

	// Function to initialise the DirectX 11 renderer 
	// objects, collider and light components
	void InitialiseAsset(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale);

	// Function to create the model
	int LoadObjModel(char* assetFile, char* textureFile);
	
	// Get Scaling values of the asset
	float GetXScale();
	float GetYScale();
	float GetZScale();

	// Function to draw the asset into the scene
	void Draw(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection);

	void ReleaseAll();


};
