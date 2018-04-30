#include "SliderUI.h"

using namespace ENGINE;
#define FRONTIMGPADDINGX 0.8f
#define FRONTIMGPADDINGY 0.8f

SliderUI::SliderUI(char* imageFilePathBACK, char* imageFilePathFRONT) : value(1)
{
	imageB = new ImageUI();
	imageB->SetName("BACK");
	imageB->SetImage(imageFilePathBACK);
	imageB->SetPosition(position);
	imageB->SetAlpha(0.5f);

	imageF = new ImageUI();
	imageF->SetName("FRONT");
	imageF->SetImage(imageFilePathFRONT);
	imageF->SetWidth(imageB->GetWidth());
	imageF->SetHeight(imageB->GetHeight());
	imageF->SetScale(FRONTIMGPADDINGX, FRONTIMGPADDINGY);
	imageF->SetPosition(imageB->GetPosition().x, imageB->GetPosition().y);
	imageF->SetAlpha(0.5f);

	text = new TextUI();
	text->SetPosition(imageB->GetPosition().x - (imageB->GetWidth() / 2) + textPaddingX, imageB->GetPosition().y + textPaddingY);
	text->SetText("100%");
	text->SetSize(0.5f);
	text->SetColour(1.0, 1.0f, 1.0f);
	textPaddingX = 140;
	textPaddingY = 15;

	sliderImage.push_back(imageB);
	sliderImage.push_back(imageF);

	width = imageB->GetWidth();
	height = imageB->GetHeight();
}

SliderUI::~SliderUI()
{
	sliderImage.empty();
	delete &sliderImage;
}

void SliderUI::SetWidth(int width)
{
	this->width = width;

	//for (int i = 0; i < sliderImage.size(); i++)
	//{
	//	sliderImage[i]->SetWidth(width);

	//	if (sliderImage[i]->GetName() == "FRONT")
	//	{
	//		sliderImage[i]->SetWidth(width * FRONTIMGPADDINGX);
	//	}
	//}
	imageB->SetWidth(width);
	imageF->SetWidth(width * (int)imageF->GetScale().x);
}

void SliderUI::SetHeight(int height)
{
	this->height = height;
	imageB->SetHeight(height);
	imageF->SetHeight(height * imageF->GetScale().y);
	//for (int i = 0; i < sliderImage.size(); i++)
	//{
	//	sliderImage[i]->SetHeight(height);

	//	if (sliderImage[i]->GetName() == "FRONT")
	//	{
	//		sliderImage[i]->SetHeight(height * FRONTIMGPADDINGY);
	//	}
	//}
}

void SliderUI::SetValue(float val)
{
	value = val;
	if (val > 1.0f)
	{
		//value = 1.0f;
	}
}

int SliderUI::GetWidth()
{
	return width;
}

int SliderUI::GetHeight()
{
	return height;
}

float SliderUI::GetValue()
{
	return value;
}

void SliderUI::SetImage(const std::string name, char* imageFilePath)
{
	for (unsigned int i = 0; i < sliderImage.size(); ++i)
	{
		if (sliderImage[i]->GetName() == name)
		{
			sliderImage[i]->SetImage(imageFilePath);
		}
	}
}

ImageUI* SliderUI::GetImage(const std::string name)
{
	for (unsigned int i = 0; i < sliderImage.size(); ++i)
	{
		if (sliderImage[i]->GetName() == name)
		{
			return sliderImage[i];
		}
	}

	std::cout << "Wrong image name" << std::endl;
	return nullptr;
}

void SliderUI::SetText(TextUI* text)
{
	this->text = text;
}

TextUI* SliderUI::GetText()
{
	return text;
}

void SliderUI::SetPadding(float x, float y)
{
	textPaddingX = x;
	textPaddingX = y;
	text->SetPosition(imageB->GetPosition().x - (imageB->GetWidth() / 2) + textPaddingX, imageB->GetPosition().y + textPaddingY);
}

void SliderUI::SetPosition(const glm::vec3 &p)
{
	position = p;
	sliderImage[0]->SetPosition(p);
	sliderImage[1]->SetPosition(p);
	text->SetPosition(imageB->GetPosition().x - (imageB->GetWidth() / 2) + textPaddingX, imageB->GetPosition().y + textPaddingY);
	//sliderImage[1]->SetPosition(sliderImage[0]->GetPosition().x - (sliderImage[0]->GetWidth() / 2) + frontImgPaddingX, sliderImage[0]->GetPosition().y + frontImgPaddingY);
}

