#include "Player.h"

using namespace GAME;

Player::Player()
{
	// Make player parts
	base = new PlayerBase("Resources/Models/Robot_Base_Greybox/Robot_Var_002_Gurran.obj");
	base->SetWorldPosition(0.0f, 0.0f, 0.0f);
	base->SetWorldScale(0.012f);
	base->renderComponent->SetColour(0.2f, 0.7f, 0.0f);

	ring = new PlayerBase();
	ring->SetWorldPosition(0.0f, -0.3f, 0.0f);
	ring->SetWorldScale(0.5f, 0.2f, 0.5f);
	ring->renderComponent->SetColour(0.2f, 0.0f, 0.7f);

	shield = new PlayerBase();
	shield->SetWorldPosition(0.0f, 0.3f, -0.5f);
	shield->SetWorldScale(1.0f, 1.0f, 0.05f);
	shield->renderComponent->SetColour(0.0f, 0.2f, 0.7f);
	shield->renderComponent->CanRender(false);

	// Make player target marker
	marker = new PlayerBase();
	marker->SetWorldPosition(0.0f, 0.0f, 0.0f);
	marker->SetWorldScale(0.2f, 0.3f, 0.2f);
	marker->renderComponent->SetColour(1.0f, 0.0f, 0.0f);

	// Initialize collision component
	collisionComponent = new CollisionComponent();
	collisionComponent->CreateCollisionVolume(CollisionComponent::Collision_Type::BOX, base->renderComponent->getVertexList());
	collisionComponent->SetBoxPadding(glm::vec3(1.0f, 1.2f, 1.0f));

	// Initialize physics componenet
	physicsComponent = new PhysicsComponent();
	physicsComponent->SetAcceleration(glm::vec3(0.0f, -25.0f, 0.0f));
	physicsComponent->SetPhysicsType(PhysicsComponent::Physics_Type::DYNAMIC);
	physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::VERY_ELASTIC);
	physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::SMOOTH);
	physicsComponent->SetMass(50.0f);

	// Set up shader
	projectileShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
	pShader = BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->GetShaderManager()->put(std::string("projectile"), projectileShader);

	// Set direction to team 1 orientation by default
	dir = -1;

	// Set player to target another enemy by default
	isTargeting = true;

	// Set cooldowns
	movementCD = Cooldown(0.0);
	shieldCD = Cooldown(2.0);
	healthIncrementCD = Cooldown(0.1);

	// Set game gamplay status
	out = false;
	canMove = true;

	// Make player input
	playerInput = new PlayerInput();

	// Set player stats
	SetStats();
}

Player::~Player()
{
	// TODO: delete everything
}

void Player::AddProjecitleManager(ProjectileManager* pM)
{
	projectileManager = pM;
}

void Player::Update(const float deltaTime)
{

	if (specialMeter > 100) {
		specialMeter = 100;
	}

	if (shieldCD.checkOffCD() && healthIncrementCD.checkOffCD()) {
		if (shieldHealth < 100 && playerState != BLOCK) {
			shieldHealth++;
		}
		healthIncrementCD.startCD();
	}

	if (shieldHealth < 0) {
		shieldHealth = 0;
	}
	// Target Direction
	//
	if (isTargeting) {
		if (!enemyTeam.at(currentTarget)->out) {
			targetedPlayer = enemyTeam.at(currentTarget)->worldPosition;
			targetAngle = -glm::atan((targetedPlayer.x - worldPosition.x) / (targetedPlayer.z - worldPosition.z));
		}
		else if (enemyTeam.at(currentTarget)->out) {
			SwitchTarget();
		}
	}
	else if (!isTargeting) {
		targetedPlayer = glm::vec3(worldPosition);
		targetedPlayer.z += dir;
		targetAngle = -glm::atan((targetedPlayer.x - worldPosition.x) / (targetedPlayer.z - worldPosition.z));
	}

	// UPDATE TIMERS
	//
	// Update stun timer
	if (playerState == STUN) {
		stunTimer -= deltaTime;
		if (stunTimer <= 0) {
			stunTimer = 0;
			playerState = NORMAL;
		}
	}

	if (!movementCD.checkOffCD()) {
		playerState = ATTACK;
	}

	// Combo timers
	if (lightComboTimer > 0 || mediumComboTimer > 0 || heavyComboTimer > 0 || specialComboTimer > 0) {
		playerState = ATTACK;
	}

	if (lightComboTimer <= 0 && mediumComboTimer <= 0 && heavyComboTimer <= 0 && specialComboTimer <= 0 && playerState != STUN && playerState != BLOCK) {
		playerState = NORMAL;
	}

	lightComboTimer -= deltaTime;
	if (lightComboTimer <= 0 && playerState == ATTACK) {
		lightComboTimer = 0;
		lightComboPosition = 0;
	}

	mediumComboTimer -= deltaTime;
	if (mediumComboTimer <= 0 && playerState == ATTACK) {
		mediumComboTimer = 0;
		mediumComboPosition = 0;
	}

	heavyComboTimer -= deltaTime;
	if (heavyComboTimer <= 0 && playerState == ATTACK) {
		heavyComboTimer = 0;
		heavyComboPosition = 0;
	}

	specialComboTimer -= deltaTime;
	if (specialComboTimer <= 0 && playerState == ATTACK) {
		specialComboTimer = 0;
		specialComboPosition = 0;
	}
	//
}

