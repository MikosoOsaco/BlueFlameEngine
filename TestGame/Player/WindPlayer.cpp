#include "WindPlayer.h"

using namespace GAME;

WindPlayer::WindPlayer() {
	base = new PlayerBase("Resources/Models/Oki (Wind)/Model/Wind_0001.obj");
	base->SetWorldPosition(0.0f, 0.0f, 0.0f);
	base->SetWorldScale(0.012f);
	base->renderComponent->SetColour(0.2f, 0.7f, 0.0f);

	ring->renderComponent->SetColour(0.0f, 1.0f, 0.0f);

	shootEffect = new ParticleSystem(BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->GetShaderManager(), glm::vec3(0.0f, 1.0f, 0.0f));
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(shootEffect);

	dialogue = PlayerDialogue(2);
	dialogue.LoadPlayerDialogue("Resources/Audio/OkiCaeliAudio.txt");

	SetStats();
}

WindPlayer::~WindPlayer() {

}

std::vector<Projectile*> WindPlayer::LightAttack()
{
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && lightComboTimer <= 0) {
		lightComboTimer = 0.3f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 180.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 25.0f, 25.0f));
		p->SetStunTime(0.2f);
		p->SetDamage(5);
		p->SetElement(WIND);
		p->SetMesh(PROJECTILE_MESH::NORM_WIND);
		p->SetLifetime(0.5f);
		projectiles.push_back(p);
		return projectiles;
	}

	/*
	if (playerState == NORMAL && lightComboPosition == 0 && playerState != BLOCK && playerState != STUN) {

	if (worldPosition.y > 0.1) {
	physicsComponent->SetVelocity(glm::vec3(0, 5, 0));
	}

	lightComboTimer = 0.5f;
	lightComboPosition++;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 200.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 25.0f, 10.0f));
	p->SetStunTime(0.3f);
	p->SetDamage(3);
	p->SetElement(WIND);
	projectiles.push_back(p);
	return projectiles;
	}
	if (lightComboTimer > 0 && lightComboPosition == 1 && playerState != BLOCK && playerState != STUN) {

	if (worldPosition.y > 0.1) {
	physicsComponent->SetVelocity(glm::vec3(0, 5, 0));
	}

	lightComboTimer = 0.5f;
	lightComboPosition++;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(125.0f, 0.0f, 200.0f));
	p->SetActingForce(glm::vec3(-10.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 100.0f, 10.0f));
	p->SetStunTime(0.3f);
	p->SetDamage(3);
	p->SetWorldScale(0.5f);
	p->SetElement(WIND);
	projectiles.push_back(p);
	return projectiles;
	}
	if (lightComboTimer > 0 && lightComboPosition == 2 && playerState != BLOCK && playerState != STUN) {

	if (worldPosition.y > 0.1) {
	physicsComponent->SetVelocity(glm::vec3(0, 5, 0));
	}

	lightComboTimer = 0.5f;
	lightComboPosition++;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(-200.0f, 335.0f, 375.0f));
	p->SetActingForce(glm::vec3(20.0f, -35.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, -100.0f, 0.0f));
	p->SetStunTime(0.3f);
	p->SetDamage(4);
	p->SetWorldScale(0.5f);
	p->SetElement(WIND);
	projectiles.push_back(p);
	dialogue.playSpecifiedFromState(dialogue.RegularProjectile, 0);
	return projectiles;
	}
	*/
	return projectiles;
}
std::vector<Projectile*> WindPlayer::MediumAttack()
{
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && mediumComboTimer <= 0) {
		mediumComboTimer = 1.5f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 200.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 25.0f, 10.0f));
		p->SetFirstDelay(0.1f, glm::vec3(0.0f, 0.0f, 1.0f * dir), glm::vec3(0.0f), glm::vec3(1.0, 0.5f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetStunTime(0.0f);
		p->SetDamage(4);
		p->SetElement(WIND);
		p->SetMesh(PROJECTILE_MESH::NORM_WIND);
		projectiles.push_back(p);

		Projectile* p1 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p1->SetImpulseForce(glm::vec3(125.0f, 0.0f, 200.0f));
		p1->SetActingForce(glm::vec3(-10.0f, 0.0f, 0.0f));
		p1->SetKnockbackForce(glm::vec3(0.0f, 100.0f, 10.0f));
		p1->SetFirstDelay(0.3f, glm::vec3(0.0f, 0.0f, 1.0f * dir), glm::vec3(0.0f), glm::vec3(1.0, 0.5f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p1->SetStunTime(0.0f);
		p1->SetDamage(4);
		p1->SetElement(WIND);
		p1->SetMesh(PROJECTILE_MESH::NORM_WIND);
		projectiles.push_back(p1);

		Projectile* p2 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p2->SetImpulseForce(glm::vec3(-200.0f, 335.0f, 375.0f));
		p2->SetActingForce(glm::vec3(20.0f, -35.0f, 0.0f));
		p2->SetKnockbackForce(glm::vec3(0.0f, -100.0f, 0.0f));
		p2->SetFirstDelay(0.5f, glm::vec3(0.0f, 0.0f, 1.0f * dir), glm::vec3(0.0f), glm::vec3(1.0, 0.5f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p2->SetStunTime(0.0f);
		p2->SetDamage(6);
		p2->SetElement(WIND);
		p2->SetMesh(PROJECTILE_MESH::NORM_WIND);
		projectiles.push_back(p2);

		dialogue.playSpecifiedFromState(dialogue.RegularProjectile, 0);

		return projectiles;
	}

	/*
	if (playerState == NORMAL && mediumComboPosition == 0 && playerState != BLOCK && playerState != STUN) {

	mediumComboTimer = 0.5f;
	mediumComboPosition++;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(50.0f, 0.0f, 150.0f));
	p->SetActingForce(glm::vec3(-2.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(20.0f, 25.0f, 5.0f));
	p->SetStunTime(0.4f);
	p->SetDamage(8);
	p->SetWorldScale(0.5f);
	p->SetElement(WIND);
	projectiles.push_back(p);
	return projectiles;
	}
	if (mediumComboTimer > 0 && mediumComboTimer < 0.2f && mediumComboPosition == 1 && playerState != BLOCK && playerState != STUN) {

	mediumComboTimer = 0.5f;
	mediumComboPosition++;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(50.0f, 0.0f, 150.0f));
	p->SetActingForce(glm::vec3(-2.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(20.0f, 25.0f, 5.0f));
	p->SetStunTime(0.4f);
	p->SetDamage(8);
	p->SetWorldScale(0.5f);
	p->SetElement(WIND);
	projectiles.push_back(p);
	return projectiles;
	}
	if (mediumComboTimer > 0 && mediumComboTimer < 0.2f && mediumComboPosition == 2 && playerState != BLOCK && playerState != STUN) {

	mediumComboTimer = 1.0f;
	mediumComboPosition++;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 130.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 150.0f, 5.0f));
	p->SetStunTime(0.9f);
	p->SetDamage(10);
	p->SetWorldScale(0.5f);
	p->SetElement(WIND);
	projectiles.push_back(p);
	dialogue.playSpecifiedFromState(dialogue.RegularProjectile, 1);
	return projectiles;
	}
	*/
	return projectiles;
}
std::vector<Projectile*> WindPlayer::HeavyAttack()
{
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && heavyComboTimer <= 0) {
		heavyComboTimer = 3.0f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 80.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 30.0f, 95.0f));
		p->SetStunTime(1.0f);
		p->SetDamage(30);
		p->SetWorldScale(1.5f, 1.5f, 0.2f);
		p->SetElement(WIND);
		p->SetMesh(PROJECTILE_MESH::WIND_WALL);
		projectiles.push_back(p);
		dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 1);
		return projectiles;
	}

	/*
	if (playerState == NORMAL && heavyComboPosition == 0 && playerState != BLOCK && playerState != STUN) {

	heavyComboTimer = 0.7f;
	heavyComboPosition++;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 75.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 20.0f));
	p->SetStunTime(0.6f);
	p->SetDamage(14);
	p->SetWorldScale(1.0f, 0.2f, 1.0f);
	p->SetElement(WIND);
	projectiles.push_back(p);
	return projectiles;
	}
	if (heavyComboTimer > 0 && heavyComboTimer < 0.2f && heavyComboPosition == 1 && playerState != BLOCK && playerState != STUN) {

	heavyComboTimer = 1.2f;
	heavyComboPosition++;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 75.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 25.0f));
	p->SetStunTime(1.1f);
	p->SetDamage(20);
	p->SetWorldScale(0.2f, 1.0f, 1.0f);
	p->SetElement(WIND);
	projectiles.push_back(p);
	dialogue.playSpecifiedFromState(dialogue.RegularProjectile, 2);
	return projectiles;
	}
	if (lightComboPosition == 3) {

	lightComboTimer = 1.0f;
	lightComboPosition++;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 25.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 70.0f, 25.0f));
	p->SetStunTime(2.0f);
	p->SetDamage(30);
	p->SetWorldScale(1.5f, 1.5f, 0.2f);
	p->SetElement(WIND);
	projectiles.push_back(p);
	dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 1);
	return projectiles;
	}
	*/
	return projectiles;
}
std::vector<Projectile*> WindPlayer::SpecialAttack()
{
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && specialMeter >= 100) {

		specialMeter = 0;
		dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 3);

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 150.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 200.0f));
		p->SetFirstDelay(0.5f, glm::vec3(0.0f, 0.0f, 1.0f * dir), glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(6.0f, 0.2f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetStunTime(2.0f);
		p->SetDamage(0);
		p->SetElement(WIND);
		p->SetMesh(PROJECTILE_MESH::WIND_DISC);
		projectiles.push_back(p);

		Projectile* p1 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p1->SetImpulseForce(glm::vec3(0.0f, 0.0f, 150.0f));
		p1->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p1->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 200.0f));
		p1->SetFirstDelay(1.0f, glm::vec3(0.0f, 0.0f, 1.0f * dir), glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(4.5f, 0.2f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p1->SetStunTime(2.0f);
		p1->SetDamage(0);
		p1->SetElement(WIND);
		p1->SetMesh(PROJECTILE_MESH::WIND_DISC);
		projectiles.push_back(p1);

		Projectile* p2 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p2->SetImpulseForce(glm::vec3(0.0f, 0.0f, 150.0f));
		p2->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p2->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 400.0f));
		p2->SetFirstDelay(1.5f, glm::vec3(0.0f, 0.0f, 1.0f * dir), glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(3.0f, 0.2f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p2->SetStunTime(2.0f);
		p2->SetDamage(0);
		p2->SetElement(WIND);
		p2->SetMesh(PROJECTILE_MESH::WIND_DISC);
		projectiles.push_back(p2);

		return projectiles;

	}
	/*
	if ((playerState == NORMAL && worldPosition.y < 0.1f && specialMeter >= 10) || mediumComboPosition == 3) {

	if (mediumComboPosition != 3) {
	specialMeter -= 10;
	if (specialMeter < 0) {
	specialMeter = 0;
	}
	}

	physicsComponent->SetPosition(glm::vec3(physicsComponent->GetPosition().x, physicsComponent->GetPosition().y + 2, physicsComponent->GetPosition().z));
	base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.2f);

	physicsComponent->SetVelocity(glm::vec3(0, 5, 0));

	ComboReset();

	dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 0);
	}

	if (heavyComboPosition == 2 && specialMeter >= 25) {

	specialMeter -= 25;
	if (specialMeter < 0) {
	specialMeter = 0;
	}

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 200.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 150.0f));
	p->SetStunTime(0.5f);
	p->SetDamage(6);
	p->SetWorldScale(3.0f, 0.2f, 1.0f);
	p->SetElement(WIND);
	projectiles.push_back(p);
	dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 3);
	return projectiles;
	}

	if (playerState == NORMAL && worldPosition.y > 0.1f && !airAttack && specialMeter >= 100) {

	specialMeter -= 100;
	if (specialMeter < 0) {
	specialMeter = 0;
	}

	physicsComponent->SetVelocity(glm::vec3(0, 5, 0));

	airAttack = true;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 200.0f, 200.0f));
	p->SetActingForce(glm::vec3(0.0f, -25.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 100.0f, 100.0f));
	p->SetStunTime(2.5f);
	p->SetDamage(50);
	p->SetWorldScale(6.0f, 0.2f, 1.0f);
	projectiles.push_back(p);
	p->SetElement(WIND);
	dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 5);
	return projectiles;
	}

	if (lightComboPosition == 3 && specialMeter > 50) {
	ComboReset();

	specialMeter -= 50;
	if (specialMeter < 0) {
	specialMeter = 0;
	}

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(00.0f, 0.0f, 500.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 25.0f));
	p->SetStunTime(0.3f);
	p->SetDamage(2);
	p->SetWorldScale(0.5f);
	projectiles.push_back(p);
	p->SetElement(WIND);
	dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 2);
	return projectiles;
	}
	*/
	return projectiles;
}
void WindPlayer::InheritedUpdate(const float deltaTime)
{
	/*
	if (worldPosition.y < 0.1f) {
	airAttack = false;
	}

	if (playerState == ATTACK)
	{
	physicsComponent->SetVelocity(glm::vec3(0.0f, physicsComponent->GetVelocity().y, 0.0f));
	}

	if (mediumComboPosition == 1) {
	physicsComponent->SetVelocity(glm::vec3(moveSpeed * 1.1f *  deltaTime * 500 * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
	base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), -0.2f);
	}
	if (mediumComboPosition == 2) {
	physicsComponent->SetVelocity(glm::vec3(-moveSpeed * 1.1f * deltaTime * 500 * dir, physicsComponent->GetVelocity().y, physicsComponent->GetVelocity().z));
	base->SetLocalRotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.2f);
	}
	*/
}
void WindPlayer::InheritedHandleEvents(SDL_Event events)
{

}
void WindPlayer::InheritedHandleStates(const Uint8 *state)
{

}