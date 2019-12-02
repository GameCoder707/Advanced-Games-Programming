#include "Game.h"

Game::Game(HINSTANCE hInstance, int nCmdShow)
{
	// Initialising the renderer object
	m_render_target = new Renderer(hInstance, nCmdShow);

	// Initialising the input object
	key = new Input(hInstance, m_render_target->GetWindow());

	// Initialising the Time object
	timing = new Time();

	// Initialising the sky box
	skybox = new SkyBox();

	skybox->InitialiseSkyBox(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), "Assets/Objects/cubeObj.obj", "Assets/Textures/night_sky.dds");
	
	hud = new UI("Assets/Textures/red_font.png", m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext());

	//rain = new ParticleEngine(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), 100);
	//rain->InitialiseParticle();

	// Boolean variables are initialised
	initialised = false;
	cine_camera = false;
	game_running = true;
	game_begin = false;

	game_timer = 0;

	// Calling the function to initialise assets
	InitialiseGameAssets();
	

}

void Game::ReleaseEverything()
{

	hud->ReleaseAll();
	skybox->ReleaseAll();
	m_root_node->ReleaseAll();
	m_render_target->ReleaseAll();
	
}

void Game::LoadManual()
{
	if (key->IsKeyPressed(key->jump))
	{
		game_begin = true;
	}

	hud->AddText("Move: W/A/S/D",   -0.3, 0.95, 0.04);
	hud->AddText("Look: Mouse",     -0.3, 0.85, 0.04);
	hud->AddText("Sprint: LShift",  -0.3, 0.75, 0.04);
	hud->AddText("Jump: Space",     -0.3, 0.65, 0.04);
	hud->AddText("Attack: LMB",     -0.3, 0.55, 0.04);
	hud->AddText("Equip Weapon: E", -0.3, 0.45, 0.04);
	hud->AddText("Carry Block: F",  -0.3, 0.35, 0.04);
	hud->AddText("Drop Weapon: G",  -0.3, 0.25, 0.04);
	hud->AddText("Swap Camera: Q",  -0.3, 0.15, 0.04);
	hud->AddText("Close Game: Esc", -0.3, 0.05, 0.04);

	hud->AddText("Press Space to Continue", -0.5, -0.4, 0.04);

	hud->RenderText();

}

