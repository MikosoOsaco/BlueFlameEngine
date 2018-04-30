#include "LightningPlayer.h"

using namespace GAME;

LightningPlayer::LightningPlayer() {
	base = new PlayerBase("Resources/Models/Alex (Electric)/Models/Electric_0001.obj");
	base->SetWorldPosition(0.0f, 0.0f, 0.0f);
	base->SetWorldScale(0.012f);
	base->renderComponent->SetColour(0.2f, 0.7f, 0.0f);

	ring->renderComponent->SetColour(1.0f, 1.0f, 0.0f);
	HeavyCD = Cooldown(4.0);
	LightCD = Cooldown(0.4);
	MediumCD = Cooldown(2.5);
	SpecialDuration = Cooldown(3.0);

	shootEffect = new ParticleSystem(BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->GetShaderManager(), glm::vec3(1.0f, 1.0f, 0.0f));
	BFEngine::GetInstance()->GetSceneManager()->GetCurrentScene()->AddObject(shootEffect);

	dialogue = PlayerDialogue(2);
	dialogue.LoadPlayerDialogue("Resources/Audio/AlexTrixAudio.txt");

	SetStats();
}

LightningPlayer::~LightningPlayer() {

}

std::vector<Projectile*> LightningPlayer::LightAttack()
{
	std::vector<Projectile*> proj;

	//if (playerState == NORMAL && lightComboTimer <= 0)
	//{
	//	dialogue.playSpecifiedFromState(dialogue.RegularProjectile, 4);

	//	lightComboTimer = 0.2f;

	//	Projectile* p = Shocker(0.0f, 0.0f, 0.0f, 15.0f, 0.3f, 10.0f);
	//	p->SetDamage(4);
	//	proj.push_back(p);

	//	return proj;
	//}

	if (playerState == NORMAL && lightComboTimer <= 0)
	{
		lightComboPosition++;
		lightComboTimer = 0.4f;

		Projectile* p = Shocker(0.5f, 0.0f, 100.0f, 0.0f, 0.2f, 3.5f, 0.0f);
		p->SetDamage(2);
		p->SetMesh(PROJECTILE_MESH::NORM_ELEC);
		proj.push_back(p);

		Projectile* a = Shocker(-0.5f, 0.0f, 100.0f, 0.0f, 0.2f, 3.5f, 0.0f);
		a->SetDamage(2);
		a->SetMesh(PROJECTILE_MESH::NORM_ELEC);
		proj.push_back(a);
		return proj;
	}

	return proj;
}

std::vector<Projectile*> LightningPlayer::MediumAttack()
{
	std::vector<Projectile*> proj;

	if (playerState == NORMAL && mediumComboTimer <= 0)
	{
		dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 3);

		mediumComboTimer = 1.5f;

		Projectile* f = ForkedLightning(0);
		f->SetDamage(4);
		f->SetMesh(PROJECTILE_MESH::NORM_ELEC);
		proj.push_back(f);

		Projectile* o = ForkedLightning(-20.0);
		o->SetDamage(4);
		o->SetMesh(PROJECTILE_MESH::NORM_ELEC);
		proj.push_back(o);

		Projectile* r = ForkedLightning(20.0);
		r->SetDamage(4);
		r->SetMesh(PROJECTILE_MESH::NORM_ELEC);
		proj.push_back(r);

		/*	Projectile* k = ForkedLightning(-160.0);
		k->SetDamage(12);
		proj.push_back(k);

		Projectile* d = ForkedLightning(160.0);
		d->SetDamage(12);
		proj.push_back(d);*/

		return proj;
	}

	//if (playerState == NORMAL && mediumComboPosition == 0 && mediumComboTimer <= 0)
	//{
	//	dialogue.playSpecifiedFromState(dialogue.RegularProjectile, 2);

	//	mediumComboPosition++;
	//	mediumComboTimer = 1.0f;

	//	Projectile* p = LightningCloud(-10.0f, 1.0f);
	//	proj.push_back(p);

	//	Projectile* f = Shocker(0, -9.0f, -250.0f, 0.0f, 1.0f, -50.0f);
	//	p->SetDamage(8);
	//	proj.push_back(f);

	//	return proj;
	//}

	//if (playerState == ATTACK && mediumComboPosition == 1 && mediumComboTimer > 0)
	//{
	//	mediumComboPosition++;
	//	mediumComboTimer = 1.0f;

	//	Projectile* p = LightningCloud(-6.0f, 5.0f);
	//	proj.push_back(p);

	//	Projectile* a = Strike(0, 0, -5.0f, 4.0f, 60.0f);
	//	a->SetDamage(9);
	//	proj.push_back(a);

	//	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	//	p->SetStrength(PROJECTILE_STRENGTH::LIGHT);
	//	p->SetClipping(PROJECTILE_CLIP::YES_WALL_PLAYER);

	//	return proj;
	//}

	//if (playerState == ATTACK && mediumComboPosition == 2 && mediumComboTimer > 0)
	//{
	//	dialogue.playSpecifiedFromState(dialogue.RegularProjectile, 5);

	//	mediumComboPosition++;
	//	mediumComboTimer = 1.0f;

	//	Projectile* p = LightningCloud(-10.0f, 1.0f);
	//	proj.push_back(p);

	//	Projectile* f = Shocker(0, -9.0f, -250.0f, 0.0f, 0.8f, -50.0f);
	//	p->SetDamage(7);
	//	proj.push_back(f);

	//	return proj;
	//}

	return proj;
}

