#pragma once
#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <BlueFlameEngine\Engine\Rendering\2D\UIObject.h>
#include <BlueFlameEngine\Engine\Rendering\2D\ImageUI.h>
#include <BlueFlameEngine\Engine\Rendering\2D\TextUI.h>
#include <BlueFlameEngine\Engine\Rendering\2D\SliderUI.h>

using namespace ENGINE;

namespace GAME {

	// Displays player information
	class PlayerWidget : public UIObject {

	public:
		PlayerWidget(int playerIndex, float xpos, float ypos);
		~PlayerWidget();

		void Update(const float deltaTime);
		void Draw(glm::mat4 projection);

		void SetMeterValue(int value);
		void SetMeterVisible(bool visible);

		ImageUI* GetPortraitImage() { return portrait; };

	private:
		ImageUI* portrait;
		SliderUI* meter;
		TextUI* meterText;
		ImageUI* meterBattery;
		ImageUI* specialReady;

		glm::vec3 playerColour;

		float z = 0;

		std::string GetPlayerPortrait(int playerIndex);
	};

}

#endif 