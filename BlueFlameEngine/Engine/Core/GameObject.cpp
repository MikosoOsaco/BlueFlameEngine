#include "GameObject.h"

using namespace ENGINE;

GameObject::GameObject() :
	localPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	worldPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	localRotation(glm::vec3(1.0f, 1.0f, 1.0f)),
	worldRotation(glm::vec3(1.0f, 1.0f, 1.0f)),
	localScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	worldScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	localRotationAngle(0.0f),
	worldRotationAngle(0.0f),
	canRender(true)
{
}

GameObject::~GameObject()
{
}

// local position
void GameObject::SetLocalPosition(const float &x, const float &y, const float &z)
{
	localPosition = glm::vec3(x, y, z);
	UpdateLocalMatrix();
}
void GameObject::SetLocalPosition(const glm::vec3 &p)
{
	localPosition = p;
	UpdateLocalMatrix();
}
glm::vec3 GameObject::GetLocalPosition() const
{
	return localPosition;
}

// local rotation
void GameObject::SetLocalRotation(const float &x, const float &y, const float &z, const float &a)
{
	localRotation = glm::vec3(x, y, z);
	localRotationAngle = a;
	if (localRotationAngle >= 360 || localRotationAngle <= -360) {
		localRotationAngle = 0;
	}
	UpdateLocalMatrix();
}
void GameObject::SetLocalRotation(const glm::vec3 &r, const float &a)
{
	localRotation = r;
	localRotationAngle = a;
	if (localRotationAngle >= 360 || localRotationAngle <= -360) {
		localRotationAngle = 0;
	}
	UpdateLocalMatrix();
}
glm::vec3 GameObject::GetLocalRotation() const
{
	return localRotation;
}

// local scale
void GameObject::SetLocalScale(const float &x, const float &y, const float &z)
{
	localScale = glm::vec3(x, y, z);
	UpdateLocalMatrix();
}
void GameObject::SetLocalScale(const glm::vec3 &s)
{
	localScale = s;
	UpdateLocalMatrix();
}
void GameObject::SetLocalScale(const float &s)
{
	localScale = glm::vec3(s, s, s);
	UpdateLocalMatrix();
}
glm::vec3 GameObject::GetLocalScale() const
{
	return localScale;
}

// local rotation angle
float GameObject::GetLocalRotationAngle() const
{
	return localRotationAngle;
}

// world position
void GameObject::SetWorldPosition(const float &x, const float &y, const float &z)
{
	worldPosition = glm::vec3(x, y, z);
	if (collisionComponent != nullptr) {
		if (collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {
			collisionComponent->GetBoundingBox().c = glm::vec3(x, y, z);
		}
		else if (collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {
			collisionComponent->GetBoundingSphere().c = glm::vec3(x, y, z);
		}
	}
	if (physicsComponent != nullptr) {
		physicsComponent->SetPosition(glm::vec3(x, y, z));
	}
	UpdateWorldMatrix();
}
void GameObject::SetWorldPosition(const glm::vec3 &p)
{
	worldPosition = p;
	if (collisionComponent != nullptr) {
		if (collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::BOX) {
			collisionComponent->GetBoundingBox().c = p;
		}
		else if (collisionComponent->GetCollisionType() == CollisionComponent::Collision_Type::SPHERE) {
			collisionComponent->GetBoundingSphere().c = p;
		}
	}
	if (physicsComponent != nullptr) {
		physicsComponent->SetPosition(p);
	}
	UpdateWorldMatrix();
}
glm::vec3 GameObject::GetWorldPosition() const
{
	return worldPosition;
}

// world rotation
void GameObject::SetWorldRotation(const float &x, const float &y, const float &z, const float &a)
{
	worldRotation = glm::vec3(x, y, z);
	worldRotationAngle = a;
	if (worldRotationAngle >= 360 || worldRotationAngle <= -360) {
		worldRotationAngle = 0;
	}
	UpdateWorldMatrix();
}
void GameObject::SetWorldRotation(const glm::vec3 &r, const float &a)
{
	worldRotation = r;
	worldRotationAngle = a;
	if (worldRotationAngle >= 360 || worldRotationAngle <= -360) {
		worldRotationAngle = 0;
	}
	UpdateWorldMatrix();
}
glm::vec3 GameObject::GetWorldRotation() const
{
	return worldRotation;
}

// world scale
void GameObject::SetWorldScale(const float &x, const float &y, const float &z)
{
	worldScale = glm::vec3(x, y, z);
	if (collisionComponent != nullptr) {
		collisionComponent->SetScale(glm::vec3(x, y, z));
	}
	UpdateWorldMatrix();
}
void GameObject::SetWorldScale(const glm::vec3 &s)
{
	worldScale = s;
	if (collisionComponent != nullptr) {
		collisionComponent->SetScale(s);
	}
	UpdateWorldMatrix();
}
void GameObject::SetWorldScale(const float &s)
{
	worldScale = glm::vec3(s, s, s);
	if (collisionComponent != nullptr) {
		collisionComponent->SetScale(glm::vec3(s, s, s));
	}
	UpdateWorldMatrix();
}
glm::vec3 GameObject::GetWorldScale() const
{
	return worldScale;
}

// world rotation angle
float GameObject::GetWorldRotationAngle() const
{
	return worldRotationAngle;
}

void GameObject::UpdateLocalMatrix() {
	localModelMatrix = glm::mat4();
	localModelMatrix = glm::translate(localModelMatrix, localPosition);
	localModelMatrix = glm::rotate(localModelMatrix, localRotationAngle, localRotation);
	localModelMatrix = glm::scale(localModelMatrix, localScale);
}

glm::mat4 GameObject::GetLocalModelMatrix() const {
	return localModelMatrix;
}

void GameObject::UpdateWorldMatrix() {
	worldModelMatrix = glm::mat4();
	worldModelMatrix = glm::translate(worldModelMatrix, worldPosition);
	worldModelMatrix = glm::rotate(worldModelMatrix, worldRotationAngle, worldRotation);
	worldModelMatrix = glm::scale(worldModelMatrix, worldScale);	
}

glm::mat4 GameObject::GetWorldModelMatrix() const {
	return worldModelMatrix;
}

void GameObject::UpdatePreviousModelMatrix() {
	previousWorldModelMatrix = worldModelMatrix;
}

glm::mat4 GameObject::GetPreviousWorldMatrix() const {
	return previousWorldModelMatrix;
}

ResourceHandle<Shader> GameObject::GetShader() {
	return shader;
}

void GameObject::SetShader(ResourceHandle<Shader> shader) {
	this->shader = shader;
}

void GameObject::Update(const float deltaTime)
{
}

void GameObject::UpdateState() {
	UpdatePreviousModelMatrix();
}

void GameObject::FixedUpdate(const float deltaTime)
{
}

void GameObject::Render(Shader* shader, const double _interpolation)
{
}