void SliderUI::SetPosition(const float &x, const float &y)
{
	position = glm::vec3(x, y, 0.0f);
	sliderImage[0]->SetPosition(x, y);
	sliderImage[1]->SetPosition(x, y);
	text->SetPosition(imageB->GetPosition().x - (imageB->GetWidth() / 2) + textPaddingX, imageB->GetPosition().y + textPaddingY);
	//sliderImage[1]->SetPosition(sliderImage[0]->GetPosition().x - (sliderImage[0]->GetWidth() / 2) + frontImgPaddingX, sliderImage[0]->GetPosition().y + frontImgPaddingY);
}

void SliderUI::UpdateText()
{
	//conversion of percentage value to string
	//setting text value here
	text->SetText(std::to_string((int)value));
	/*
	if (value <= 0.5f)
	{
		if (value <= 0.25f)
		{
			if (value <= 0.10f)
			{
				if (value <= 0.10) { text->SetText("10%"); }
				if (value <= 0.09) { text->SetText("9%"); }
				if (value <= 0.08) { text->SetText("8%"); }
				if (value <= 0.07) { text->SetText("7%"); }
				if (value <= 0.06) { text->SetText("6%"); }
				if (value <= 0.05) { text->SetText("5%"); }
				if (value <= 0.04) { text->SetText("4%"); }
				if (value <= 0.03) { text->SetText("3%"); }
				if (value <= 0.02) { text->SetText("2%"); }
				if (value <= 0.01) { text->SetText("1%"); }
				if (value <= 0.00) { text->SetText("0%"); }
			}
			else
			{
				if (value <= 0.25) { text->SetText("25%"); }
				if (value <= 0.24) { text->SetText("24%"); }
				if (value <= 0.23) { text->SetText("23%"); }
				if (value <= 0.22) { text->SetText("22%"); }
				if (value <= 0.21) { text->SetText("21%"); }
				if (value <= 0.20) { text->SetText("20%"); }
				if (value <= 0.19) { text->SetText("19%"); }
				if (value <= 0.18) { text->SetText("18%"); }
				if (value <= 0.17) { text->SetText("17%"); }
				if (value <= 0.16) { text->SetText("16%"); }
				if (value <= 0.15) { text->SetText("15%"); }
				if (value <= 0.14) { text->SetText("14%"); }
				if (value <= 0.13) { text->SetText("13%"); }
				if (value <= 0.12) { text->SetText("12%"); }
				if (value <= 0.11) { text->SetText("11%"); }
			}
		}
		else
		{
			if (value < 0.39f)
			{
				if (value <= 0.39) { text->SetText("39%"); }
				if (value <= 0.38) { text->SetText("38%"); }
				if (value <= 0.37) { text->SetText("37%"); }
				if (value <= 0.36) { text->SetText("36%"); }
				if (value <= 0.35) { text->SetText("35%"); }
				if (value <= 0.34) { text->SetText("34%"); }
				if (value <= 0.33) { text->SetText("33%"); }
				if (value <= 0.32) { text->SetText("32%"); }
				if (value <= 0.31) { text->SetText("31%"); }
				if (value <= 0.30) { text->SetText("30%"); }
				if (value <= 0.29) { text->SetText("29%"); }
				if (value <= 0.28) { text->SetText("28%"); }
				if (value <= 0.27) { text->SetText("27%"); }
				if (value <= 0.26) { text->SetText("26%"); }
			}
			else
			{
				if (value <= 0.50) { text->SetText("50%"); }
				if (value <= 0.49) { text->SetText("49%"); }
				if (value <= 0.48) { text->SetText("48%"); }
				if (value <= 0.47) { text->SetText("47%"); }
				if (value <= 0.46) { text->SetText("46%"); }
				if (value <= 0.45) { text->SetText("45%"); }
				if (value <= 0.44) { text->SetText("44%"); }
				if (value <= 0.43) { text->SetText("43%"); }
				if (value <= 0.42) { text->SetText("42%"); }
				if (value <= 0.41) { text->SetText("41%"); }
				if (value <= 0.40) { text->SetText("40%"); }
			}
		}
	}
	else if (value > 0.5f)
	{
		if (value <= 0.75f)
		{
			if (value <= 0.60f)
			{
				if (value <= 0.60) { text->SetText("60%"); }
				if (value <= 0.59) { text->SetText("59%"); }
				if (value <= 0.58) { text->SetText("58%"); }
				if (value <= 0.57) { text->SetText("57%"); }
				if (value <= 0.56) { text->SetText("56%"); }
				if (value <= 0.55) { text->SetText("55%"); }
				if (value <= 0.54) { text->SetText("54%"); }
				if (value <= 0.53) { text->SetText("53%"); }
				if (value <= 0.52) { text->SetText("52%"); }
				if (value <= 0.51) { text->SetText("51%"); }
			}		
			else
			{
				if (value <= 0.75) { text->SetText("75%"); }
				if (value <= 0.74) { text->SetText("74%"); }
				if (value <= 0.73) { text->SetText("73%"); }
				if (value <= 0.72) { text->SetText("72%"); }
				if (value <= 0.71) { text->SetText("71%"); }
				if (value <= 0.70) { text->SetText("70%"); }
				if (value <= 0.69) { text->SetText("69%"); }
				if (value <= 0.68) { text->SetText("68%"); }
				if (value <= 0.67) { text->SetText("67%"); }
				if (value <= 0.66) { text->SetText("66%"); }
				if (value <= 0.65) { text->SetText("65%"); }
				if (value <= 0.64) { text->SetText("64%"); }
				if (value <= 0.63) { text->SetText("63%"); }
				if (value <= 0.62) { text->SetText("62%"); }
				if (value <= 0.61) { text->SetText("61%"); }
			}
		}
		else
		{
			if (value < 0.89f)
			{
				if (value <= 0.89f) { text->SetText("89%"); }
				if (value <= 0.88f) { text->SetText("88%"); }
				if (value <= 0.87f) { text->SetText("87%"); }
				if (value <= 0.86f) { text->SetText("86%"); }
				if (value <= 0.85f) { text->SetText("85%"); }
				if (value <= 0.84f) { text->SetText("84%"); }
				if (value <= 0.83f) { text->SetText("83%"); }
				if (value <= 0.82f) { text->SetText("82%"); }
				if (value <= 0.81f) { text->SetText("81%"); }
				if (value <= 0.80f) { text->SetText("80%"); }
				if (value <= 0.79f) { text->SetText("79%"); }
				if (value <= 0.78f) { text->SetText("78%"); }
				if (value <= 0.77f) { text->SetText("77%"); }
				if (value <= 0.76f) { text->SetText("76%"); }
			}
			else
			{
				if (value > 0.99f) { text->SetText("100%"); }
				if (value <= 0.99f) { text->SetText("99%"); }
				if (value <= 0.98f) { text->SetText("98%"); }
				if (value <= 0.97f) { text->SetText("97%"); }
				if (value <= 0.96f) { text->SetText("96%"); }
				if (value <= 0.95f) { text->SetText("95%"); }
				if (value <= 0.94f) { text->SetText("94%"); }
				if (value <= 0.93f) { text->SetText("93%"); }
				if (value <= 0.92f) { text->SetText("92%"); }
				if (value <= 0.91f) { text->SetText("91%"); }
				if (value <= 0.90f) { text->SetText("90%"); }
			}
		}
	}
	*/
}

void SliderUI::Update(const float deltaTime)
{
	imageF->SetWidth((int(width * (value / 100))));
	UpdateText();
	//imageF->SetPosition(position.x * (value / 2), imageF->GetPosition().y);

	imageF->SetPosition((imageB->GetPosition().x - (imageB->GetWidth() / 2) + (imageF->GetWidth() / 2)), imageF->GetPosition().y);

	for (unsigned int i = 0; i < sliderImage.size(); ++i)
	{
		if (sliderImage[i] != nullptr) {
			sliderImage[i]->Update(deltaTime);
		}
	}
	if (text != nullptr) {
		text->Update(deltaTime);
	}
}

void SliderUI::Draw(glm::mat4 projection)
{
	for (unsigned int i = 0; i < sliderImage.size(); ++i)
	{
		if (sliderImage[i] != nullptr) {
			sliderImage[i]->Draw(projection);
		}
	}
	if (text != nullptr) {
		if (text->GetVisible()) {
			text->Draw(projection);
		}
	}
}