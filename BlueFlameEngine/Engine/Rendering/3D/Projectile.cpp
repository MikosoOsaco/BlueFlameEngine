#include "Projectile.h"

using namespace ENGINE;

Projectile::Projectile(glm::vec3 p, float _angle, int _dir)
{
	physicsComponent = new PhysicsComponent();
	physicsComponent->SetElasticity(PhysicsComponent::Elastic_Type::PERFECT_NON_ELASTIC);
	physicsComponent->SetMaterialType(PhysicsComponent::Material_Type::ROUGH);
	physicsComponent->SetMass(50.0f);
	physicsComponent->SetDestructible(true);
	physicsComponent->hasGravity = false;

	glm::vec3 poop = glm::vec3(p.x + _angle * _dir, p.y, p.z);
	SetWorldPosition(poop);
	SetWorldScale(0.5f);
	SetWorldRotation(glm::vec3(0.0f, 1.0f, 0.0f), -_angle);

	dir = _dir;
	angle = _angle;
	impulseForce = glm::vec3(0.0f, 0.0f, 0.0f);
	actingForce = glm::vec3(0.0f, 0.0f, 0.0f);
	knockbackForce = glm::vec3(0.0f, 0.0f, 0.0f);

	delay = 0.0f;
	delay2 = 0.0f;
	delayTimer = 0.0f;
	delayTimer2 = 0.0f;
	delayIndex = 2;

	stunTime = 1.0f;
	damage = 1;
	lifetime = Cooldown(5.0);
	lifetime.startCD();

	canFlipX = false;
	canFlipY = false;
	flipIntervalX = 0.5f;
	flipIntervalY = 0.75f;
	flipTimeX = flipIntervalX / 2.0f;
	flipTimeY = flipIntervalY / 2.0f;

	element = PROJECTILE_ELEMENT::DEFAULT_ELEMENT;
	strength = PROJECTILE_STRENGTH::DEFAULT_STRENGTH;
	clipping = PROJECTILE_CLIP::YES;
	mesh = PROJECTILE_MESH::CUBE;
	team = PROJECTILE_TEAM::TEAM0;
	player = PROJECTILE_PLAYER::NONE;
}

Projectile::~Projectile() {

}

void Projectile::CreateCollision(RenderComponent* renderComponent, glm::vec3 p) {
	collisionComponent = new CollisionComponent();
	collisionComponent->CreateCollisionVolume(CollisionComponent::Collision_Type::BOX, renderComponent->getVertexList());
	collisionComponent->GetBoundingBox().c = p;
	collisionComponent->SetLayer(2);
}

void Projectile::SetImpulseForce(glm::vec3 _force) {
	impulseForce = glm::rotateY(_force, angle * dir);
	impulseForce.z *= -dir;
}

void Projectile::SetActingForce(glm::vec3 _force) {
	actingForce = glm::rotateY(_force, angle * dir);
	actingForce.z *= -dir;
}

void Projectile::SetKnockbackForce(glm::vec3 _force) {
	knockbackForce = glm::rotateY(_force, angle * dir);
	knockbackForce.z *= -dir;
}

void Projectile::SetFirstDelay(float _time, glm::vec3 _endPosition, glm::vec3 _beginScale, glm::vec3 _endScale, glm::quat _endRotation) {
	delayIndex = 0;
	delay = _time;
	delayTimer = 0.0f;
	beginPosition = worldPosition;
	_endPosition.x *= dir;
	_endPosition.z *= -dir;
	endPosition = glm::rotateY(_endPosition, -angle) + beginPosition;
	beginScale = _beginScale;
	endScale = _endScale;
	beginRotation = glm::quat(-angle, 0.0f, 1.0f, 0.0f);
	endRotation = _endRotation + beginRotation;
	if (endRotation.x == 0.0f && endRotation.y == 0.0f && endRotation.z == 0.0f && endRotation.w == 0.0f) {
		endRotation = glm::quat(-angle, 0.0f, 1.0f, 0.0f);
	}
	SetWorldPosition(worldPosition);
	SetWorldScale(beginScale);
}

void Projectile::SetSecondDelay(float _time, glm::vec3 _endPosition, glm::vec3 _endScale, glm::quat _endRotation) {
	if (delay != 0.0f) {
		delay2 = _time;
		delayTimer2 = 0.0f;
		_endPosition.x *= dir;
		_endPosition.z *= -dir;
		endPosition2 = glm::rotateY(_endPosition, -angle) + endPosition;
		endScale2 = _endScale + endScale;
		endRotation2 = _endRotation * endRotation;
		if (endRotation2.x == 0.0f && endRotation2.y == 0.0f && endRotation2.z == 0.0f && endRotation2.w == 0.0f) {
			endRotation2 = endRotation;
		}
	}
}

