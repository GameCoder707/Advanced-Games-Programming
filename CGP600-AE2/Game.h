#pragma once

// This is to ignore warnings when
// reading a script
#define _CRT_SECURE_NO_WARNINGS

// Including the standard library
#include <stdlib.h>

// Including all the header files in the game
#include "Renderer.h"
#include "Camera.h"
#include "Input.h"
#include "SceneNode.h"
#include "Time.h"
#include "SkyBox.h"
#include "UI.h"
#include "ParticleEngine.h"


class Game
{

private:
	
	// This is the renderer object through
	// which all objects are going to be
	// rendered
	Renderer* m_render_target;

	// This is the input object used by
	// user
	Input* key;

	// This is a time object to keep
	// in track of FPS and get deltaTime
	Time* timing;

	// A single UI object to display
	// texts on screen
	UI* hud;

	//SkyBox Cube
	SkyBox* skybox;

	// Rain particle
	ParticleEngine* rain;

	// This is a vector of camera objects
	vector<Camera*> view;

	// A FILE object to read Asset_Details.txt
	FILE* assetFile;
	FILE* new_wep_file;

	// The prime node to parent all other
	// nodes in the game
	SceneNode* m_root_node;

	// A single node to represent the player
	SceneNode* m_player_node;

	//*******Vectors of asset nodes*******//

	// Player's Weapon Nodes
	vector <SceneNode*> m_spear_nodes;

	// Enemy Nodees
	vector <SceneNode*> m_enemy_nodes;

	// Enemies' Weapon Nodes
	vector <SceneNode*> m_eweapon_nodes;

	// Static Nodes
	vector <SceneNode*> m_statik_nodes;

	// Block Nodes
	vector <SceneNode*> m_block_nodes;

	// Food Nodes
	vector <SceneNode*> m_food_nodes;


	// A vector to contain all nodes except
	// the root node and the player
	vector<SceneNode*> objs;

	int game_timer;

	// A boolean to check if all the assets 
	// in the game are initialised
	bool initialised;

	// A boolean to check if the cinematic
	// camera is enabled or not
	bool cine_camera;

	// A boolean to close the game loop
	bool game_running;

	// A boolean to begin the game
	bool game_begin;


public:
	// Constructor
	Game(HINSTANCE hInstance, int nCmdShow);

	// The main update of the game
	void MainUpdate();

	// This is to initialise all the
	// game objects in the game
	void InitialiseGameAssets();

	// This is to erase all the objects
	// and re-initialise them
	void RestartGame();

	void LoadManual();

	// Texts can be added to be displayed
	// during game
	void PopulateHUD();

	// Generates a weapon in the game
	void InstantiateWeapon(float x, float y, float z);

	// This initiates combat actions of the asset
	void InitiateCombat(SceneNode* attacker, bool start_condition);

	// This damages the node
	bool IsNodeDamaged(SceneNode* attacker, SceneNode* victim);

	// This is to release every child of
	// root node
	void ReleaseEverything();

	// Returns the boolean
	bool isGameRunning();

};
