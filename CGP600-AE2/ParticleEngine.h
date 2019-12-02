#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <xnamath.h>
#include <time.h>
#include <vector>
using namespace std;

struct Particle
{
	float gravity;
	XMFLOAT3 position;
	XMFLOAT3 velocity;
	XMFLOAT4 color;

};

class ParticleEngine
{
	private:

		// DirectX 11 components for rendering objects into the scene
		ID3D11Device* m_pD3DDevice;
		ID3D11DeviceContext* m_pImmediateContext;

		// DirectX 11 Shaders, Buffers, Texture and Sampler Objects
		ID3D11VertexShader* m_pVShader;
		ID3D11PixelShader*  m_pPShader;
		ID3D11InputLayout* m_pInputLayout;
		ID3D11Buffer* m_pVertexBuffer;
		ID3D11ShaderResourceView* m_pTexture0;
		ID3D11SamplerState* m_pSampler0;

		vector<Particle*> m_free;
		vector<Particle*> m_active;


		float m_untilParticle;

	public:

		// Particle Engine class constructor and destructor

		ParticleEngine(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, int count);
		~ParticleEngine();

		int InitialiseParticle();

		// Function to draw the asset into the scene
		void Draw(XMMATRIX* view, XMMATRIX* projection, XMFLOAT3* position, float deltaTime);
		void DrawOne(Particle* one, XMMATRIX* view, XMMATRIX* projection, XMFLOAT3* position);

		float RandomBetweenFloats(float min, float max);

		float RandomZeroToOne();
		float RandomNegOneToPosOne();


};