void Projectile::StopDelay() {
	delayIndex = 2;
}

void Projectile::SetStunTime(float time) {
	stunTime = time;
}

void Projectile::SetDamage(int d) {
	damage = d;
}

void Projectile::SetElement(PROJECTILE_ELEMENT _element) {
	element = _element;
}

void Projectile::SetStrength(PROJECTILE_STRENGTH _strength) {
	strength = _strength;
}

void Projectile::SetClipping(PROJECTILE_CLIP _clipping) {
	clipping = _clipping;
}

void Projectile::SetMesh(PROJECTILE_MESH _mesh) {
	mesh = _mesh;
}

void Projectile::SetTeam(PROJECTILE_TEAM _team) {
	team = _team;
}

void Projectile::SetPlayer(PROJECTILE_PLAYER _player) {
	player = _player;
}

void Projectile::AddMaxDistance(float distance) {
	maximumDistance = true;
	maxD = distance;
	beginPosition = worldPosition;
}

void Projectile::SetLifetime(double seconds) {
	lifetime.setNewDuration(seconds);
	lifetime.startCD();
}

glm::vec3 Projectile::GetForce() {
	return knockbackForce;
}

float Projectile::GetStunTime() {
	return stunTime;
}

int Projectile::GetDamage() {
	return damage;
}

PROJECTILE_ELEMENT Projectile::GetElement() {
	return element;
}

PROJECTILE_STRENGTH Projectile::GetStrength() {
	return strength;
}

PROJECTILE_CLIP Projectile::GetClipping() {
	return clipping;
}

PROJECTILE_MESH Projectile::GetMesh() {
	return mesh;
}

PROJECTILE_TEAM Projectile::GetTeam() {
	return team;
}

PROJECTILE_PLAYER Projectile::GetPlayer() {
	return player;
}

void Projectile::Update(const float deltaTime) {

	if (deleted == false && !lifetime.checkOffCD()) {

		if (maximumDistance == false || glm::length(worldPosition - beginPosition) <= maxD) {

			if (canFlipX) {
				flipTimeX += deltaTime;
			}

			if (canFlipY) {
				flipTimeY += deltaTime;
			}

			if (flipTimeX >= flipIntervalX) {
				actingForce.x = -actingForce.x;
				flipTimeX = 0.0f;
			}

			if (flipTimeY >= flipIntervalY) {
				actingForce.x = -actingForce.x;
				flipTimeY = 0.0f;
			}
		}
	}
}

void Projectile::FixedUpdate(const float deltaTime) {
	// This has to be done here 
	UpdatePreviousModelMatrix();

	if (deleted == false && !lifetime.checkOffCD()) {

		if (maximumDistance == false || glm::length(worldPosition - beginPosition) <= maxD) {

			if (delayIndex == 0) {

				if (delayTimer != 0.0f)
					SetWorldScale(glm::lerp(beginScale, endScale, delayTimer / delay));

				if (delayTimer != 0.0f)
					SetWorldPosition(glm::lerp(beginPosition, endPosition, delayTimer / delay));

				if (delayTimer != 0.0f) {
					glm::quat rotation = glm::lerp(beginRotation, endRotation, delayTimer / delay);
					SetWorldRotation(rotation.x, rotation.y, rotation.z, rotation.w);
				}

				delayTimer += deltaTime;
				if (delayTimer >= delay) {
					if (delay2 != 0.0f) {
						delayIndex++;
					}
					else {
						delayIndex = 2;
					}
				}
			}

			if (delayIndex == 1) {

				if (delayTimer2 != 0.0f)
					SetWorldScale(glm::lerp(endScale, endScale2, delayTimer2 / delay2));

				if (delayTimer2 != 0.0f)
					SetWorldPosition(glm::lerp(endPosition, endPosition2, delayTimer2 / delay2));

				if (delayTimer2 != 0.0f) {
					glm::quat rotation = glm::lerp(endRotation, endRotation2, delayTimer2 / delay2);
					SetWorldRotation(rotation.x, rotation.y, rotation.z, rotation.w);
				}

				delayTimer2 += deltaTime;
				if (delayTimer2 >= delay2) {
					delayIndex++;
				}
			}


			if (delayIndex == 2) {
				physicsComponent->AddForce(impulseForce);
				impulseForce = glm::vec3(0.0f);
				physicsComponent->AddForce(actingForce);
				physicsComponent->Update(deltaTime);
				SetWorldPosition(physicsComponent->GetPosition());
				collisionComponent->Update(physicsComponent->GetPosition());
			}
		}
	}
	else if (lifetime.checkOffCD()) {
		deleted = true;
	}
}

void Projectile::Render(Shader* shader, const double _interpolation)
{

}