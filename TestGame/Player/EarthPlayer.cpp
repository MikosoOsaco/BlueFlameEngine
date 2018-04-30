#include "EarthPlayer.h"

using namespace GAME;

EarthPlayer::EarthPlayer() {
	base = new PlayerBase("Resources/Models/Flint (Earth)/Models/Earth_0001.obj");
	base->SetWorldPosition(0.0f, 0.0f, 0.0f);
	base->SetWorldScale(0.012f);
	base->renderComponent->SetColour(0.2f, 0.7f, 0.0f);

	ring->renderComponent->SetColour(0.8, 0.5f, 0.3f);
	HeavyCD = Cooldown(5.0);
	LightCD = Cooldown(0.4);
	MediumCD = Cooldown(3.0);

	shootEffect = new ParticleSystem(BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->GetShaderManager(), glm::vec3(0.8, 0.5f, 0.3f));
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(shootEffect);

	dialogue = PlayerDialogue(2);
	dialogue.LoadPlayerDialogue("Resources/Audio/FlintDamascusAudio.txt");

	SetStats();
}

EarthPlayer::~EarthPlayer() {

}

std::vector<Projectile*> EarthPlayer::LightAttack()
{
	std::vector<Projectile*> proj;
	/*
	if (playerState == NORMAL && lightComboPosition == 0 && lightComboTimer <= 0)
	{
	lightComboTimer = 0.5f;
	lightComboPosition++;

	Projectile* p = smallRock(0, 0);
	proj.push_back(p);


	return proj;
	}*/

	if (playerState == NORMAL && lightComboTimer <= 0)
	{
		//dialogue.playSpecifiedFromState(dialogue.RegularProjectile, 1);
		lightComboTimer = 0.4f;

		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
			targetAngle, dir);

		p->SetElement(ENGINE::PROJECTILE_ELEMENT::EARTH);
		p->SetStrength(PROJECTILE_STRENGTH::LIGHT);
		p->SetImpulseForce(glm::vec3(0.0f, 100.0f, 150.0f));
		p->SetActingForce(glm::vec3(0.0f, -5.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 20.0f, 20.0f));
		p->SetStunTime(0.2f);
		p->SetWorldScale(0.7f);
		p->SetDamage(5);
		p->SetMesh(PROJECTILE_MESH::NORM_EARTH);

		proj.push_back(p);
		return proj;
	}
	return proj;
}

std::vector<Projectile*> EarthPlayer::MediumAttack()
{
	std::vector<Projectile*> proj;

	/*if (playerState == NORMAL && mediumComboPosition == 0 && mediumComboTimer <= 0)
	{
	mediumComboPosition++;
	mediumComboTimer = 1.0f;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + collisionComponent->GetBoundingBox().r.y / 1.5f * GetWorldScale().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
	targetAngle, dir);

	p->SetElement(ENGINE::PROJECTILE_ELEMENT::EARTH);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 250.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, -20.0f, 15.0f));
	p->SetWorldScale(0.5f);
	p->SetStunTime(0.8f);
	p->SetDamage(6);
	proj.push_back(p);
	return proj;
	}*/
	/*
	if (playerState == NORMAL && mediumComboTimer <= 0)
	{
	mediumComboPosition++;
	mediumComboTimer = 1.5f;

	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, -200.0f, 300.0f));
	p->SetActingForce(glm::vec3(0.0f, 10.0f, -7.5f));
	p->SetKnockbackForce(glm::vec3(0.0f, 150.0f, 30.0f));
	p->SetStunTime(1.2f);
	p->SetWorldScale(0.5f);
	p->SetDamage(10);
	p->SetLifetime(0.8);
	p->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
	p->SetElement(PROJECTILE_ELEMENT::EARTH);
	p->SetStrength(PROJECTILE_STRENGTH::MEDIUM);
	proj.push_back(p);
	return proj;
	}*/

	if (playerState == NORMAL && mediumComboTimer <= 0)
	{
		mediumComboTimer = 6.0f;

		dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 2);
		//wall
		Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, -3.0f, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
		p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 0.0f));
		p->SetStunTime(0.0f);
		p->SetFirstDelay(0.2f, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		p->SetElement(PROJECTILE_ELEMENT::EARTH);
		p->SetStrength(PROJECTILE_STRENGTH::SPECIAL);
		p->SetClipping(PROJECTILE_CLIP::YES_PROJECTILE);
		p->SetMesh(PROJECTILE_MESH::EARTH_ULT);
		p->SetLifetime(2.7f);

		proj.push_back(p);
	}

	//if (playerState == ATTACK && mediumComboPosition == 2 && mediumComboTimer > 0)
	//{
	//	dialogue.playSpecifiedFromState(dialogue.RegularProjectile, 0);

	//	mediumComboPosition++;
	//	mediumComboTimer = 0.8f;

	//	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y + collisionComponent->GetBoundingBox().r.y / 1.5f * GetWorldScale().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir),
	//		targetAngle, dir);

	//	p->SetElement(ENGINE::PROJECTILE_ELEMENT::EARTH);
	//	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 300.0f));
	//	p->SetFirstDelay(0.4f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	//	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	//	p->SetKnockbackForce(glm::vec3(0.0f, -200.0f, 15.0f));
	//	p->SetStunTime(0.8f);
	//	p->SetDamage(10);
	//	proj.push_back(p);
	//	return proj;
	//}
	return proj;
}