std::vector<Projectile*> LightningPlayer::HeavyAttack()
{
	std::vector<Projectile*> proj;

	if (playerState == NORMAL && heavyComboTimer <= 0)
	{
		dialogue.playSpecifiedFromState(dialogue.RegularProjectile, 2);
		heavyComboTimer = 3.0f;

		Projectile* p = LightningCloud(-10.0f, 1.0f);
		proj.push_back(p);

		Projectile* f = Shocker(0, -9.0f, -150.0f, 0.0f, 0.0f, 130.0f, 0.5f);
		f->SetDamage(25);
		f->SetMesh(PROJECTILE_MESH::NORM_ELEC);
		proj.push_back(f);

		return proj;
	}

	//if (playerState == NORMAL && heavyComboPosition == 0 && heavyComboTimer <= 0)
	//{
	//	dialogue.playSpecifiedFromState(dialogue.RegularProjectile, 0);

	//	heavyComboPosition++;
	//	heavyComboTimer = 1.5f;

	//	Projectile* p = Shocker(0.0f, 0.0f, 200.0f, 0.0f, 0.7f, 10.0f);
	//	p->SetWorldScale(0.3f, 0.3f, 0.3f);
	//	p->SetDamage(10);
	//	proj.push_back(p);

	//	return proj;
	//}

	//if (playerState == ATTACK && heavyComboPosition == 1 && heavyComboTimer > 0)
	//{
	//	heavyComboPosition++;
	//	heavyComboTimer = 1.5f;

	//	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + 1.0f + (0 * targetAngle * dir), GetWorldPosition().y + 1.0f, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z), targetAngle, dir);
	//	p->SetImpulseForce(glm::vec3(-15.0f, -15.0f, 200.0f));
	//	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f)); //no accel
	//	p->SetKnockbackForce(glm::vec3(0.0f, 10.0f, 30.0f)); //static
	//	p->SetDamage(12);
	//	p->SetStunTime(1.3f);
	//	p->SetWorldScale(0.5f, 0.5f, 0.5f);
	//	p->SetLifetime(3.0f);
	//	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	//	p->SetStrength(PROJECTILE_STRENGTH::LIGHT);
	//	p->SetClipping(PROJECTILE_CLIP::YES);

	//	proj.push_back(p);

	//	return proj;
	//}

	//if (playerState == ATTACK && heavyComboPosition == 2 && heavyComboTimer > 0)
	//{
	//	dialogue.playSpecifiedFromState(dialogue.RegularProjectile, 1);
	//	heavyComboPosition++;
	//	heavyComboTimer = 1.5f;

	//	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x - 1.0f + (0 * targetAngle * dir), GetWorldPosition().y + 1.0f, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z), targetAngle, dir);
	//	p->SetImpulseForce(glm::vec3(15.0f, -15.0f, 200.0f));
	//	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f)); //no accel
	//	p->SetKnockbackForce(glm::vec3(0.0f, 10.0f, 30.0f)); //static
	//	p->SetDamage(12);
	//	p->SetStunTime(1.2f);
	//	p->SetWorldScale(0.8f, 0.8f, 0.8f);
	//	p->SetLifetime(3.0f);
	//	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	//	p->SetStrength(PROJECTILE_STRENGTH::LIGHT);
	//	p->SetClipping(PROJECTILE_CLIP::YES);

	//	proj.push_back(p);

	//	return proj;
	//}

	return proj;
}

