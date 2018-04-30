#include "PlayerWidget.h"

using namespace GAME;

PlayerWidget::PlayerWidget(int playerIndex, float xpos, float ypos) {
	portrait = new ImageUI();
	portrait->SetImage(GetPlayerPortrait(playerIndex));
	portrait->SetPosition(xpos, ypos);
	portrait->SetScale(0.8f);

	meter = new SliderUI("Resources/Textures/batteryback.png", "Resources/Textures/batteryFill.png");
	meter->SetPosition(portrait->GetPosition().x + 170, ypos);
	meter->GetImage("BACK")->SetAlpha(1.0f);
	meter->GetImage("FRONT")->SetAlpha(1.0f);
	meter->GetImage("FRONT")->SetColour(playerColour);
	meter->SetPadding(50, 15);
	meter->SetWidth(250);
	meter->SetHeight(100);
	meter->SetValue(0.0f);
	meter->GetText()->SetVisible(false);

	meterText = new TextUI();
	meterText->SetFont("Resources/Fonts/ka1.ttf");
	meterText->SetText("Special");
	meterText->SetColour(1.0f, 1.0f, 1.0f);
	meterText->SetSize(0.2f);
	meterText->SetSpacing(9.0f);
	meterText->SetPosition(meter->GetPosition().x - 95, meter->GetPosition().y - 35);

	meterBattery = new ImageUI();
	meterBattery->SetImage("Resources/Textures/battery.png");
	meterBattery->SetPosition(meter->GetPosition());

	specialReady = new ImageUI();
	specialReady->SetImage("Resources/Textures/prompt.png");
	specialReady->SetPosition(meter->GetPosition().x + 80, meter->GetPosition().y - 50);
}

PlayerWidget::~PlayerWidget() {
	delete portrait;
	portrait = nullptr;
	delete meter;
	meter = nullptr;
	delete meterText;
	meterText = nullptr;
	delete meterBattery;
	meterBattery = nullptr;
	delete specialReady;
	specialReady = nullptr;
}

void PlayerWidget::SetMeterValue(int value) {
	if (value < 25) {
		meter->SetValue(0);
	}
	else if (value < 50) {
		meter->SetValue(40);
	}
	else if (value < 75) {
		meter->SetValue(60);
	}
	else if (value < 100) {
		meter->SetValue(80);
	}
	else if (value >= 100) {
		meter->SetValue(100);
	}
	else {
		meter->SetValue(0);
	}
}

void PlayerWidget::SetMeterVisible(bool visible) {
	meter->SetVisible(visible);
}

std::string PlayerWidget::GetPlayerPortrait(int playerIndex)
{
	if (playerIndex == 0) {
		playerColour = glm::vec3(1.0f, 1.0f, 0.0f);
		return "Resources/Textures/CharacterSelectScreen/Alex_Trix.png";
	}
	else if (playerIndex == 1) {
		playerColour = glm::vec3(0.8, 0.5f, 0.3f);
		return "Resources/Textures/CharacterSelectScreen/Flint_Damascus.png";
	}
	else if (playerIndex == 2) {
		playerColour = glm::vec3(0.4f, 0.7f, 1.0f);
		return "Resources/Textures/CharacterSelectScreen/Jack_Cole.png";
	}
	else if (playerIndex == 3) {
		playerColour = glm::vec3(0.7f, 0.2f, 0.1f);
		return "Resources/Textures/CharacterSelectScreen/Kal_Orr.png";
	}
	else if (playerIndex == 4) {
		playerColour = glm::vec3(0.0f, 1.0f, 0.0f);
		return "Resources/Textures/CharacterSelectScreen/Oki_Caeli.png";
	}
	else
	{
		playerColour = glm::vec3(1.0f, 1.0f, 1.0f);
		return "Resources/Textures/CharacterSelectScreen/porEmpty.png";
	}
}

void PlayerWidget::Update(const float deltaTime) {
	meter->Update(deltaTime);

	if (meter->GetValue() >= 100) {
		specialReady->SetVisible(true);
	}
	else {
		specialReady->SetVisible(false);
	}

	if (specialReady->GetVisible()) {
		float fade = (sin(z += deltaTime * 5) / 2.0f) + 0.5f;
		specialReady->SetAlpha(fade);
	}
}

void PlayerWidget::Draw(glm::mat4 projection) {
	if (meter->GetVisible()) {
		meter->Draw(projection);
		meterBattery->Draw(projection);
		meterText->Draw(projection);
		if (specialReady->GetVisible()) {
			specialReady->Draw(projection);
		}
	}
	portrait->Draw(projection);
}