void Player::FixedUpdate(const float deltaTime) {

	ring->UpdateState();
	shield->UpdateState();
	marker->UpdateState();

	shield->SetWorldScale(shieldHealth / 100.0f, shieldHealth / 100.0f, 0.05f);

	// Update collision and physics
	physicsComponent->Update(deltaTime);
	SetWorldPosition(physicsComponent->GetPosition());
	collisionComponent->Update(GetWorldPosition());

	if (playerState != STUN) {
		physicsComponent->SetVelocity(glm::vec3(0.0f, physicsComponent->GetVelocity().y, 0.0f));
	}

	// Update player model
	UpdateModel(deltaTime);

	// If player can move update movement 
	if (canMove) {
		if (playerInput->CheckForController()) {//if they have a controller
			glm::vec2 mods = playerInput->LeftJoystick();

			if (playerInput->LeftTriggerPressed()) {
				if (!out && shieldHealth > 0) {
					Block();
				}
			}
			else {
				StopBlock();
				if (mods.x > 0.01f) {
					Movement(Player::PLAYERMOVEMENT::RIGHT, deltaTime);
				}
				if (mods.x < -0.01f) {
					Movement(Player::PLAYERMOVEMENT::LEFT, deltaTime);
				}
				if (mods.y > 0.01f) {
					Movement(Player::PLAYERMOVEMENT::BACKWARD, deltaTime);
				}
				if (mods.y < -0.01f) {
					Movement(Player::PLAYERMOVEMENT::FORWARD, deltaTime);
				}
			}
			if (playerInput->RightTriggerPressed()) {
				if (!out) {
					Jump();
				}
			}
		}
	}

	// Update rotation
	if (playerTeam == PLAYERTEAM::TEAM1) {
		SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), -targetAngle);
	}
	else if (playerTeam == PLAYERTEAM::TEAM2) {
		SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), 3.14 - targetAngle);
	}

	// Update function from child
	InheritedUpdate(deltaTime);
}