void Game::MainUpdate()
{
	// This is called to clear the screen before
	// drawing again
	m_render_target->ClearBuffers();

	// Reads if any key is pressed
	key->ReadInputStates();

	if (key->IsKeyPressed(key->closeGame))
	{
		game_running = false;
	}

	if (game_begin == false) LoadManual();


	if (game_running == true && game_begin == true)
	{

		// This will increase the timer each time
		// we hit the frame limit
		if (timing->GetTime() > 1.0 && cine_camera == false)
		{
			game_timer += 1;
		}

		// We execute the time functions to receive
		// delta time
		timing->Execute();		

		// To check if the keys are released
		key->IsKeyReleased(key->swapCamera);

		// If the cinematic camera is not enabled,
		// play the game
		if (cine_camera == false)
		{		

			// This restricts the pitch of the player's rotation
			m_player_node->RestrictPitch();

			// This rotates the player based on mouse input
			m_player_node->RotateAsset(key->m_mouse_state.lY, key->m_mouse_state.lX, 0.0f);


			// Keyboard Controls

			// If the forward key is pressed, this returns true
			if (key->IsKeyPressed(key->mve_frwd))
			{
				// We move the player forward in the
				// direction the player is facing. This
				// is determined by the yaw angle and movement
				// speed is retrieved from the player class

				m_player_node->MoveAsset(sin(m_player_node->GetYAngle()) * m_player_node->m_p_asset->GetPlayerMoveSpeed() * timing->GetDeltaTime(), 0.0f,
					cos(m_player_node->GetYAngle()) * m_player_node->m_p_asset->GetPlayerMoveSpeed() * timing->GetDeltaTime());
			}
			if (key->IsKeyPressed(key->mve_lft))
			{
				// We strafe the player left in the
				// direction the player is facing.

				m_player_node->MoveAsset(sin(m_player_node->GetYAngle() + XMConvertToRadians(90)) * -m_player_node->m_p_asset->GetPlayerMoveSpeed() * timing->GetDeltaTime(), 0.0f,
					cos(m_player_node->GetYAngle() + XMConvertToRadians(90)) * -m_player_node->m_p_asset->GetPlayerMoveSpeed() * timing->GetDeltaTime());

			}
			if (key->IsKeyPressed(key->mve_bck))
			{
				// We move the player back in the
				// direction the player is facing.

				m_player_node->MoveAsset(sin(m_player_node->GetYAngle()) * -m_player_node->m_p_asset->GetPlayerMoveSpeed() * timing->GetDeltaTime(), 0.0f,
					cos(m_player_node->GetYAngle()) * -m_player_node->m_p_asset->GetPlayerMoveSpeed() * timing->GetDeltaTime());

			}
			if (key->IsKeyPressed(key->mve_rght))
			{
				// We strafe the player right in the
				// direction the player is facing.

				m_player_node->MoveAsset(sin(m_player_node->GetYAngle() + XMConvertToRadians(90)) * m_player_node->m_p_asset->GetPlayerMoveSpeed() * timing->GetDeltaTime(), 0.0f,
					cos(m_player_node->GetYAngle() + XMConvertToRadians(90)) * m_player_node->m_p_asset->GetPlayerMoveSpeed() * timing->GetDeltaTime());

			}
			// If the player pressed the jump key and
			// if the player is on ground, this returns true
			if (key->IsKeyPressed(key->jump) && m_player_node->GetOnGroundState() == true)
			{
				// Set the player on ground boolean to false
				m_player_node->SetOnGroundState(false);

				// Set the jump state to true
				m_player_node->m_p_asset->SetJumpState(true);

				// Set the jump height 3 units higher 
				// from our current position
				m_player_node->m_p_asset->SetJumpHeight(m_player_node->GetYPos() + 3.0f);

			}

			// If the swap camera key is pressed,
			// enable the cinematic camera
			if (key->IsKeyPressedOnce(key->swapCamera))
			{
				cine_camera = true;
			}

			// If the sprint key is pressed, this
			// returns true
			if (key->IsKeyPressed(key->sprint))
			{
				// We double the speed multiplier 
				m_player_node->m_p_asset->SetSpeedMultiplier(2.0f);
			}
			else if (m_player_node->m_p_asset->GetSpeedMultiplier() != 1.0f)
			{
				// We reset the speed multiplier if the sprint
				// key is not pressed
				m_player_node->m_p_asset->SetSpeedMultiplier(1.0f);
			}


			// If the player is jumping, this returns true
			if (m_player_node->m_p_asset->GetJumpState() == true)
			{
				// We gradually increase the player's Y position
				m_player_node->MoveAsset(0.0f, m_player_node->m_p_asset->GetJumpSpeed() * timing->GetDeltaTime(), 0.0f);

				// If we had reached the jump height, stop jumping 
				if (m_player_node->GetYPos() > m_player_node->m_p_asset->GetJumpHeight())
				{
					m_player_node->m_p_asset->SetJumpState(false);
				}
			}

			// Calling the function to update the player's
			// collision tree
			m_player_node->UpdateCollisionTree(&XMMatrixIdentity());

			// Checking collision of every object
			// in the world against the player
			for (int i = 0; i < objs.size(); i++)
			{
				m_player_node->CheckCollision(objs[i]);

				if (m_player_node->IsColliding())
				{
					break;
				}

			}

			// If the player has children and is carrying a weapon, this returns true
			if (m_player_node->GetChildrenSize() != 0 && 
				m_player_node->m_p_asset->GetWeaponCarryingState() == true && 
				m_player_node->m_p_asset->GetPushState() == false)
			{
				// If the drop key is pressed, this returns true
				if (key->IsKeyPressed(key->drop))
				{
					// We push the weapon back to the vector
					m_spear_nodes.push_back(m_player_node->GetEquippedWeaponNode());

					// It is now a child of the root node
					m_root_node->AddChildNode(m_player_node->GetEquippedWeaponNode());

					// Place the weapon a few units from 
					// the player in the world
					m_player_node->GetEquippedWeaponNode()->SetXPos(m_player_node->GetXPos() + sin(m_player_node->GetYAngle()) * 3.0);
					m_player_node->GetEquippedWeaponNode()->SetYPos(m_player_node->GetYPos() - 0.5);
					m_player_node->GetEquippedWeaponNode()->SetZPos(m_player_node->GetZPos() + cos(m_player_node->GetYAngle()) * 3.0);

					// Set the player weapon carrying 
					// state to false
					m_player_node->m_p_asset->SetWeaponCarryingState(false);

					// Push the weapon back to the all objects vector
					objs.push_back(m_player_node->GetEquippedWeaponNode());

					// Finally, detach the weapon 
					// node from the player node
					m_player_node->DetachNode(m_player_node->GetEquippedWeaponNode());

				}

				// A double check because this shouldn't be executed
				// if the player dropped the weapon
				if (m_player_node->GetChildrenSize() != 0)
				{
					// If the left mouse button is pressed, the player is
					// carrying a weapon and if the player isn't attacking,
					// this returns true
					InitiateCombat(m_player_node, key->m_mouse_state.rgbButtons[0]);
					
					for (int i = 1; i < m_statik_nodes.size(); i++)
					{
						if (IsNodeDamaged(m_player_node, m_statik_nodes[i]))
						{
							m_statik_nodes[i]->m_s_asset->SetTreeHealth(m_statik_nodes[i]->m_s_asset->GetTreeHealth() - 
							m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponDamage());
						}

						if (m_statik_nodes[i]->m_s_asset->GetTreeHealth() <= 0)
						{
							// We now loop through every object in the game
							for (int j = 0; j < objs.size(); j++)
							{
								// If the enemy object matches the
								// object in the all objects vector,
								// this returns true
								if (objs[j] == m_statik_nodes[i])
								{
									// We erase the object from the vector
									objs.erase(objs.begin() + j);
								}
							}

							// Adding a new weapon
							InstantiateWeapon(m_statik_nodes[i]->GetXPos(), m_statik_nodes[i]->GetYPos(), m_statik_nodes[i]->GetZPos());

							// Detaching the enemy node from the
							// root node
							m_root_node->DetachNode(m_statik_nodes[i]);

							m_statik_nodes[i]->ReleaseAll();

							// Finally, we erase the enemy itself
							m_statik_nodes.erase(m_statik_nodes.begin() + i);

							
						}

					}

					// We now loop through every enemy
					// in the game
					for (int i = 0; i < m_enemy_nodes.size(); i++)
					{
						
						if (IsNodeDamaged(m_player_node, m_enemy_nodes[i]))
						{
							// This will reduce the enemy's health by 1
							m_enemy_nodes[i]->m_e_asset->SetEnemyHealth(m_enemy_nodes[i]->m_e_asset->GetEnemyHealth() - 
							m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponDamage());
						}

							// If the enemy's health is or below 0,
							// this returns true
							if (m_enemy_nodes[i]->m_e_asset->GetEnemyHealth() <= 0)
							{
								// We now loop through every object in the game
								for (int j = 0; j < objs.size(); j++)
								{
									// If the enemy object matches the
									// object in the all objects vector,
									// this returns true
									if (objs[j] == m_enemy_nodes[i])
									{
										// We erase the object from the vector
										objs.erase(objs.begin() + j);
									}
								}

								// Detaching the enemy node from the
								// root node
								m_root_node->DetachNode(m_enemy_nodes[i]);

								m_enemy_nodes[i]->ReleaseAll();

								// Erasing the enemy's weapon node
								m_eweapon_nodes.erase(m_eweapon_nodes.begin() + i);

								// Finally, we erase the enemy itself
								m_enemy_nodes.erase(m_enemy_nodes.begin() + i);
							}
						

					}
				}

			}

			// Looping through every enemy
			for (int i = 0; i < m_enemy_nodes.size(); i++)
			{

				m_enemy_nodes[i]->CheckInRange(m_player_node->GetWorldPos());

				// Looping through every object in the game
				for (int j = 0; j < objs.size(); j++)
				{
					// Checking collision of every enemy 
					// against every other object

					// If the enemy is colliding with anything,
					// this will return true
					if(m_enemy_nodes[i]->CheckCollision(objs[j]))
					{
						// Revert all enemy states
						m_enemy_nodes[i]->SetHaltState(false);
						m_enemy_nodes[i]->SetInRangeState(false);

						// Make the enemy move to its previous spot
						m_enemy_nodes[i]->SetToPreviousSpot();

						// Break the loop to stop further collision
						// checks
						break;

					}
				}

				// Make the enemy look at its movement
				// spots
				m_enemy_nodes[i]->LookAt();

				if (timing->GetDeltaTime() < 1)
				{
					// Make the enemy move in the direction it's facing
					m_enemy_nodes[i]->MoveAsset(sin(m_enemy_nodes[i]->GetYAngle()) * m_enemy_nodes[i]->m_e_asset->GetMoveSpeed() * timing->GetDeltaTime(), 0.0f,
						cos(m_enemy_nodes[i]->GetYAngle()) * m_enemy_nodes[i]->m_e_asset->GetMoveSpeed() * timing->GetDeltaTime());
				}

				// If the enemy has reached closer to the player, then commence attack
				// Same attack logic as the player
				InitiateCombat(m_enemy_nodes[i], m_enemy_nodes[i]->isHalted());

				// If the player is getting damaged, this returns true
				if (IsNodeDamaged(m_enemy_nodes[i], m_player_node))
				{
					// This reduces the player's health by 1
					m_player_node->m_p_asset->SetPlayerHealth(m_player_node->m_p_asset->GetPlayerHealth() - 
					m_enemy_nodes[i]->GetEquippedWeaponNode()->m_w_asset->GetWeaponDamage());
				}

					// If the player has lost all its
					// health, this will return true
					if (m_player_node->m_p_asset->GetPlayerHealth() <= 0)
					{
						// Restart the game
						RestartGame();
					}

			}

			// Loop through every player 
			// weapon in the game
			for (int i = 0; i < m_spear_nodes.size(); i++)
			{
				// Check collision of every weapon
				// against the player
				m_spear_nodes[i]->CheckActionCollision(m_player_node);

				// If the weapon is colliding and if the player
				// has pressed the pick up key button, this
				// returns true
				if (m_spear_nodes[i]->IsInteracting() == true &&
					m_player_node->m_p_asset->GetWeaponCarryingState() == false &&
					key->IsKeyPressed(key->pick_up))
				{
					// Setting the position of the weapon
					// from player's position
					m_spear_nodes[i]->SetXPos(1.0f);
					m_spear_nodes[i]->SetYPos(-0.5f);
					m_spear_nodes[i]->SetZPos(1.0f);

					// Detach the node from the root node
					m_root_node->DetachNode(m_spear_nodes[i]);

					// Set the weapon carrying state to true
					m_player_node->m_p_asset->SetWeaponCarryingState(true);

					// Set the weapon as a child of the player
					m_player_node->AddChildNode(m_spear_nodes[i]);

					// Loop through every object in the scene
					for (int j = 0; j < objs.size(); j++)
					{
						// If the weapon matches the object
						// in the all objects vector, this
						// will return true
						if (m_spear_nodes[i] == objs[j])
						{
							// Erase the object from the
							// all objects vector
							objs.erase(objs.begin() + j);
							break;
						}
					}

					// Finally, erase the weapon from the weapon
					// nodes vector
					m_spear_nodes.erase(m_spear_nodes.begin() + i);

				}

			}

			// Loop through each dynamic object in the game
			for (int i = 0; i < m_block_nodes.size(); i++)
			{
				// If the player isn't pushing, check for collision
				if (m_player_node->m_p_asset->GetPushState() == false)
				{
					m_block_nodes[i]->CheckCollision(m_player_node);
				}

				// Check interact collision of the dynamic object
				// against the player
				m_block_nodes[i]->CheckActionCollision(m_player_node);

				// If the dynamic object is colliding with
				// the player, not being pushed and if the
				// interact key is pressed, this will return true
				if (m_block_nodes[i]->IsInteracting() && key->IsKeyPressed(key->interact) && m_player_node->m_p_asset->GetPushState() == false)
				{
					// Setting the position of the object
					// respective to the player
					m_block_nodes[i]->SetXPos(0.0f);
					m_block_nodes[i]->SetYPos(-0.5f);
					m_block_nodes[i]->SetZPos(2.0f);

					// Detach the dynamic object 
					// node from the root node
					m_root_node->DetachNode(m_block_nodes[i]);

					// Set the push state to true
					m_player_node->m_p_asset->SetPushState(true);

					// Set the dynamic object as a child of
					// the player
					m_player_node->AddChildNode(m_block_nodes[i]);

					// Same process for removing the object
					// from the all objects vector
					for (int j = 0; j < objs.size(); j++)
					{
						if (m_block_nodes[i] == objs[j])
						{
							objs.erase(objs.begin() + j);
							break;
						}
					}

					// Finally, erase the object from 
					// the dynamic object node vector
					m_block_nodes.erase(m_block_nodes.begin() + i);

				}


			}

			for (int i = 0; i < m_food_nodes.size(); i++)
			{
				m_food_nodes[i]->RotateAsset(0.01f, 0.1f, 0.0f);

				if (m_food_nodes[i]->CheckActionCollision(m_player_node) && 
					m_player_node->m_p_asset->GetPlayerHealth() != m_player_node->m_p_asset->GetMaxHealth())
				{
					m_player_node->m_p_asset->SetPlayerHealth(m_player_node->m_p_asset->GetPlayerHealth() + m_food_nodes[i]->m_f_asset->GetHealthValue());

					if (m_player_node->m_p_asset->GetPlayerHealth() > m_player_node->m_p_asset->GetMaxHealth())
					{
						m_player_node->m_p_asset->ResetPlayerHealth();
					}

					for (int j = 0; j < objs.size(); j++)
					{
						if (m_food_nodes[i] == objs[j])
						{
							objs.erase(objs.begin() + j);
							break;
						}
					}

					m_root_node->DetachNode(m_food_nodes[i]);

					m_food_nodes[i]->ReleaseAll();

					m_food_nodes.erase(m_food_nodes.begin() + i);

				}
			}

			// If the player has released the interact 
			// key and is pushing an object, this will
			// return true
			if (m_player_node->GetChildrenSize() != 0 && !key->IsKeyPressed(key->interact) && m_player_node->m_p_asset->GetPushState() == true)
			{
				// Similar process of dropping a weapon
				// Refer Line 157
				m_block_nodes.push_back(m_player_node->GetPushingCrate());
				m_root_node->AddChildNode(m_player_node->GetPushingCrate());

				m_player_node->GetPushingCrate()->SetXPos(m_player_node->GetXPos() + sin(m_player_node->GetYAngle()) * 2.5);
				m_player_node->GetPushingCrate()->SetYPos(m_player_node->GetYPos() - 0.5);
				m_player_node->GetPushingCrate()->SetZPos(m_player_node->GetZPos() + cos(m_player_node->GetYAngle()) * 2.5);

				m_player_node->GetPushingCrate()->SetYAngle(m_player_node->GetYAngle());

				m_player_node->m_p_asset->SetPushState(false);

				objs.push_back(m_player_node->GetPushingCrate());

				m_player_node->DetachNode(m_player_node->GetPushingCrate());

			}

			

			// Stopping the player at collision
			m_player_node->RestrictPos(m_player_node->IsColliding());
			

			// Storing previous positions of the player
			m_player_node->UpdatePos(m_player_node->IsColliding());

			


			//Check if the player is colliding with anything below		
			for (int i = 0; i < objs.size(); i++)
			{
				// Checking collision of the player's bottom
				// against every other object
				
				
				//if (m_player_node->GetOnGroundState() == true)
				if(m_player_node->CheckNodeBottomCollision(objs[i]))
				{
					break;
				}
			

			}

			// We apply the same logic as above
			// to every object against
			// every other object
			for (int i = 0; i < objs.size(); i++)
			{
				for (int j = 0; j < objs.size(); j++)
				{
					//if (objs[i]->GetOnGroundState() == true)
					if(objs[i]->CheckNodeBottomCollision(objs[j]))
					{
						break;
					}
					
				}
			}

			// If the player is in the air
			// and not jumping, pull the player
			// down
			if (m_player_node->m_p_asset->GetJumpState() == false)
			{
				m_player_node->ApplyGravity(timing->GetDeltaTime());
			}

			// Apply gravity to objects that can
			// fall down
			for (int i = 0; i < objs.size(); i++)
			{
				objs[i]->ApplyGravity(timing->GetDeltaTime());
				objs[i]->UpdateCollisionTree(&XMMatrixIdentity());
			}

			// Calling the function to move the 
			// camera along the player
			view[0]->MoveWithPlayer(m_player_node->GetXPos(), m_player_node->GetYPos(), m_player_node->GetZPos());


			// Mouse Controls

			// Rotate the camera based on mouse input
			view[0]->PitchCamera(-key->m_mouse_state.lY);
			view[0]->YawCamera(key->m_mouse_state.lX);

			

		}
		else if (cine_camera == true)
		{
			// Revert back to the First-Person camera
			// if the key is pressed again
			if (key->IsKeyPressedOnce(key->swapCamera))
			{
				cine_camera = false;
			}

			// Rotate the cinematic camera
			// based on mouse input
			view[1]->PitchCamera(-key->m_mouse_state.lY);
			view[1]->YawCamera(key->m_mouse_state.lX);

			// Move the camera freely based on
			// input movement keys
			if (key->IsKeyPressed(key->mve_frwd))
			{
				view[1]->Move(0.0f, 0.0f, 15.0 * timing->GetDeltaTime());
			}
			if (key->IsKeyPressed(key->mve_lft))
			{
				view[1]->Move(-15.0 * timing->GetDeltaTime(), 0.0f, 0.0f);
			}
			if (key->IsKeyPressed(key->mve_bck))
			{
				view[1]->Move(0.0f, 0.0f, -15.0 * timing->GetDeltaTime());

			}
			if (key->IsKeyPressed(key->mve_rght))
			{
				view[1]->Move(15.0 * timing->GetDeltaTime(), 0.0f, 0.0f);
			}

		}

		
		// DRAW

		// If the cinematic camera is disabled,
		// use the first-person camera
		if (cine_camera == false)
		{
			skybox->DrawSkyBox(&view[0]->GetViewMatrix(), &view[0]->GetProjectionMatrix(), view[0]->GetPosition(), view[0]->GetNearClipPlane());
			m_root_node->Draw(&XMMatrixIdentity(), &view[0]->GetViewMatrix(), &view[0]->GetProjectionMatrix());
			//rain->Draw(&view[0]->GetViewMatrix(), &view[0]->GetProjectionMatrix(), 0, timing->GetDeltaTime());

		}
		// Else use the cinematic camera
		else
		{
			skybox->DrawSkyBox(&view[1]->GetViewMatrix(), &view[1]->GetProjectionMatrix(), view[1]->GetPosition(), view[1]->GetNearClipPlane());
			m_root_node->Draw(&XMMatrixIdentity(), &view[1]->GetViewMatrix(), &view[1]->GetProjectionMatrix());
		}


		PopulateHUD();
		

		hud->RenderText();

		// Finally, display everything that is
		// drawn and rendered
			
		
	}

	m_render_target->Display();

}