std::vector<Projectile*> LightningPlayer::SpecialAttack() //Zero Laser!
{
	std::vector<Projectile*> proj;

	if (playerState == NORMAL && specialMeter >= 100)
	{
		dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 4);
		specialMeter = 0;

		for (int i = 0; i < 10; i++) {
			Projectile* p = ZeroLaser(0.8f + 0.05f * i, 400.0f, 1.0f);
			p->SetDamage(3);
			p->SetMesh(PROJECTILE_MESH::ELEC_DISC);
			proj.push_back(p);
		}
		return proj;
	}

	//if (playerState == ATTACK && heavyComboPosition == 3 && specialMeter >= 25)
	//{
	//	dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 3);
	//	heavyComboPosition++;
	//	specialMeter -= 25;
	//	if (specialMeter < 0) {
	//		specialMeter = 0;
	//	}

	//	Projectile* f = ForkedLightning(0);
	//	f->SetDamage(12);
	//	proj.push_back(f);

	//	Projectile* o = ForkedLightning(-80.0);
	//	o->SetDamage(12);
	//	proj.push_back(o);

	//	Projectile* r = ForkedLightning(80.0);
	//	r->SetDamage(12);
	//	proj.push_back(r);

	//	Projectile* k = ForkedLightning(-160.0);
	//	k->SetDamage(12);
	//	proj.push_back(k);

	//	Projectile* d = ForkedLightning(160.0);
	//	d->SetDamage(12);
	//	proj.push_back(d);

	//	return proj;
	//}

	//if (playerState == ATTACK && mediumComboPosition == 2 && specialMeter >= 50)
	//{
	//	dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 2);

	//	mediumComboPosition++;
	//	specialMeter -= 50;
	//	if (specialMeter < 0) {
	//		specialMeter = 0;
	//	}

	//	Projectile* p = LightningCloud(-7.0f, 5.0f);
	//	proj.push_back(p);

	//	Projectile* a = Strike(-0.5f, 0, -6.0f, 4.0f, 30.0f);
	//	p->SetDamage(7);
	//	proj.push_back(a);

	//	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	//	p->SetStrength(PROJECTILE_STRENGTH::LIGHT);
	//	p->SetClipping(PROJECTILE_CLIP::YES_WALL_PLAYER);

	//	ComboReset();
	//	return proj;
	//}

	//if (playerState == ATTACK && lightComboPosition == 3 && specialMeter >= 15) {

	//	dialogue.playSpecifiedFromState(dialogue.SpecialProjectile, 0);
	//	lightComboPosition++;
	//	specialMeter -= 15;
	//	if (specialMeter < 0) {
	//		specialMeter = 0;
	//	}

	//	for (int i = 0; i < 3; i++) {
	//		Projectile* p = ZeroLaser(0.05f * i, 120.0f, 2.0f);
	//		p->SetDamage(2);
	//		proj.push_back(p);
	//	}
	//}
	return proj;
}

void LightningPlayer::InheritedUpdate(const float deltaTime)
{
	if (SpecialDuration.checkOffCD() && charging) {
		charging = false;
	}
	else if (charging) {
		Stun();
	}
}

void LightningPlayer::InheritedHandleEvents(SDL_Event events)
{

}

void LightningPlayer::InheritedHandleStates(const Uint8 *state)
{
	if (state[SDL_SCANCODE_Y]) {
		HeavyAttack();
	}
}