void Player::HandleEvents(SDL_Event events)
{
	// Player can press buttons when not off the arena
	if (!out) {
		switch (events.type)
		{
		case SDL_JOYBUTTONDOWN:
			// X button
			if (events.jbutton.button == 2 && events.jbutton.which == SDL_JoystickInstanceID(playerInput->GetJoystick()))
			{
				std::vector<Projectile*> p = LightAttack();
				if (p.size() > 0) {
					for each (Projectile* subP in p) {

						if (playerTeam == PLAYERTEAM::TEAM1) {
							subP->SetTeam(PROJECTILE_TEAM::TEAM1);
						}
						else if (playerTeam == PLAYERTEAM::TEAM2) {
							subP->SetTeam(PROJECTILE_TEAM::TEAM2);
						}

						if (playerNumber == PLAYERNUMBER::PLAYER1) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER1);
						}
						else if (playerNumber == PLAYERNUMBER::PLAYER2) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER2);
						}
						else if (playerNumber == PLAYERNUMBER::PLAYER3) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER3);
						}
						else if (playerNumber == PLAYERNUMBER::PLAYER4) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER4);
						}

						projectileManager->AddProjectile(subP);
					}
				}
			}
			// Y button
			if (events.jbutton.button == 3 && events.jbutton.which == SDL_JoystickInstanceID(playerInput->GetJoystick()))
			{
				std::vector<Projectile*> p = MediumAttack();
				if (p.size() > 0) {
					for each (Projectile* subP in p) {

						if (playerTeam == PLAYERTEAM::TEAM1) {
							subP->SetTeam(PROJECTILE_TEAM::TEAM1);
						}
						else if (playerTeam == PLAYERTEAM::TEAM2) {
							subP->SetTeam(PROJECTILE_TEAM::TEAM2);
						}

						if (playerNumber == PLAYERNUMBER::PLAYER1) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER1);
						}
						else if (playerNumber == PLAYERNUMBER::PLAYER2) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER2);
						}
						else if (playerNumber == PLAYERNUMBER::PLAYER3) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER3);
						}
						else if (playerNumber == PLAYERNUMBER::PLAYER4) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER4);
						}

						projectileManager->AddProjectile(subP);
					}
				}
			}
			// B button
			if (events.jbutton.button == 1 && events.jbutton.which == SDL_JoystickInstanceID(playerInput->GetJoystick()))
			{
				std::vector<Projectile*> p = HeavyAttack();
				if (p.size() > 0) {
					for each (Projectile* subP in p) {

						if (playerTeam == PLAYERTEAM::TEAM1) {
							subP->SetTeam(PROJECTILE_TEAM::TEAM1);
						}
						else if (playerTeam == PLAYERTEAM::TEAM2) {
							subP->SetTeam(PROJECTILE_TEAM::TEAM2);
						}

						if (playerNumber == PLAYERNUMBER::PLAYER1) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER1);
						}
						else if (playerNumber == PLAYERNUMBER::PLAYER2) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER2);
						}
						else if (playerNumber == PLAYERNUMBER::PLAYER3) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER3);
						}
						else if (playerNumber == PLAYERNUMBER::PLAYER4) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER4);
						}

						projectileManager->AddProjectile(subP);
					}
				}
			}
			// A button
			if (events.jbutton.button == 0 && events.jbutton.which == SDL_JoystickInstanceID(playerInput->GetJoystick()))
			{
				std::vector<Projectile*> p = SpecialAttack();
				if (p.size() > 0) {
					for each (Projectile* subP in p) {

						if (playerTeam == PLAYERTEAM::TEAM1) {
							subP->SetTeam(PROJECTILE_TEAM::TEAM1);
						}
						else if (playerTeam == PLAYERTEAM::TEAM2) {
							subP->SetTeam(PROJECTILE_TEAM::TEAM2);
						}

						if (playerNumber == PLAYERNUMBER::PLAYER1) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER1);
						}
						else if (playerNumber == PLAYERNUMBER::PLAYER2) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER2);
						}
						else if (playerNumber == PLAYERNUMBER::PLAYER3) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER3);
						}
						else if (playerNumber == PLAYERNUMBER::PLAYER4) {
							subP->SetPlayer(PROJECTILE_PLAYER::PLAYER4);
						}

						projectileManager->AddProjectile(subP);
					}
				}
			}
			// Left Bumper
			if (events.jbutton.button == 4 && events.jbutton.which == SDL_JoystickInstanceID(playerInput->GetJoystick()))
			{
				EnableTarget();
			}
			// Right Bumper
			if (events.jbutton.button == 5 && events.jbutton.which == SDL_JoystickInstanceID(playerInput->GetJoystick()))
			{
				SwitchTarget();
			}
			break;

		case SDL_JOYHATMOTION:
			// DPAD
			if (events.jhat.value & SDL_HAT_UP)
			{
			}
			if (events.jhat.value & SDL_HAT_DOWN)
			{
			}
			if (events.jhat.value & SDL_HAT_LEFT)
			{
			}
			if (events.jhat.value & SDL_HAT_RIGHT)
			{
			}
			break;
		}

		// Child events
		InheritedHandleEvents(events);
	}
}

void Player::HandleStates(const Uint8 *state)
{
	InheritedHandleStates(state);
}