void Game::InitiateCombat(SceneNode* attacker, bool start_condition)
{
	// A starting condition has to be true and
	// if the weapon is in idle state
	if (start_condition &&
		attacker->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackedState() == false &&
		attacker->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackCompleteState() == false)
	{
		// Set the attack state to true
		attacker->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackedState(true);
		attacker->GetEquippedWeaponNode()->m_w_asset->SetWeaponHitState(false);

		// Set the current Z position of the weapon
		attacker->GetEquippedWeaponNode()->SetCurZPos();

	}

	// If the weapon is in attack state, 
	// this returns true
	if (attacker->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackedState() == true)
	{
		// If the weapon has reached enough distance, this
		// will return true
		if (attacker->GetEquippedWeaponNode()->GetZPos() >=
			attacker->GetEquippedWeaponNode()->GetCurZPos() + 1.5)
		{
			// The weapon isn't attacking anymore and 
			// is now in completed state
			attacker->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackedState(false);
			attacker->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackCompleteState(true);
		}
		// Else move the weapon forward
		else
		{
			attacker->GetEquippedWeaponNode()->MoveAsset(0.0f, 0.0f, 15.0 * timing->GetDeltaTime());
		}

	}

	// If the weapon has completed its attack,
	// this will return true
	else if (attacker->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackCompleteState() == true)
	{

		// If the weapon has reached its original
		// position, this will return true
		if (attacker->GetEquippedWeaponNode()->GetZPos()
			<= attacker->GetEquippedWeaponNode()->GetCurZPos())
		{
			// Now the weapon has attacked and completed
			attacker->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackCompleteState(false);
		}
		// Else move the weapon back
		else
		{
			attacker->GetEquippedWeaponNode()->MoveAsset(0.0f, 0.0f, -15.0f * timing->GetDeltaTime());
		}

	}
}


