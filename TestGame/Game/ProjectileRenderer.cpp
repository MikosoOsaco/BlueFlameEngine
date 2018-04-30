#include "ProjectileRenderer.h"

using namespace ENGINE;
using namespace GAME;

ProjectileRenderer::ProjectileRenderer() {
	cubeMesh = new RenderComponent();
	cubeMesh->SetRenderType(RenderComponent::Render_Type::CUBE);

	/*
	FIX THE FOLLOWING
	--------------------------
	normfire		looking right
	stalagmites		looking up
	normEarth		looking right
	normIce			looking right
	iceUlt			looking up
	normElec		looking right
	normWind		looking right
	*/

	normFire = new Model("Resources/Models/Projectiles/Fire/Projectile_F_0001.obj");
	normFire->SetWorldScale(0.2f);
	normFire->SetLocalRotation(0.0f, 1.0f, 0.0f, -1.57f);
	fireMeteor = new Model("Resources/Models/Projectiles/Fire/Projectile_Meteor_0001.obj");
	fireMeteor->SetWorldScale(0.025f);

	normEarth = new Model("Resources/Models/Projectiles/Earth/Projectile_S_0001.obj");
	normEarth->SetWorldScale(0.2f);
	normEarth->SetLocalRotation(0.0f, 1.0f, 0.0f, -1.57f);
	earthStalagmites = new Model("Resources/Models/Projectiles/Earth/Projectile_EarthSpike_0001.obj");
	earthStalagmites->SetWorldScale(0.010f);
	earthStalagmites->SetLocalRotation(1.0f, 0.0f, 0.0f, 3.14f);
	earthUlt = new Model("Resources/Models/Projectiles/Earth/Projectile_Wall_Ice.obj");
	earthUlt->SetWorldScale(0.010f);

	//STILL NEEDS TO BE TEXTURED
	normIce = new Model("Resources/Models/Projectiles/Ice/Projectile_I_0001.obj");
	normIce->SetWorldScale(0.2f);
	normIce->SetLocalRotation(0.0f, 1.0f, 0.0f, -1.57f);
	iceRamp = new Model("Resources/Models/Projectiles/Ice/Projectile_Ramp_Ice.obj");
	iceRamp->SetWorldScale(0.010f);
	iceWall = new Model("Resources/Models/Projectiles/Ice/Projectile_Wall_Ice.obj");
	iceWall->SetWorldScale(0.010f);
	iceUlt = new Model("Resources/Models/Projectiles/Ice/Projectile_Icicle.obj");
	iceUlt->SetWorldScale(0.015f, 0.015f, 0.010f);
	iceUlt->SetLocalRotation(1.0f, 0.0f, 0.0f, -1.57f);

	normElec = new Model("Resources/Models/Projectiles/Elec/Projectile_E_0001.obj");
	normElec->SetWorldScale(0.2f);
	normElec->SetLocalRotation(0.0f, 1.0f, 0.0f, -1.57f);
	elecDisc = new Model("Resources/Models/Projectiles/Elec/Projectile_Shurniken_0001.obj");
	elecDisc->SetWorldScale(0.010f);

	normWind = new Model("Resources/Models/Projectiles/Wind/Projectile_W_0001.obj");
	normWind->SetWorldScale(0.2f);
	normWind->SetLocalRotation(0.0f, 1.0f, 0.0f, -1.57f);
	windDisc = new Model("Resources/Models/Projectiles/Wind/Projectile_Shurniken_0001.obj");
	windDisc->SetWorldScale(0.010f);
	windWall = new Model("Resources/Models/Projectiles/Wind/Projectile_Wall_Ice.obj");
	windWall->SetWorldScale(0.010f);

	projectileShader = new Shader("Shaders/projectile.vs", "Shaders/projectile.fs");
	shader = BFEngine::GetInstance()->GetSceneManager()->GetRenderer()->GetShaderManager()->put(std::string("projectile"), projectileShader);
}

ProjectileRenderer::~ProjectileRenderer() {

}

void ProjectileRenderer::Update(const float deltaTime) {

}

void ProjectileRenderer::Render(Shader* shader, const double _interpolation) {

	glm::mat4 interpolatedMatrix;

	for (unsigned int i = 0; i < projectiles.size(); ++i) {

		// Account for interpolation
		interpolatedMatrix = (projectiles.at(i)->GetWorldModelMatrix() * (float)_interpolation) + (projectiles.at(i)->GetPreviousWorldMatrix() * (1.0f - (float)_interpolation));

		// Don't interpolate on the first render 
		if (projectiles.at(i)->firstRender == true) {
			interpolatedMatrix = projectiles.at(i)->GetWorldModelMatrix();
		}

		if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::CUBE)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(0.0f, 0.0f, 0.0f));
			cubeMesh->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::NORM_FIRE)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				normFire->GetWorldModelMatrix() * normFire->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(-0.3f, -0.8f, -0.9f));
			normFire->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::FIRE_METEOR)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				fireMeteor->GetWorldModelMatrix() * fireMeteor->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(0.2f, 0.0f, 0.0f));
			fireMeteor->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::NORM_EARTH)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				normEarth->GetWorldModelMatrix() * normEarth->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(0.6, 0.3f, 0.1f));
			normEarth->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::EARTH_STALAGMITES)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				earthStalagmites->GetWorldModelMatrix() * earthStalagmites->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(0.3f, 0.1f, 0.0f));
			earthStalagmites->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::EARTH_ULT)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				earthUlt->GetWorldModelMatrix() * earthUlt->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(0.0f, 0.0f, 0.0f));
			earthUlt->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::NORM_ICE)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				normIce->GetWorldModelMatrix() * normIce->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(-0.5f, -0.2f, 0.1f));
			normIce->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::ICE_RAMP)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				iceRamp->GetWorldModelMatrix() * iceRamp->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(0.0f, 0.0f, 0.0f));
			iceRamp->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::ICE_WALL)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				iceWall->GetWorldModelMatrix() * iceWall->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(0.0f, 0.0f, 0.0f));
			iceWall->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::ICE_ULT)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				iceUlt->GetWorldModelMatrix() * iceUlt->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(0.0f, 0.0f, 0.0f));
			iceUlt->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::NORM_ELEC)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				normElec->GetWorldModelMatrix() * normElec->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(0.0f, 0.0f, -1.0f));
			normElec->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::ELEC_DISC)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				elecDisc->GetWorldModelMatrix() * elecDisc->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(5.0f, 5.0f, 0.0f));
			elecDisc->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::NORM_WIND)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				normWind->GetWorldModelMatrix() * normWind->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(-0.8f, 0.0f, -0.8f));
			normWind->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::WIND_DISC)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				windDisc->GetWorldModelMatrix() * windDisc->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(-0.5f, -0.3f, -0.5f));
			windDisc->model->Render(shader);
		}
		else if (projectiles.at(i)->GetMesh() == PROJECTILE_MESH::WIND_WALL)
		{
			shader->setMat4("model", interpolatedMatrix * projectiles.at(i)->GetLocalModelMatrix() *
				windWall->GetWorldModelMatrix() * windWall->GetLocalModelMatrix());
			shader->setVec3("Colour", glm::vec3(0.0f, 0.2f, 0.0f));
			windWall->model->Render(shader);
		}
	}
}