void Player::Movement(PLAYERMOVEMENT movement, const float deltaTime)
{
	if (playerState == NORMAL || (movementCD.checkOffCD() && moveWhileShooting)) {

		float moveMod = 1;

		if (worldPosition.y > 0.1f) {
			moveMod = 0.2f;
		}
		if (movement == FORWARD) {
			if (!out) {
				physicsComponent->SetVelocity(glm::vec3(physicsComponent->GetVelocity().x, physicsComponent->GetVelocity().y, (-moveSpeed * moveMod) * deltaTime * 500 * dir));
			}
			base->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), -0.2f);
		}
		if (movement == BACKWARD) {
			if (!out) {
				physicsComponent->SetVelocity(glm::vec3(physicsComponent->GetVelocity().x, physicsComponent->GetVelocity().y, (moveSpeed * moveMod) * deltaTime * 500 * dir));
			}
			base->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.2f);
		}
		if (movement == RIGHT) {
			if (!out) {
				physicsComponent->SetVelocity(glm::vec3((moveSpeed * moveMod) * deltaTime * 500 * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
			}
			base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), -0.2f);
		}
		if (movement == LEFT) {
			if (!out) {
				physicsComponent->SetVelocity(glm::vec3((-moveSpeed * moveMod) * deltaTime * 500 * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
			}
			base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.2f);
		}
	}
}

void Player::UpdateModel(const float deltaTime)
{
	if (playerState == NORMAL) {
		// Rotate Ring
		ring->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), ring->GetWorldRotationAngle() + 2.0f * deltaTime);

		// Move up and down
		if (localPosition.y >= 0.05f) {
			down = true;
		}
		else if (localPosition.y <= 0.0f) {
			down = false;
		}

		if (down) {
			SetLocalPosition(GetLocalPosition() - glm::vec3(0.0f, deltaTime / 20, 0.0f));
		}
		else {
			SetLocalPosition(GetLocalPosition() + glm::vec3(0.0f, deltaTime / 20, 0.0f));
		}

		ResetModel();
	}

	if (playerState == STUN) {
		ring->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), ring->GetWorldRotationAngle() - 10.0f * deltaTime);
		base->SetWorldRotation(glm::vec3(0.0f, 1.0f, 1.0f), base->GetWorldRotationAngle() + 0.3f * deltaTime);
	}

	if (playerState == BLOCK) {
		ring->SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), ring->GetWorldRotationAngle() - 0.3f * deltaTime);
		shield->SetWorldRotation(glm::vec3(0.0f, 0.0f, 1.0f), shield->GetWorldRotationAngle() + 10.0f * deltaTime);
		shield->renderComponent->CanRender(true);
	}
	else {
		shield->renderComponent->CanRender(false);
	}

	marker->SetWorldPosition(targetedPlayer.x, targetedPlayer.y + 1.5f, targetedPlayer.z);
}

void Player::ResetModel() {
	// Set rotations back to normal
	base->SetLocalRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
	base->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
}

void Player::SetStats() {
	shieldHealth = maxHealth;
	moveSpeed = 1.0f;
	specialMeter = 0;
}

void Player::SetEnemyTeam(Player* player1, Player* player2) {
	enemyTeam = { player1, player2 };
	SetTarget(player1->GetWorldPosition());
}

void Player::SetTarget(glm::vec3 targetPlayer) {
	targetedPlayer = targetPlayer;
}

void Player::SetTarget(int target) {
	currentTarget = target;
}

void Player::SetTargetColour(glm::vec3 colour) {
	targetColour = colour;
	marker->renderComponent->SetColour(targetColour.x, targetColour.y, targetColour.z);
}

void Player::SetPlayerTeam(PLAYERTEAM pT) {
	playerTeam = pT;
	if (playerTeam == PLAYERTEAM::TEAM1) {
		dir = 1;
	}
	else if (playerTeam == PLAYERTEAM::TEAM2) {
		dir = -1;
	}
}