bool Game::IsNodeDamaged(SceneNode* attacker, SceneNode* victim)
{
	// Check the collision of the victim 
	// against attacker's weapon
	victim->CheckActionCollision(attacker->GetEquippedWeaponNode());

	// If the attacker weapon is colliding with the
	// enemy, this returns true
	if (victim->IsInteracting() && attacker->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackedState() == true &&
		attacker->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackCompleteState() == false &&
		attacker->GetEquippedWeaponNode()->m_w_asset->GetWeaponHitState() == false)
	{

		attacker->GetEquippedWeaponNode()->m_w_asset->SetWeaponHitState(true);
		return true;

	}
	else
	{
		return false;
	}

}

void Game::PopulateHUD()
{
	hud->AddText("HEALTH:" + to_string(int(m_player_node->m_p_asset->GetPlayerHealth())) + "/" + to_string(int(m_player_node->m_p_asset->GetMaxHealth())),
		-0.98, -0.7, 0.04);

	hud->AddText("TIME:" + to_string(game_timer), 0.0, 0.94, 0.04);

	hud->AddText("FPS:" + to_string(timing->GetFPS()), -0.98, 0.94, 0.04);

	if (cine_camera == true)
	{
		hud->AddText("PAUSED", -0.3, 0.7, 0.1f);
	}

}

