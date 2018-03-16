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

	SetStats();

	movementCD = Cooldown(0.0);

	out = false;
}

Player::~Player()
{
}

void Player::Update(const float deltaTime)
{
	// Target Direction
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

	// Update collision and physics
	physicsComponent->Update(deltaTime);
	collisionComponent->Update(GetWorldPosition());
	SetWorldPosition(physicsComponent->GetPosition());
	if (playerTeam == PLAYERTEAM::TEAM1) {
		SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), -targetAngle);
	}
	else if (playerTeam == PLAYERTEAM::TEAM2) {
		SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), 3.14 - targetAngle);
	}
	if (playerState == NORMAL) {
		physicsComponent->SetVelocity(glm::vec3(0.0f, physicsComponent->GetVelocity().y, 0.0f));
	}

	// Update player model
	UpdateModel(deltaTime);

	// Update function from child
	InheritedUpdate(deltaTime);
}

void Player::HandleEvents(SDL_Event events) 
{
	// Child events
	InheritedHandleEvents(events);
}

void Player::HandleStates(const Uint8 *state)
{
	// Debug commands
	//----------------------------------------------
	if (state[SDL_SCANCODE_Q]) {
		Stun();
	}

	if (state[SDL_SCANCODE_E]) {
		if (playerState != STUN) {
			Block();
		}
	}
	else {
		if (playerState != STUN && playerState != ATTACK) {
			playerState = NORMAL;
		}
	}
	//----------------------------------------------

	// Child events
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
			//SetWorldPosition(GetWorldPosition() + glm::vec3(0.0f, 0.0f, -moveSpeed * deltaTime));
			physicsComponent->SetVelocity(glm::vec3(physicsComponent->GetVelocity().x, physicsComponent->GetVelocity().y, (-moveSpeed * moveMod) * deltaTime * 500 * dir));
			base->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), -0.2f);
		}
		if (movement == BACKWARD) {
			//SetWorldPosition(GetWorldPosition() + glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime));
			physicsComponent->SetVelocity(glm::vec3(physicsComponent->GetVelocity().x, physicsComponent->GetVelocity().y, (moveSpeed * moveMod) * deltaTime * 500 * dir));
			base->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.2f);
		}
		if (movement == RIGHT) {
			//SetWorldPosition(GetWorldPosition() + glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f));
			physicsComponent->SetVelocity(glm::vec3((moveSpeed * moveMod) * deltaTime * 500 * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
			base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), -0.2f);
		}
		if (movement == LEFT) {
			//SetWorldPosition(GetWorldPosition() + glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f));
			physicsComponent->SetVelocity(glm::vec3((-moveSpeed * moveMod) * deltaTime * 500 * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
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
		// Rotate Ring
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
	health = maxHealth;
	moveSpeed = 1.0f;
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
		health -= projectile->GetDamage();
		specialMeter += 10;
		if (specialMeter >= 100)
		{
			specialMeter = 100;
		}
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
	playerState = BLOCK;
	ResetModel();
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
	if (worldPosition.y < 0.1f && playerState == NORMAL) {
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

void Player::Render(Shader* shader)
{
	shader->setMat4("model", worldModelMatrix * localModelMatrix * base->GetWorldModelMatrix() * base->GetLocalModelMatrix() * base->model->GetWorldModelMatrix() * base->model->GetLocalModelMatrix());
	base->Render(shader);

	if (worldPosition.y > 0.0f) {
		ring->SetWorldPosition(worldPosition.x, ring->GetWorldPosition().y, worldPosition.z);
		shader->setMat4("model", ring->GetWorldModelMatrix() * ring->GetLocalModelMatrix());
	}
	else {
		ring->SetWorldPosition(0.0f, ring->GetWorldPosition().y, 0.0f);
		shader->setMat4("model", worldModelMatrix * localModelMatrix * ring->GetWorldModelMatrix() * ring->GetLocalModelMatrix());
	}
	ring->Render(shader);

	shader->setMat4("model", worldModelMatrix * localModelMatrix * shield->GetWorldModelMatrix() * shield->GetLocalModelMatrix());
	shield->Render(shader);

	if (isTargeting) {
		shader->setMat4("model", marker->GetWorldModelMatrix() * marker->GetLocalModelMatrix());
		marker->Render(shader);
	}
}