void Player::Hit(Projectile* projectile) {
	if (playerState != BLOCK) {
		physicsComponent->AddForce(projectile->GetForce());
		Stun(projectile->GetStunTime());
		specialMeter += projectile->GetDamage() / 5;
		if (specialMeter >= 100)
		{
			specialMeter = 100;
		}
	}
	else if (shieldHealth <= 0) {
		shieldCD.startCD();
		physicsComponent->AddForce(projectile->GetForce() / 2.0f);
		Stun(2.0f);
	}

	if (playerState == BLOCK) {
		shieldHealth -= projectile->GetDamage();
		specialMeter += projectile->GetDamage() / 7;
		if (shieldHealth > 0) {
			shield->SetWorldScale(shieldHealth / 100.0f, shieldHealth / 100.0f, 0.05f);
		}
	}
	else if (shieldHealth <= 0) {
		shieldCD.startCD();
		physicsComponent->AddForce(projectile->GetForce() / 2.0f);
		Stun(2.0f);
	}
}

void Player::Stun() {
	ComboReset();
	stunTimer = stunTimerSet;
	playerState = STUN;
	ResetModel();
}

void Player::Stun(float stunTime) {
	stunTimer = stunTime;
	playerState = STUN;
	ResetModel();
}

void Player::Block() {
	if (shieldHealth > 0) {
		playerState = BLOCK;
	}
	else if (shieldHealth <= 0) {
		playerState = NORMAL;
	}
	ResetModel();
}

void Player::StopBlock() {
	if (playerState == BLOCK) {
		playerState = NORMAL;
		ResetModel();
	}
}

void Player::ComboReset() {
	lightComboTimer = 0;
	lightComboPosition = 0;
	mediumComboTimer = 0;
	mediumComboPosition = 0;
	heavyComboTimer = 0;
	heavyComboPosition = 0;
	specialComboTimer = 0;
	specialComboPosition = 0;
}

void Player::Jump() {
	if (worldPosition.y < 0.1f && worldPosition.y > -0.1f && playerState == NORMAL) {
		physicsComponent->AddForce(glm::vec3(0.0f, 110.0f, 0.0f));
	}
}

void Player::SwitchTarget() {
	if (currentTarget == 0) {
		currentTarget = 1;
	}
	else if (currentTarget == 1) {
		currentTarget = 0;
	}
}

void Player::EnableTarget() {
	if (isTargeting) {
		isTargeting = false;
	}
	else if (!isTargeting) {
		isTargeting = true;
	}
}

void Player::Render(Shader* shader, const double _interpolation)
{
	// Account for interpolation
	glm::mat4 interpolatedMatrix;
	interpolatedMatrix = worldModelMatrix * (float)_interpolation + previousWorldModelMatrix * (1.0f - (float)_interpolation);

	glm::mat4 ringInterpolatedMatrix;
	ringInterpolatedMatrix = ring->GetWorldModelMatrix() * (float)_interpolation + ring->GetPreviousWorldMatrix() * (1.0f - (float)_interpolation);

	glm::mat4 shieldInterpolatedMatrix;
	shieldInterpolatedMatrix = shield->GetWorldModelMatrix() * (float)_interpolation + shield->GetPreviousWorldMatrix() * (1.0f - (float)_interpolation);

	glm::mat4 markerInterpolatedMatrix;
	markerInterpolatedMatrix = marker->GetWorldModelMatrix() * (float)_interpolation + marker->GetPreviousWorldMatrix() * (1.0f - (float)_interpolation);

	shader->setMat4("model", interpolatedMatrix * localModelMatrix * base->GetWorldModelMatrix() * base->GetLocalModelMatrix() * base->model->GetWorldModelMatrix() * base->model->GetLocalModelMatrix());
	base->Render(shader, _interpolation);

	if (worldPosition.y > 0.0f) {
		ring->SetWorldPosition(worldPosition.x, ring->GetWorldPosition().y, worldPosition.z);
		shader->setMat4("model", ringInterpolatedMatrix * ring->GetLocalModelMatrix());
	}
	else {
		ring->SetWorldPosition(0.0f, ring->GetWorldPosition().y, 0.0f);
		shader->setMat4("model", interpolatedMatrix * localModelMatrix * ringInterpolatedMatrix * ring->GetLocalModelMatrix());
	}
	ring->Render(shader, _interpolation);

	shader->setMat4("model", interpolatedMatrix * localModelMatrix * shieldInterpolatedMatrix * shield->GetLocalModelMatrix());
	shield->Render(shader, _interpolation);

	if (isTargeting) {
		shader->setMat4("model", markerInterpolatedMatrix * marker->GetLocalModelMatrix());
		marker->Render(shader, _interpolation);
	}
}