void Game::InitialiseGameAssets()
{
	// Open the script file to initialise assets
	fopen_s(&assetFile, "Assets/Scripts/Asset_Details.txt", "r");

	// Initialising the root node
	m_root_node = new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), '\0', NULL, NULL, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0);

	// Declaring and initialising variables
	// to reference and use from the script
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float cam_rot = 0.0f;
	float fov = 0.0f;
	float n_clip = 0.0f;
	float f_clip = 0.0f;

	float x_scle = 0.0f;
	float y_scle = 0.0f;
	float z_scle = 0.0f;

	// A char object to read location
	// of the asset object file
	char assetObj[256];

	// A char object to read location
	// of the asset texture file
	char textureFile[256];

	// To store the num of asset objects
	int num_assets = 0;

	// To store the gravity state 
	// of the object
	int gravityState = 0;

	// This is to store the type of
	// asset read from the script
	char asset_type[256];

	// This is to store the type of
	// node read from the script
	char node_type;

	// Run the loop until everything
	// is initialised
	while (initialised == false)
	{
		// This scans the script to
		// store the type of asset
		fscanf(assetFile, "%s", asset_type);

		// We now compare this string with
		// what was scanned to check if 
		// they're same
		if (strstr("Camera", asset_type) != 0)
		{
			// Now we scan the number of assets
			fscanf(assetFile, "%d", &num_assets);

			// We now loop through each asset
			for (int i = 0; i < num_assets; i++)
			{
				// We scan the complete line from the file 
				// to store of all this asset's values
				fscanf(assetFile, "%f %f %f %f %f %f %f", &x, &y, &z, &cam_rot, &fov, &n_clip, &f_clip);

				// We now initialise the camera object based
				// on the values from the script
				view.push_back(new Camera(x, y, z, cam_rot, fov, m_render_target->GetWindowWidth(), m_render_target->GetWindowHeight(), n_clip, f_clip));

			}
		}

		// Similar process is followed 
		// for the following asset types

		if (strstr("Player", asset_type) != 0)
		{
			fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

			m_player_node = new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState);

			// We add the player node as a child of the root node
			m_root_node->AddChildNode(m_player_node);

		}

		if (strstr("Enemy", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				// Initialising the enemy nodes
				m_enemy_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));
				m_enemy_nodes[i]->InitialisePatrolSpots();

				objs.push_back(m_enemy_nodes[i]);

				m_root_node->AddChildNode(m_enemy_nodes[i]);

			}


		}


		if (strstr("Spear", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_spear_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));

				objs.push_back(m_spear_nodes[i]);

				m_root_node->AddChildNode(m_spear_nodes[i]);

			}

			
		}

		if (strstr("Club", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_eweapon_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));

				m_enemy_nodes[i]->AddChildNode(m_eweapon_nodes[i]);

			}


		}

		if (strstr("Block", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_block_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));

				objs.push_back(m_block_nodes[i]);

				m_root_node->AddChildNode(m_block_nodes[i]);

			}


		}

		if (strstr("Food", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_food_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));

				//objs.push_back(m_food_nodes[i]);

				m_root_node->AddChildNode(m_food_nodes[i]);

			}


		}


		if (strstr("Statik", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_statik_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));
				objs.push_back(m_statik_nodes[i]);
				m_root_node->AddChildNode(m_statik_nodes[i]);

			}

			// Initialised is set to true
			// when completed
			initialised = true;

			// The file is closed
			fclose(assetFile);
		}

		
	}


}

