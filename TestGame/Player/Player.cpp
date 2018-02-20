#include "Player.h"

using namespace GAME;

Player::Player()
{
	base = new PlayerBase();
	base->SetWorldPosition(0.0f, 0.35f, 0.0f);
	base->SetWorldScale(0.7f, 1.0f, 0.5f);
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

	health = maxHealth;
}

Player::~Player()
{
}

void Player::Update(const float deltaTime)
{
	if (playerState == STUN) {
		stunTimer -= deltaTime;
		if (stunTimer <= 0) {
			stunTimer = 0;
			playerState = NORMAL;
		}
	}	

	UpdateModel(deltaTime);
}

void Player::HandleEvents(SDL_Event events) 
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_Q]) {
		Stun();
	}

	if (state[SDL_SCANCODE_E]) {
		if (playerState != STUN) {
			Block();
		}
	}
	else {
		if (playerState != STUN) {
			playerState = NORMAL;
		}
	}
}

void Player::Movement(PLAYERMOVEMENT movement, const float deltaTime) {
	if (playerState == NORMAL) {
		if (movement == FORWARD) {
			SetWorldPosition(GetWorldPosition() + glm::vec3(0.0f, 0.0f, -moveSpeed * deltaTime));
			base->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), -0.1f);
		}
		if (movement == BACKWARD) {
			SetWorldPosition(GetWorldPosition() + glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime));
			base->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.1f);
		}
		if (movement == RIGHT) {
			SetWorldPosition(GetWorldPosition() + glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f));
			base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), -0.05f);
		}
		if (movement == LEFT) {
			SetWorldPosition(GetWorldPosition() + glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f));
			base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.05f);
		}
	}
}

void Player::UpdateModel(const float deltaTime) {
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
}

void Player::ResetModel() {
	// Set rotations back to normal
	base->SetLocalRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
	base->SetWorldRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
}

void Player::Stun() {
	stunTimer = stunTimerSet;
	playerState = STUN;
	ResetModel();
}

void Player::Block() {
	playerState = BLOCK;
	ResetModel();
}

void Player::Render(Shader* shader)
{
	shader->setMat4("model", worldModelMatrix * localModelMatrix * base->GetWorldModelMatrix() * base->GetLocalModelMatrix());
	base->Render(shader);

	shader->setMat4("model", worldModelMatrix * localModelMatrix * ring->GetWorldModelMatrix() * ring->GetLocalModelMatrix());
	ring->Render(shader);

	shader->setMat4("model", worldModelMatrix * localModelMatrix * shield->GetWorldModelMatrix() * shield->GetLocalModelMatrix());
	shield->Render(shader);
}