Projectile* LightningPlayer::LightningCloud(float offset, float height) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x - offset * targetAngle * dir, GetWorldPosition().y + height, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z + offset * dir), targetAngle, dir);
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f)); //static
	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 0.0f)); //static
	p->SetStunTime(0.0f);
	p->SetWorldScale(1.0f, 0.5f, 1.0f);
	p->SetLifetime(0.4f);
	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	p->SetStrength(PROJECTILE_STRENGTH::LIGHT);
	p->SetClipping(PROJECTILE_CLIP::YES_WALL_PLAYER);

	return p;
}

Projectile* LightningPlayer::Strike(float offsetx, float offsety, float offsetz, float height, float kBack) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x - offsetz * targetAngle * dir, GetWorldPosition().y + height, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z + offsetz * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, -100.0f, 0.0f));
	p->SetActingForce(glm::vec3(0.0f, -10.0f, 0.0f)); //no accel
	p->SetKnockbackForce(glm::vec3(0.0f, 50.0f, kBack)); //knocks them back. finisher
	p->SetStunTime(0.8f);
	p->SetWorldScale(0.2f, 1.0f, 0.2f);
	p->SetLifetime(0.5f);
	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	p->SetStrength(PROJECTILE_STRENGTH::MEDIUM);
	p->SetClipping(PROJECTILE_CLIP::YES_WALL_PLAYER);
	return p;
}

Projectile* LightningPlayer::ZeroLaser(float delay, float speed, float baseLifetime) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, speed));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f)); //no accel
	p->SetFirstDelay(delay, glm::vec3(0), glm::vec3(0.5f, 0.1f, 0.5f), glm::vec3(1.8f, 0.1f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
	p->SetKnockbackForce(glm::vec3(0.0f, 0.0f, 200.0f)); //static
	p->SetStunTime(0.5f);
	p->SetLifetime(delay + baseLifetime);
	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	p->SetStrength(PROJECTILE_STRENGTH::SPECIAL);
	p->SetClipping(PROJECTILE_CLIP::YES_PLAYER);
	return p;
}

Projectile* LightningPlayer::ForkedLightning(float offset) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(offset, 0.0f, 150.0f));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, 0.0f)); //no accel
	p->SetKnockbackForce(glm::vec3(0.0f, 10.0f, 30.0f)); //static
	p->SetStunTime(1.0f);
	p->SetWorldScale(0.2f, 0.2f, 0.2f);
	p->SetLifetime(0.4f);
	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	p->SetStrength(PROJECTILE_STRENGTH::HEAVY);
	p->SetClipping(PROJECTILE_CLIP::YES);
	return p;
}

Projectile* LightningPlayer::Shocker(float offsetx, float offsetz, float speed, float accel, float stun, float knockback, float delay) {
	Projectile* p = new Projectile(glm::vec3(GetWorldPosition().x + offsetx - (offsetz * targetAngle * dir), GetWorldPosition().y, GetWorldPosition().z - collisionComponent->GetBoundingBox().r.z * 2.0f * GetWorldScale().z + offsetz * dir), targetAngle, dir);
	p->SetImpulseForce(glm::vec3(0.0f, 0.0f, speed));
	p->SetActingForce(glm::vec3(0.0f, 0.0f, accel)); //no accel
	p->SetKnockbackForce(glm::vec3(0.0f, 20.0f, knockback)); //static
	p->SetStunTime(stun);
	p->SetFirstDelay(delay, glm::vec3(0), glm::vec3((0.25f, 0.25f, 0.5f)), glm::vec3(0.25f, 0.25f, 0.5f), glm::quat(0, 0, 0, 0));
	//p->SetWorldScale(0.25f, 0.25f, 0.5f);
	p->SetLifetime(3.0f);
	p->SetPlayer(PROJECTILE_PLAYER::PLAYER3);
	p->SetElement(PROJECTILE_ELEMENT::LIGHTNING);
	p->SetStrength(PROJECTILE_STRENGTH::LIGHT);
	p->SetClipping(PROJECTILE_CLIP::YES);
	return p;
}