void Game::RestartGame()
{
	// Initialised is reset to false
	initialised = false;

	// We open the file again
	fopen_s(&assetFile, "Assets/Scripts/Asset_Details.txt", "r");

	// Reset the player back to the original position
	m_player_node->ResetToInitalPos();
	m_player_node->m_p_asset->ResetPlayerHealth();
	game_timer = 0;

	// Looping through enemies who are alive
	for (int i = 0; i < m_enemy_nodes.size(); i++)
	{
		for (int j = 0; j < objs.size(); j++)
		{
			// Erase the enemy from the all
			// objects vector
			if (objs[j] == m_enemy_nodes[i])
			{
				objs.erase(objs.begin() + j);
				break;
			}
		}

		// Detach the enemy from the root node
		m_root_node->DetachNode(m_enemy_nodes[i]);

		m_enemy_nodes[i]->ReleaseAll();

	}

	// Clear both the vectors of nodes
	m_enemy_nodes.clear();
	m_eweapon_nodes.clear();

	// The enemy and enemy weapon
	// nodes are being re-initialised

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	float x_scle = 0.0f;
	float y_scle = 0.0f;
	float z_scle = 0.0f;

	int num_assets;

	int gravityState = 0;

	char assetObj[256];
	char textureFile[256];

	char asset_type[256];
	char node_type;

	while (initialised == false)
	{
		fscanf(assetFile, "%s", asset_type);

		if (strstr("Enemy", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_enemy_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));
				m_enemy_nodes[i]->InitialisePatrolSpots();

				objs.push_back(m_enemy_nodes[i]);

				m_root_node->AddChildNode(m_enemy_nodes[i]);

			}


		}

		if (strstr("Club", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_eweapon_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));

				m_enemy_nodes[i]->AddChildNode(m_eweapon_nodes[i]);

			}

			initialised = true;
			fclose(assetFile);

		}
	}

}

void Game::InstantiateWeapon(float x, float y, float z)
{
	fopen_s(&new_wep_file, "Assets/Scripts/New_Weapon.txt", "r");

	char node_type;

	float x_scale = 0.0f;
	float y_scale = 0.0f;
	float z_scale = 0.0f;

	char assetObj[256];

	char textureFile[256];

	int gravityState = 0;

	fscanf(new_wep_file, " %c %s %s %f %f %f %d", &node_type, &assetObj, &textureFile, &x_scale, &y_scale, &z_scale, &gravityState);

	m_spear_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile,
		x, y, z, x_scale, y_scale, z_scale, gravityState));

	objs.push_back(m_spear_nodes.back());

	m_root_node->AddChildNode(m_spear_nodes.back());

	fclose(new_wep_file);

}

bool Game::isGameRunning()
{
	return game_running;
}