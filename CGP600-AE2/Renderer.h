#pragma once

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>

class Renderer
{
private:

	HINSTANCE m_hInst = NULL;

	WNDCLASSEX wcex = { 0 };

	D3D_DRIVER_TYPE         m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           m_pD3DDevice = NULL;
	ID3D11DeviceContext*    m_pImmediateContext = NULL;
	IDXGISwapChain*         m_pSwapChain = NULL;
	ID3D11RenderTargetView* m_pBackBufferRTView = NULL;
	ID3D11DepthStencilView* m_pZBuffer;
	ID3D11BlendState* m_pAlphaBlendEnable; // 14
	ID3D11BlendState* m_pAlphaBlendDisable; // 14

	// Window's width and height
	float m_win_width;
	float m_win_height;

	// Window Name
	char g_CGP600_Assignment_2[100] = "CGP600 Assignment 2\0";


public:

	// Constructor and Destrucor
	Renderer(HINSTANCE hInstance, int nCmdShow);
	~Renderer();

	// Window Process function
	// It is called when interacted
	// with the window
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	// Function to initialise the window
	HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);

	// Function to initialise DirectX 11 
	HRESULT InitialiseD3D();

	// This is to clear the screen and
	// depth stencil view
	void ClearBuffers();

	// This is to present the back buffer
	// to the screen
	void Display();

	// This is to set the alpha blend state
	void SetBlendState(bool state);

	// These return the window's height
	// and width
	float GetWindowWidth();
	float GetWindowHeight();

	// Returns the window for other classes
	// to use
	HWND GetWindow();

	void ReleaseAll();
	
	// These return the rendering objects
	// for other classes to use
	ID3D11Device* GetD3DDevice();
	ID3D11DeviceContext* GetDeviceContext();
};
