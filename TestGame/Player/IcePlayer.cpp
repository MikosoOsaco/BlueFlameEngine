#include "IcePlayer.h"

using namespace GAME;

IcePlayer::IcePlayer() {
	base = new PlayerBase("Resources/Models/Jack (Ice)/Models/Ice_0001.obj");
	base->SetWorldPosition(0.0f, 0.0f, 0.0f);
	base->SetWorldScale(0.012f);
	base->renderComponent->SetColour(0.2f, 0.7f, 0.0f);

	ring->renderComponent->SetColour(0.4f, 0.7f, 1.0f);
	HeavyCD = Cooldown(5.0);
	LightCD = Cooldown(0.4);
	MediumCD = Cooldown(1.5);
	moveWhileShooting = true;

	shootEffect = new ParticleSystem(BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->GetShaderManager(), glm::vec3(0.4f, 0.7f, 1.0f));
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(shootEffect);

	dialogue = PlayerDialogue(7);
	dialogue.LoadPlayerDialogue("Resources/Audio/JackColeAudio.txt");

	SetStats();
}

IcePlayer::~IcePlayer() {

}

std::vector<Projectile*> IcePlayer::LightAttack()
{
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && lightComboTimer <= 0) {

		lightComboTimer = 0.4f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 90.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 20.0f));
		p->SetFirstDelay(0.15f, glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.25f, 0.25f, 0.25f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetSecondDelay(0.15f, glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetDamage(3);
		p->SetStunTime(0.2f);
		p->SetMesh(PROJECTILE_MESH::NORM_ICE);
		projectiles.push_back(p);

		Projectile* p1 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p1->SetImpulseForce(glm::vec3(0.0f, 0.0f, 90.0f));
		p1->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p1->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 20.0f));
		p1->SetFirstDelay(0.15f, glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.25f, 0.25f, 0.25f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p1->SetSecondDelay(0.15f, glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p1->SetDamage(3);
		p1->SetStunTime(0.2f);
		p1->SetClipping(YES);
		p1->SetElement(ICE);
		p1->SetMesh(PROJECTILE_MESH::NORM_ICE);
		projectiles.push_back(p1);

		//dialogue.sometimesPlayRandomFromOtherState(PlayerDialogue::RegularProjectile, 2, false);
	}
	/*
	if (playerState == NORMAL && lightComboTimer <= 0.0f && lightComboPosition == 0 && playerState != BLOCK && playerState != STUN) {

	lightComboTimer = 0.5f;
	lightComboPosition++;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x - 0.25f, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 70.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 30.0f));
	p->SetFirstDelay(0.05f, glm::vec3(0.0f), glm::vec3(0.05f, 0.05f, 0.25f), glm::vec3(0.2f, 0.2f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetDamage(2);
	p->SetStunTime(0.3f);
	p->SetClipping(YES);
	p->SetElement(ICE);
	projectiles.push_back(p);
	}
	else if (lightComboTimer > 0.0f && lightComboPosition == 1 && playerState != BLOCK && playerState != STUN) {

	lightComboTimer = 0.5f;
	lightComboPosition++;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + 0.25f, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 70.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 30.0f));
	p->SetFirstDelay(0.05f, glm::vec3(0.0f), glm::vec3(0.05f, 0.05f, 0.25f), glm::vec3(0.2f, 0.2f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetDamage(2);
	p->SetStunTime(0.3f);
	p->SetClipping(YES);
	p->SetElement(ICE);
	projectiles.push_back(p);
	}

	else if (lightComboTimer > 0.0f && lightComboPosition == 2 && playerState != BLOCK && playerState != STUN) {

	lightComboTimer = 1.3f;
	lightComboPosition++;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 70.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 20.0f));
	p->SetFirstDelay(0.15f, glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.25f, 0.25f, 0.25f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetSecondDelay(0.15f, glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetDamage(2);
	p->SetStunTime(0.5f);
	projectiles.push_back(p);

	Projectile* p1 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p1->SetImpulseForce(glm::vec3(0.0f, 0.0f, 70.0f));
	p1->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p1->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 20.0f));
	p1->SetFirstDelay(0.15f, glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.25f, 0.25f, 0.25f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p1->SetSecondDelay(0.15f, glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p1->SetDamage(2);
	p1->SetStunTime(0.5f);
	p1->SetClipping(YES);
	p1->SetElement(ICE);
	projectiles.push_back(p1);

	dialogue.sometimesPlayRandomFromOtherState(PlayerDialogue::RegularProjectile, 2, false);
	}
	*/
	return projectiles;
}
std::vector<Projectile*> IcePlayer::MediumAttack()
{
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && mediumComboTimer <= 0) {
		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 100.0f));
		p->SetActingForce(glm::vec3(0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 20.0f, 120.0f));
		p->SetFirstDelay(0.2f, glm::vec3(0.0f, -0.7f, 0.5f), glm::vec3(0.5f, 0.25f, 0.5f), glm::vec3(1.5f, 0.5f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetClipping(YES_PLAYER);
		p->SetDamage(10);
		p->SetStunTime(1.0f);
		p->SetElement(ICE);
		p->SetMesh(PROJECTILE_MESH::ICE_RAMP);
		projectiles.push_back(p);

		mediumComboTimer = 1.5f;

		dialogue.sometimesPlayRandomFromOtherState(PlayerDialogue::RegularProjectile, 2, false);
	}
	/*
	if (MediumCD.checkOffCD()) {
	if (playerState == NORMAL && mediumComboPosition == 0 && playerState != BLOCK && playerState != STUN) {

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 100.0f));
	p->SetActingForce(glm::vec3(0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 160.0f, 0.0f));
	p->SetFirstDelay(0.2f, glm::vec3(0.0f, -0.7f, 0.5f), glm::vec3(0.5f, 0.25f, 0.5f), glm::vec3(1.5f, 0.5f, 0.75f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetClipping(YES_PLAYER);
	p->SetDamage(10);
	p->SetStunTime(1.0f);
	p->SetElement(ICE);
	projectiles.push_back(p);

	mediumComboPosition++;
	mediumComboTimer = 1.0f;

	movementCD.setNewDuration(1.5f);
	movementCD.startCD();

	dialogue.sometimesPlayRandomFromOtherState(PlayerDialogue::RegularProjectile, 2, false);

	}
	else if (mediumComboPosition == 1 && playerState != BLOCK && playerState != STUN) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y - 0.5f, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 120.0f, 200.0f));
	p->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 20.0f));
	p->SetFirstDelay(0.5f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.9f, 0.9f, 0.9f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetStunTime(0.7f);
	p->SetClipping(YES);
	p->SetDamage(8);
	p->SetElement(ICE);
	projectiles.push_back(p);
	mediumComboPosition++;

	mediumComboTimer = 1.0f;

	MediumCD.startCD();

	dialogue.sometimesPlayRandomFromOtherState(PlayerDialogue::RegularProjectile, 2, false);
	}
	}
	*/
	return projectiles;
}
std::vector<Projectile*> IcePlayer::HeavyAttack()
{
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && heavyComboTimer <= 0) {

		heavyComboTimer = 3.0f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + 0.1f, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 60.0f));
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 20.0f, 120.0f));
		p->SetFirstDelay(0.15f, glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.15f, 0.15f, 0.15f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetSecondDelay(0.15f, glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.55f, 1.15f, 1.35f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetDamage(25);
		p->SetClipping(YES_PLAYER_PROJECTILE);
		p->SetElement(ICE);
		p->SetStunTime(0.0f);
		p->SetLifetime(4.0f);
		p->SetMesh(PROJECTILE_MESH::ICE_WALL);
		projectiles.push_back(p);

		dialogue.sometimesPlayRandomFromOtherState(PlayerDialogue::RegularProjectile, 2, false);
	}
	/*
	if (playerState == NORMAL && heavyComboTimer <= 0 && heavyComboPosition == 1 && playerState != BLOCK && playerState != STUN) {

	heavyComboTimer = 2.0f;
	heavyComboPosition = 0;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + 0.1f, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 30.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 25.0f, 40.0f));
	p->SetFirstDelay(0.15f, glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.15f, 0.15f, 0.15f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetSecondDelay(0.15f, glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.55f, 1.15f, 1.35f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetDamage(14);
	p->SetClipping(YES_PLAYER_PROJECTILE);
	p->SetElement(ICE);
	p->SetStunTime(1.5f);
	p->SetLifetime(4.0f);
	projectiles.push_back(p);

	movementCD.setNewDuration(0.7f);
	movementCD.startCD();

	dialogue.sometimesPlayRandomFromOtherState(PlayerDialogue::RegularProjectile, 2, false);
	}
	*/
	return projectiles;
}
std::vector<Projectile*> IcePlayer::SpecialAttack()
{
	std::vector<Projectile*> projectiles;

	if (playerState == NORMAL && specialMeter >= 100) {
		specialMeter = 0;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 120.0f));
		p->SetActingForce(glm::vec3(0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 200.0f));
		p->SetStunTime(2.0f);
		p->SetFirstDelay(0.4f, glm::vec3(-2.5f, 0.0f, 0.0f), glm::vec3(0.35f, 0.35f, 0.25f), glm::vec3(0.35f, 0.35f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetSecondDelay(0.6f, glm::vec3(0.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
		p->SetElement(ICE);
		p->SetDamage(0);
		p->SetMesh(PROJECTILE_MESH::ICE_ULT);
		projectiles.push_back(p);

		Projectile* p1 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p1->SetImpulseForce(glm::vec3(0.0f, 0.0f, 120.0f));
		p1->SetActingForce(glm::vec3(0.0f));
		p1->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 200.0f));
		p1->SetStunTime(2.0f);
		p1->SetFirstDelay(0.5f, glm::vec3(2.5f, 0.0f, 0.0f), glm::vec3(0.35f, 0.35f, 0.35f), glm::vec3(0.35f, 0.35f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p1->SetSecondDelay(0.5f, glm::vec3(0.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p1->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
		p1->SetElement(ICE);
		p1->SetDamage(0);
		p1->SetMesh(PROJECTILE_MESH::ICE_ULT);
		projectiles.push_back(p1);

		Projectile* p2 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p2->SetImpulseForce(glm::vec3(0.0f, 0.0f, 120.0f));
		p2->SetActingForce(glm::vec3(0.0f));
		p2->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 200.0f));
		p2->SetStunTime(2.0f);
		p2->SetFirstDelay(0.6f, glm::vec3(-1.25f, 0.0f, 0.0f), glm::vec3(0.35f, 0.35f, 0.35f), glm::vec3(0.35f, 0.35f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p2->SetSecondDelay(0.4f, glm::vec3(0.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p2->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
		p2->SetElement(ICE);
		p2->SetDamage(0);
		p2->SetMesh(PROJECTILE_MESH::ICE_ULT);
		projectiles.push_back(p2);

		Projectile* p3 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p3->SetImpulseForce(glm::vec3(0.0f, 0.0f, 120.0f));
		p3->SetActingForce(glm::vec3(0.0f));
		p3->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 200.0f));
		p3->SetStunTime(2.0f);
		p3->SetFirstDelay(0.7f, glm::vec3(1.25f, 0.0f, 0.0f), glm::vec3(0.35f, 0.35f, 0.35f), glm::vec3(0.35f, 0.35f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p3->SetSecondDelay(0.3f, glm::vec3(0.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p3->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
		p3->SetElement(ICE);
		p3->SetDamage(0);
		p3->SetMesh(PROJECTILE_MESH::ICE_ULT);
		projectiles.push_back(p3);

		Projectile* p4 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p4->SetImpulseForce(glm::vec3(0.0f, 0.0f, 120.0f));
		p4->SetActingForce(glm::vec3(0.0f));
		p4->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 200.0f));
		p4->SetStunTime(2.0f);
		p4->SetFirstDelay(0.8f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.35f, 0.35f, 0.35f), glm::vec3(0.35f, 0.35f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p4->SetSecondDelay(0.2f, glm::vec3(0.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p4->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
		p4->SetElement(ICE);
		p4->SetDamage(0);
		p4->SetMesh(PROJECTILE_MESH::ICE_ULT);
		projectiles.push_back(p4);

		dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 1);
	}

	/*
	if (specialComboTimer <= 0 && mediumComboPosition == 2 && specialMeter >= 35) {

	specialMeter -= 35;
	if (specialMeter < 0) {
	specialMeter = 0;
	}

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 100.0f));
	p->SetActingForce(glm::vec3(0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 45.0f));
	p->SetFirstDelay(0.4f, glm::vec3(0.0f, -0.9f, 0.5f), glm::vec3(0.5f, 0.25f, 0.5f), glm::vec3(2.0f, 1.0f, 1.00f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
	p->SetStunTime(1.25f);
	p->SetElement(ICE);
	p->SetDamage(30);
	projectiles.push_back(p);

	mediumComboTimer = 1.0f;
	specialComboTimer = 1.1f;

	dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 0);
	}
	else if (lightComboPosition == 3 && specialMeter >= 75) {
	specialMeter -= 75;
	if (specialMeter < 0) {
	specialMeter = 0;
	}

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 80.0f));
	p->SetActingForce(glm::vec3(0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 70.0f));
	p->SetStunTime(2.0f);
	p->SetFirstDelay(0.4f, glm::vec3(-2.5f, 0.0f, 0.0f), glm::vec3(0.35f, 0.35f, 0.25f), glm::vec3(0.35f, 0.35f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetSecondDelay(0.6f, glm::vec3(0.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
	p->SetElement(ICE);
	p->SetDamage(60);
	projectiles.push_back(p);

	Projectile* p1 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p1->SetImpulseForce(glm::vec3(0.0f, 0.0f, 80.0f));
	p1->SetActingForce(glm::vec3(0.0f));
	p1->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 70.0f));
	p1->SetStunTime(2.0f);
	p1->SetFirstDelay(0.5f, glm::vec3(2.5f, 0.0f, 0.0f), glm::vec3(0.35f, 0.35f, 0.35f), glm::vec3(0.35f, 0.35f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p1->SetSecondDelay(0.5f, glm::vec3(0.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p1->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
	p1->SetElement(ICE);
	p1->SetDamage(60);
	projectiles.push_back(p1);

	Projectile* p2 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p2->SetImpulseForce(glm::vec3(0.0f, 0.0f, 80.0f));
	p2->SetActingForce(glm::vec3(0.0f));
	p2->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 70.0f));
	p2->SetStunTime(2.0f);
	p2->SetFirstDelay(0.6f, glm::vec3(-1.25f, 0.0f, 0.0f), glm::vec3(0.35f, 0.35f, 0.35f), glm::vec3(0.35f, 0.35f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p2->SetSecondDelay(0.4f, glm::vec3(0.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p2->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
	p2->SetElement(ICE);
	p2->SetDamage(60);
	projectiles.push_back(p2);

	Projectile* p3 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p3->SetImpulseForce(glm::vec3(0.0f, 0.0f, 80.0f));
	p3->SetActingForce(glm::vec3(0.0f));
	p3->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 70.0f));
	p3->SetStunTime(2.0f);
	p3->SetFirstDelay(0.7f, glm::vec3(1.25f, 0.0f, 0.0f), glm::vec3(0.35f, 0.35f, 0.35f), glm::vec3(0.35f, 0.35f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p3->SetSecondDelay(0.3f, glm::vec3(0.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p3->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
	p3->SetElement(ICE);
	p3->SetDamage(60);
	projectiles.push_back(p3);

	Projectile* p4 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p4->SetImpulseForce(glm::vec3(0.0f, 0.0f, 80.0f));
	p4->SetActingForce(glm::vec3(0.0f));
	p4->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 70.0f));
	p4->SetStunTime(2.0f);
	p4->SetFirstDelay(0.8f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.35f, 0.35f, 0.35f), glm::vec3(0.35f, 0.35f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p4->SetSecondDelay(0.2f, glm::vec3(0.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p4->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
	p4->SetElement(ICE);
	p4->SetDamage(60);
	projectiles.push_back(p4);

	dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 1);

	specialComboTimer = 1.3;

	movementCD.setNewDuration(1.5);
	movementCD.startCD();
	}

	else if (playerState == NORMAL && specialComboTimer <= 0 && lightComboPosition != 3 && mediumComboPosition != 2 && specialMeter >= 25) {
	specialMeter -= 25;
	if (specialMeter < 0) {
	specialMeter = 0;
	}

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + 1.0f, GetWorldPosition().z + 1.0f * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, -2.0f, 0.0f));
	p->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f));
	p->SetFirstDelay(0.75f, glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.45f, 1.5f, 0.45f), glm::vec3(0.45f, 1.5f, 0.45f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetSecondDelay(1.25f, glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetStunTime(3.0f);
	p->SetClipping(YES_WALL_PLAYER);
	p->SetElement(ICE);
	p->SetDamage(20);
	projectiles.push_back(p);

	Projectile* p1 = new Projectile(glm::vec3(GetWorldPosition().x - 1.0f * dir, GetWorldPosition().y + 1.0f, GetWorldPosition().z), targetAngle, dir);
	p1->SetImpulseForce(glm::vec3(0.0f, -2.0f, 0.0f));
	p1->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
	p1->SetKnockbackForce(glm::vec3(0.0f));
	p1->SetFirstDelay(0.75f, glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.45f, 1.5f, 0.45f), glm::vec3(0.45f, 1.5f, 0.45f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p1->SetSecondDelay(1.25f, glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p1->SetStunTime(3.0f);
	p1->SetClipping(YES_WALL_PLAYER);
	p1->SetElement(ICE);
	p1->SetDamage(20);
	projectiles.push_back(p1);

	Projectile* p2 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + 1.0f, GetWorldPosition().z - 1.0f * dir), targetAngle, dir);
	p2->SetImpulseForce(glm::vec3(0.0f, -2.0f, 0.0f));
	p2->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
	p2->SetKnockbackForce(glm::vec3(0.0f));
	p2->SetFirstDelay(0.75f, glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.45f, 1.5f, 0.45f), glm::vec3(0.45f, 1.5f, 0.45f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p2->SetSecondDelay(1.25f, glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p2->SetStunTime(3.0f);
	p2->SetClipping(YES_WALL_PLAYER);
	p2->SetElement(ICE);
	p2->SetDamage(20);
	projectiles.push_back(p2);

	Projectile* p3 = new Projectile(glm::vec3(GetWorldPosition().x + 1.0f * dir, GetWorldPosition().y + 1.0f, GetWorldPosition().z), targetAngle, dir);
	p3->SetImpulseForce(glm::vec3(0.0f, -2.0f, 0.0f));
	p3->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
	p3->SetKnockbackForce(glm::vec3(0.0f));
	p3->SetFirstDelay(0.75f, glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.45f, 1.5f, 0.45f), glm::vec3(0.45f, 1.5f, 0.45f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p3->SetSecondDelay(1.25f, glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p3->SetStunTime(3.0f);
	p3->SetClipping(YES_WALL_PLAYER);
	p3->SetElement(ICE);
	p3->SetDamage(20);
	projectiles.push_back(p3);


	Projectile* p4 = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + 1.0f, GetWorldPosition().z), targetAngle, dir);
	p4->SetImpulseForce(glm::vec3(0.0f, -2.0f, 0.0f));
	p4->SetActingForce(glm::vec3(0.0f, -4.0f, 0.0f));
	p4->SetKnockbackForce(glm::vec3(0.0f));
	p4->SetFirstDelay(0.75f, glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.45f, 1.5f, 0.45f), glm::vec3(0.45f, 1.5f, 0.45f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p4->SetSecondDelay(1.25f, glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p4->SetStunTime(3.0f);
	p4->SetClipping(YES_WALL_PLAYER);
	p4->SetElement(ICE);
	p4->SetDamage(20);
	projectiles.push_back(p4);

	heavyComboPosition = 0;
	specialComboTimer = 1.0f;

	dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 2);

	movementCD.setNewDuration(1.0);
	movementCD.startCD();
	}
	*/
	return projectiles;
}
void IcePlayer::InheritedUpdate(const float deltaTime)
{

}
void IcePlayer::InheritedHandleEvents(SDL_Event events)
{

}
void IcePlayer::InheritedHandleStates(const Uint8 *state)
{

}