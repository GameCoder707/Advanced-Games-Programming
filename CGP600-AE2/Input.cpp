#include "Input.h"

Input::Input(HINSTANCE hInst, HWND hWnd)
{
	// We now initialise the input devices
	InitialiseInput(hInst, hWnd);

	// Initialising the game controls
	mve_frwd = DIK_W;
	mve_lft = DIK_A;
	mve_bck = DIK_S;
	mve_rght = DIK_D;
	jump = DIK_SPACE;
	sprint = DIK_LSHIFT;
	pick_up = DIK_E;
	drop = DIK_G;
	interact = DIK_F;
	swapCamera = DIK_Q;
	closeGame = DIK_ESCAPE;

	// The boolean is initialised
	ButtonPushedOnce = false;

}

Input::~Input()
{
	// Unaquiring and releasing the
	// input devices
	m_keyboard_device->Unacquire();
	m_keyboard_device->Release();

	m_mouse_device->Unacquire();
	m_mouse_device->Release();

	m_direct_input->Release();
}

HRESULT Input::InitialiseInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// INITIALISING KEYBOARD DEVICE

	ZeroMemory(m_keyboard_keys_state, sizeof(m_keyboard_keys_state));

	// Creating the input handle 
	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_direct_input, NULL);

	if (FAILED(hr)) return hr;

	// Creating the keyboard device
	hr = m_direct_input->CreateDevice(GUID_SysKeyboard, &m_keyboard_device, NULL);

	if (FAILED(hr)) return hr;

	hr = m_keyboard_device->SetDataFormat(&c_dfDIKeyboard);

	if (FAILED(hr)) return hr;

	// Setting the keyboard to be exclusive
	// to the window or not
	hr = m_keyboard_device->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if (FAILED(hr)) return hr;

	// We now acquire the keyboard device
	hr = m_keyboard_device->Acquire();

	if (FAILED(hr)) return hr;

	// INITIALISING MOUSE DEVICE

	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_direct_input, NULL);

	if (FAILED(hr)) return hr;

	hr = m_direct_input->CreateDevice(GUID_SysMouse, &m_mouse_device, NULL);

	if (FAILED(hr)) return hr;

	hr = m_mouse_device->SetDataFormat(&c_dfDIMouse);

	if (FAILED(hr)) return hr;

	hr = m_mouse_device->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	if (FAILED(hr)) return hr;

	hr = m_mouse_device->Acquire();

	if (FAILED(hr)) return hr;

	return S_OK;

}

void Input::ReadInputStates()
{
	HRESULT hr;

	// KEYBOARD READ STATE

	// Retrieving the keyboard input state
	hr = m_keyboard_device->GetDeviceState(sizeof(m_keyboard_keys_state), (LPVOID)&m_keyboard_keys_state);

	if (FAILED(hr))
	{
		// This is to re-acquire the input device
		// if the connection is lost or it hasn't been
		// aquired before
		if (hr == DIERR_INPUTLOST || (hr == DIERR_NOTACQUIRED))
		{
			m_keyboard_device->Acquire();
		}
	}

	// MOUSE READ STATE

	hr = m_mouse_device->GetDeviceState(sizeof(m_mouse_state), (LPVOID)&m_mouse_state);

	if (FAILED(hr))
	{
		if (hr == DIERR_INPUTLOST || (hr == DIERR_NOTACQUIRED))
		{
			m_mouse_device->Acquire();
		}
	}
}

bool Input::IsKeyPressed(unsigned char DI_Keycode)
{
	// This returns true if the key
	// passed through the parameter is pressed
	return m_keyboard_keys_state[DI_Keycode] & 0x80;
}

bool Input::IsKeyPressedOnce(unsigned char DI_Keycode)
{
	// Using the boolean, we make the function to
	// be executed only once
	if (ButtonPushedOnce == false && m_keyboard_keys_state[DI_Keycode] & 0x80)
	{
		ButtonPushedOnce = true;
		return m_keyboard_keys_state[DI_Keycode] & 0x80;
	}
	else
	{
		return !m_keyboard_keys_state[DI_Keycode] & 0x80;
	}

}

bool Input::IsKeyReleased(unsigned char DI_Keycode)
{
	// We check the reverse state of the key to see
	// if the key is released or not
	if (!(m_keyboard_keys_state[DI_Keycode] & 0x80))
	{
		ButtonPushedOnce = false;
		return !(m_keyboard_keys_state[DI_Keycode] & 0x80);
	}
	else
	{
		return m_keyboard_keys_state[DI_Keycode] & 0x80;
	}

}