std::vector<Projectile*> EarthPlayer::HeavyAttack()
{
	std::vector<Projectile*> proj;

	if (playerState == NORMAL && heavyComboTimer <= 0) {

		//dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 2);

		heavyComboTimer = 3.0f;

		dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 1);

		//Projectile* p0 = stalagmite(1.0f, 0.8f);
		Projectile* p2 = stalagmite(2.0f, 0.4f);
		p2->SetMesh(PROJECTILE_MESH::EARTH_STALAGMITES);
		//Projectile* p3 = stalagmite(3.0f, 1.0f);
		Projectile* p4 = stalagmite(4.0f, 0.5f);
		p4->SetMesh(PROJECTILE_MESH::EARTH_STALAGMITES);
		//Projectile* p5 = stalagmite(5.0f, 1.2f);
		Projectile* p6 = stalagmite(6.0f, 0.6f);
		p6->SetMesh(PROJECTILE_MESH::EARTH_STALAGMITES);
		//Projectile* p7 = stalagmite(7.0f, 1.4f);
		Projectile* p8 = stalagmite(8.0f, 0.7f);
		p8->SetMesh(PROJECTILE_MESH::EARTH_STALAGMITES);

		//proj.push_back(p0);
		proj.push_back(p2);
		//proj.push_back(p3);
		proj.push_back(p4);
		//proj.push_back(p5);
		proj.push_back(p6);
		//proj.push_back(p7);
		proj.push_back(p8);
		return proj;
	}

	//if (playerState == NORMAL && heavyComboPosition == 0 && heavyComboTimer <= 0)
	//{
	//	heavyComboPosition++;
	//	heavyComboTimer = 1.2f;

	//	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + 0.8, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);

	//	p->SetElement(PROJECTILE_ELEMENT::EARTH);
	//	p->SetStrength(PROJECTILE_STRENGTH::HEAVY);
	//	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 200.0f));
	//	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	//	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 20.0f));
	//	p->SetStunTime(1.0);
	//	p->SetWorldScale(glm::vec3(1.5f, 0.5f, 0.25f));
	//	p->SetDamage(12);

	//	proj.push_back(p);
	//	return proj;
	//}

	//if (playerState == ATTACK && heavyComboPosition == 1 && heavyComboTimer > 0)
	//{
	//	dialogue.playSpecifiedFromState(dialogue.RegularProjectile, 2);
	//	heavyComboPosition++;
	//	heavyComboTimer = 1.2f;

	//	Projectile* p = new Projectile(glm::vec3
	//		(GetWorldPosition().x - 0.8, GetWorldPosition().y, GetWorldPosition().z -
	//			collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);

	//	p->SetElement(PROJECTILE_ELEMENT::EARTH);
	//	p->SetStrength(PROJECTILE_STRENGTH::HEAVY);
	//	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 200.0f));
	//	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	//	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 20.0f));
	//	p->SetStunTime(1.0f);
	//	p->SetWorldScale(glm::vec3(1.5f, 0.5f, 0.25f));
	//	p->SetDamage(12);

	//	proj.push_back(p);
	//	return proj;
	//}

	return proj;
}

