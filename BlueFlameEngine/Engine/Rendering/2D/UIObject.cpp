#include "UIObject.h"

using namespace ENGINE;

UIObject::UIObject() : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 1.0f), rotationAngle(0.0f), scale(1.0f, 1.0, 1.0f)
{
}

UIObject::~UIObject()
{
}

void UIObject::SetPosition(const float &x, const float &y)
{
	position = glm::vec3(x, y, 0.0f);
}

void UIObject::SetPosition(const glm::vec3 &p)
{
	position = p;
}

void UIObject::SetRotation(const float &x, const float &y, const float &z)
{
	rotation = glm::vec3(x, y, z);
}

void UIObject::SetRotation(const glm::vec3 &p)
{
	rotation = p;
}

void UIObject::SetRotationAngle(const float &a)
{
	rotationAngle = a;
}

void UIObject::SetScale(const float &x, const float &y)
{
	scale = glm::vec3(x, y, 1.0f);
}

void UIObject::SetScale(const glm::vec3 &s)
{
	scale = s;
}

void UIObject::SetScale(const float &s)
{
	scale = glm::vec3(s, s, 1.0f);
}

void UIObject::SetVisible(bool setVisible)
{
	visible = setVisible;
}

glm::vec3 UIObject::GetPosition() const
{
	return position;
}

glm::vec3 UIObject::GetRotation() const
{
	return rotation;
}

float UIObject::GetRotationAngle() const
{
	return rotationAngle;
}

glm::vec3 UIObject::GetScale() const
{
	return scale;
}

bool UIObject::GetVisible() const
{
	return visible;
}
