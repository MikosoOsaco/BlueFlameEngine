#pragma once
#ifndef SLIDER_H
#define SLIDER_H

#include "UIObject.h"
#include "ImageUI.h"
#include "TextUI.h"
#include <vector>

namespace ENGINE
{
	class SliderUI : public UIObject
	{
	public:
		SliderUI(char* imageFilePathBACK, char* imageFilePathFRONT);
		~SliderUI();

		void SetWidth(int width);
		void SetHeight(int height);
		void SetValue(float val);
		int GetWidth();
		int GetHeight();
		float GetValue();

		void SetImage(const std::string name, char* imageFilePathFRONT);
		ImageUI* GetImage(const std::string name);

		void SetText(TextUI* text);
		TextUI* GetText();
		void SetPadding(float x, float y);

		void SetPosition(const glm::vec3 &p) override;
		void SetPosition(const float &x, const float &y) override;

		void UpdateText();
		void Update(const float deltaTime);
		void Draw(glm::mat4 projection);

	private:

		std::vector<ImageUI*> sliderImage;

		ImageUI* imageB;
		ImageUI* imageF;
		TextUI* text;

		int width;
		int height;
		float value;

		float textPaddingX;
		float textPaddingY;
	};
}


#endif // !SLIDER_H
