#include "Model.h"

using namespace ENGINE;

Model::Model()
{
}

Model::Model(std::string path)
{
	model = new ModelComponent(path);
	collisionComponent = new CollisionComponent();
	collisionComponent->CreateCollisionVolume(CollisionComponent::Collision_Type::BOX, model->meshes);
	collisionComponent->SetBoxPadding(glm::vec3(0.3f, 1.0f, 0.95f));
	physicsComponent = new PhysicsComponent();
}

Model::Model(std::string path, bool check)
{
	model = new ModelComponent(path);
}

Model::~Model()
{
}

void Model::Update(const float deltaTime) {
	model->Update();
}

void Model::FixedUpdate(const float deltaTime) {
	if (physicsComponent != NULL) {
		physicsComponent->Update(deltaTime);
		collisionComponent->Update(GetWorldPosition());
		SetWorldPosition(physicsComponent->GetPosition());
	}
}

void Model::Render(Shader* shader, const double _interpolation) {
	model->Render(shader);
}