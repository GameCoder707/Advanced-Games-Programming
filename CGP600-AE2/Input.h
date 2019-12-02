#pragma once

#include <dinput.h>

class Input
{
private:
	// Declaring the direct input handle
	IDirectInput8* m_direct_input;

	// Declaring the keyboard device
	IDirectInputDevice8* m_keyboard_device;

	// Declaring the mouse device
	IDirectInputDevice8* m_mouse_device;

	
	/*long prev_mouse_x = 0;
	long prev_mouse_y = 0;

	long mouse_x = 0;
	long mouse_y = 0;*/

	// A boolean to check if the key
	// is pressed only once
	bool ButtonPushedOnce;

	// This is to retrieve the state
	// of each key in keyboard
	unsigned char m_keyboard_keys_state[256];

public:

	// This is to retrieve the state
	// of each key and movement of
	// the mouse
	DIMOUSESTATE m_mouse_state;

	// This is to store each key
	// from the keyboard to be
	// used in the game
	unsigned char mve_frwd;
	unsigned char mve_lft;
	unsigned char mve_bck;
	unsigned char mve_rght;
	unsigned char jump;
	unsigned char sprint;
	unsigned char pick_up;
	unsigned char drop;
	unsigned char interact;
	unsigned char swapCamera;
	unsigned char closeGame;

	//Class constructor and destructor
	Input(HINSTANCE hInst, HWND hWnd);
	~Input();

	// This initialises the input devices
	HRESULT InitialiseInput(HINSTANCE hInst, HWND hWnd);

	// This is to read input states
	// from both keyboard and mouse
	void ReadInputStates();

	// This is to check if the key
	// is continuously pressed
	bool IsKeyPressed(unsigned char DI_Keycode);

	// This is to check if the key
	// is pressed once
	bool IsKeyPressedOnce(unsigned char DI_Keycode);

	// This is to check if the key
	// is released
	bool IsKeyReleased(unsigned char DI_Keycode);

};