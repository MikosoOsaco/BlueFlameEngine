#include "ButtonUI.h"

using namespace ENGINE;

ButtonUI::ButtonUI(char* imageFilePath) 
{
	paddingX = 0;
	paddingY = 0;
	textPaddingX = 0;
	textPaddingY = 0;

	image = new ImageUI();
	image->SetImage(imageFilePath);
	image->SetPosition(position);

	text = new TextUI();
	text->SetPosition(image->GetPosition().x - (image->GetWidth() / 2) + textPaddingX, image->GetPosition().y + textPaddingY);

	width = image->GetWidth();
	height = image->GetHeight();

	clickable = true;
	clicked = false;
	hovered = false;
	selected = false;
}

ButtonUI::~ButtonUI() 
{

}

bool ButtonUI::OnHover(SDL_Event events, SceneManager* sceneManager)
{
	float yt = events.button.y / (float)sceneManager->GetWindowHeight();
	float y = sceneManager->GetScreenHeight() * yt;

	float xt = events.button.x / (float)sceneManager->GetWindowWidth();
	float x = sceneManager->GetScreenWidth() * xt;

	if (y > image->GetPosition().y - (image->GetHeight() / 2) + paddingY &&
		y < image->GetPosition().y + (image->GetHeight() / 2) + paddingY &&
		x > image->GetPosition().x - (image->GetWidth() / 2) + paddingX &&
		x < image->GetPosition().x + (image->GetWidth() / 2) + paddingX)
	{
		hovered = true;
		return true;
	}
	else
	{
		hovered = false;
		return false;
	}
}

bool ButtonUI::OnHover(ImageUI* crossHair)
{
	float y = crossHair->GetPosition().y;

	float x = crossHair->GetPosition().x;

	if (y > image->GetPosition().y - (image->GetHeight() / 2) + paddingY &&
		y < image->GetPosition().y + (image->GetHeight() / 2) + paddingY &&
		x > image->GetPosition().x - (image->GetWidth() / 2) + paddingX &&
		x < image->GetPosition().x + (image->GetWidth() / 2) + paddingX)
	{
		hovered = true;
		return true;
	}
	else
	{
		hovered = false;
		return false;
	}
}

void ButtonUI::OnClick()
{
	clicked = true;
}

void ButtonUI::EnableClickable(bool clickable)
{
	this->clickable = clickable;
}

void ButtonUI::SetWidth(int width)
{
	this->width = width;
	image->SetWidth(width);
}

void ButtonUI::SetHeight(int height)
{
	this->height = height;
	image->SetHeight(height);
}

int ButtonUI::GetWidth() 
{
	return width;
}

int ButtonUI::GetHeight()
{
	return height;
}

void ButtonUI::SetImage(ImageUI* image)
{
	this->image = image;
}

ImageUI* ButtonUI::GetImage()
{
	return image;
}

void ButtonUI::SetText(TextUI* text)
{
	this->text = text;
}

TextUI* ButtonUI::GetText()
{
	return text;
}

void ButtonUI::SetPadding(float x, float y)
{
	paddingX = x;
	paddingY = y;
}

void ButtonUI::SetTextPadding(float x, float y)
{
	textPaddingX = x;
	textPaddingY = y;
	text->SetPosition(image->GetPosition().x - (image->GetWidth() / 2) + textPaddingX, image->GetPosition().y + textPaddingY);
}

void ButtonUI::SetSelected(bool selected) {
	this->selected = selected;
}

bool ButtonUI::GetSelected() {
	return selected;
}

void ButtonUI::SetPosition(const glm::vec3 &p)
{
	position = p;
	image->SetPosition(p);
	text->SetPosition(image->GetPosition().x - (image->GetWidth() / 2) + textPaddingX, image->GetPosition().y + textPaddingY);
}

void ButtonUI::SetPosition(const float &x, const float &y)
{
	position = glm::vec3(x, y, 0.0f);
	image->SetPosition(x, y);
	text->SetPosition(image->GetPosition().x - (image->GetWidth() / 2) + textPaddingX, image->GetPosition().y + textPaddingY);
}

void ButtonUI::Update(const float deltaTime) 
{
	if (image != nullptr) {
		image->Update(deltaTime);
	}
	if (text != nullptr) {
		text->Update(deltaTime);
	}
}

void ButtonUI::Draw(glm::mat4 projection)
{
	if (image != nullptr) {
		image->Draw(projection);
	}
	if (text != nullptr) {
		text->Draw(projection);
	}
}