std::vector<Projectile*> EarthPlayer::SpecialAttack()
{
	std::vector<Projectile*> proj;

	if (playerState == NORMAL && specialMeter >= 100)
	{
		dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 0);

		specialMeter = 0;

		Projectile* e = SpecialRock(0.0f);
		e->SetMesh(PROJECTILE_MESH::EARTH_ULT);

		proj.push_back(e);

		Projectile* a = SpecialRock(2.0f);
		a->SetMesh(PROJECTILE_MESH::EARTH_ULT);

		proj.push_back(a);

		Projectile* r = SpecialRock(4.0f);
		r->SetMesh(PROJECTILE_MESH::EARTH_ULT);

		proj.push_back(r);

		Projectile* t = SpecialRock(-2.0f);
		t->SetMesh(PROJECTILE_MESH::EARTH_ULT);

		proj.push_back(t);

		Projectile* h = SpecialRock(-4.0f);
		h->SetMesh(PROJECTILE_MESH::EARTH_ULT);

		proj.push_back(h);
	}
	/*
	if (lightComboPosition == 2 && specialMeter >= 35) {

	specialMeter -= 35;
	if (specialMeter < 0) {
	specialMeter = 0;
	}

	Projectile* p = smallRock(0, 0);
	proj.push_back(p);
	Projectile* p2 = smallRock(-0.5, -100.0);
	Projectile* p3 = smallRock(0.5, 100.0);
	proj.push_back(p2);
	proj.push_back(p3);

	ComboReset();
	}
	*/
	return proj;
}

void EarthPlayer::InheritedUpdate(const float deltaTime)
{
}
void EarthPlayer::InheritedHandleEvents(SDL_Event events)
{
}
void EarthPlayer::InheritedHandleStates(const Uint8 *state)
{
	if (state[SDL_SCANCODE_Y]) {
		HeavyAttack();
	}
}

Projectile* EarthPlayer::smallRock(float offset, float xxx) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + offset, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(xxx, 100.0f, 180.0f));
	p->SetActingForce(glm::vec3(0.0f, -5.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 5.0f, 20.0f));
	p->SetStunTime(0.3f);
	p->SetWorldScale(0.35f);
	p->SetDamage(3);
	p->SetElement(PROJECTILE_ELEMENT::EARTH);
	p->SetStrength(PROJECTILE_STRENGTH::LIGHT);
	return p;
}

Projectile* EarthPlayer::SpecialRock(float offset) {
	Projectile* p = new Projectile(glm::vec3(offset, GetWorldPosition().y - 1.75f, -1.5f * -dir), 0.0f, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 70.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, 200.0f));
	p->SetFirstDelay(1.0f, glm::vec3(0.0f, 1.0f, 0.5f), glm::vec3(1.5f, 1.0f, 2.0f), glm::vec3(1.5f, 1.0f, 2.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetStunTime(3.5f);
	p->SetElement(PROJECTILE_ELEMENT::EARTH);
	p->SetStrength(PROJECTILE_STRENGTH::SPECIAL);
	p->SetClipping(YES_PLAYER_PROJECTILE);
	p->SetLifetime(8.0f);
	p->SetDamage(45);

	return p;
}

Projectile* EarthPlayer::stalagmite(float offset, float delay) {
	glm::vec3 startPos = glm::vec3(GetWorldPosition().x + (offset * targetAngle * dir), 0.0f, GetWorldPosition().z - (offset * dir) - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir);
	Projectile* p = new Projectile(startPos, targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, 0.0f));
	p->SetActingForce(glm::vec3(0.0f, 5.0f, 0.0f));
	p->SetFirstDelay(delay, glm::vec3(0), glm::vec3(0.5f, 0.8f, 0.5f), glm::vec3(0.5f, 0.8f, 0.5f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 20.0f, 100.0f));
	p->SetStunTime(1.0f);
	p->SetLifetime(delay + 0.5f);
	p->SetElement(PROJECTILE_ELEMENT::EARTH);
	p->SetStrength(PROJECTILE_STRENGTH::SPECIAL);
	p->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
	p->SetDamage(20);
	return p;
}