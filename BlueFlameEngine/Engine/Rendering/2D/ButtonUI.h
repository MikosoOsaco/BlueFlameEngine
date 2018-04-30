#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include "UIObject.h"
#include "ImageUI.h"
#include "TextUI.h"
#include "../../Core/SceneManager.h"

namespace ENGINE {

	class ButtonUI : public UIObject
	{
	public:
		ButtonUI(char* imageFilePath);
		~ButtonUI();

		bool OnHover(SDL_Event events, SceneManager* sceneManager);
		bool OnHover(ImageUI* crossHair);
		void OnClick();
		void EnableClickable(bool clickable);
		void SetWidth(int width);
		void SetHeight(int height);
		int GetWidth();
		int GetHeight();
		void SetImage(ImageUI* image);
		ImageUI* GetImage();
		void SetText(TextUI* text);
		TextUI* GetText();
		void SetPadding(float x, float y);
		void SetTextPadding(float x, float y);

		void SetSelected(bool selected);
		bool GetSelected();

		void SetPosition(const glm::vec3 &p) override;
		void SetPosition(const float &x, const float &y) override;

		void Update(const float deltaTime);
		void Draw(glm::mat4 projection);

	private:
		ImageUI* image;
		TextUI* text;
		int width;
		int height;
		float paddingX;
		float paddingY;
		float textPaddingX;
		float textPaddingY;

		bool clickable;
		bool clicked;
		bool hovered;
		bool selected;
	};
}
#endif