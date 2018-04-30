#include "PlayerPortrait.h"

using namespace GAME;

PlayerPortrait::PlayerPortrait()
{
	portX = portY = textX = textY = 0;
	chartype = NONE;
	SetUpImages();
}

PlayerPortrait::PlayerPortrait(float pX, float pY, float tX, float tY)
{
	portX = pX; portY = pY;
	textX = tX; textY = tY;

	chartype = NONE;
	SetUpImages();
}

PlayerPortrait::~PlayerPortrait()
{

}

void PlayerPortrait::SetUpImages()
{
	champBLANK = new ImageUI();
	champBLANK->SetImage("Resources/Textures/CharacterSelectScreen/porEmpty.png");
	champBLANK->SetPosition(portX, portY);
	champBLANK->SetHeight(180);
	champBLANK->SetWidth(180);
	champBLANK->SetAlpha(0.0f);

	champAlexTrix = new ImageUI();
	champAlexTrix->SetImage("Resources/Textures/CharacterSelectScreen/Alex_Trix.png");
	champAlexTrix->SetPosition(portX, portY);
	champAlexTrix->SetHeight(180);
	champAlexTrix->SetWidth(180);
	champAlexTrix->SetAlpha(0.0f);

	champFlintDamascus = new ImageUI();
	champFlintDamascus->SetImage("Resources/Textures/CharacterSelectScreen/Flint_Damascus.png");
	champFlintDamascus->SetPosition(portX, portY);
	champFlintDamascus->SetHeight(180);
	champFlintDamascus->SetWidth(180);
	champFlintDamascus->SetAlpha(0.0f);

	champJackCole = new ImageUI();
	champJackCole->SetImage("Resources/Textures/CharacterSelectScreen/Jack_Cole.png");
	champJackCole->SetPosition(portX, portY);
	champJackCole->SetHeight(180);
	champJackCole->SetWidth(180);
	champJackCole->SetAlpha(0.0f);

	champKalOrr = new ImageUI();
	champKalOrr->SetImage("Resources/Textures/CharacterSelectScreen/Kal_Orr.png");
	champKalOrr->SetPosition(portX, portY);
	champKalOrr->SetHeight(180);
	champKalOrr->SetWidth(180);
	champKalOrr->SetAlpha(0.0f);

	champOkiCaeli = new ImageUI();
	champOkiCaeli->SetImage("Resources/Textures/CharacterSelectScreen/Oki_Caeli.png");
	champOkiCaeli->SetPosition(portX, portY);
	champOkiCaeli->SetHeight(180);
	champOkiCaeli->SetWidth(180);
	champOkiCaeli->SetAlpha(0.0f);

	textBlank = new ImageUI();
	textBlank->SetImage("Resources/Textures/CharacterSelectScreen/textBoxJoin.png");
	textBlank->SetPosition(textX, textY);
	textBlank->SetHeight(80);
	textBlank->SetWidth(280);

	textAlexTrix = new ImageUI();
	textAlexTrix->SetImage("Resources/Textures/CharacterSelectScreen/textBoxAlex.png");
	textAlexTrix->SetPosition(textX, textY);
	textAlexTrix->SetHeight(80);
	textAlexTrix->SetWidth(280);

	textFlintDamascus = new ImageUI();
	textFlintDamascus->SetImage("Resources/Textures/CharacterSelectScreen/textBoxFlint.png");
	textFlintDamascus->SetPosition(textX, textY);
	textFlintDamascus->SetHeight(80);
	textFlintDamascus->SetWidth(280);

	textJackCole = new ImageUI();
	textJackCole->SetImage("Resources/Textures/CharacterSelectScreen/textBoxJack.png");
	textJackCole->SetPosition(textX, textY);
	textJackCole->SetHeight(80);
	textJackCole->SetWidth(280);

	textKalOrr = new ImageUI();
	textKalOrr->SetImage("Resources/Textures/CharacterSelectScreen/textBoxKal.png");
	textKalOrr->SetPosition(textX, textY);
	textKalOrr->SetHeight(80);
	textKalOrr->SetWidth(280);

	textOkiCaeli = new ImageUI();
	textOkiCaeli->SetImage("Resources/Textures/CharacterSelectScreen/textBoxOki.png");
	textOkiCaeli->SetPosition(textX, textY);
	textOkiCaeli->SetHeight(80);
	textOkiCaeli->SetWidth(280);
}

std::vector<ImageUI*> PlayerPortrait::GetImages()
{
	std::vector<ImageUI*> imgList;
	imgList.push_back(champBLANK);
	imgList.push_back(champAlexTrix);
	imgList.push_back(champFlintDamascus);
	imgList.push_back(champJackCole);
	imgList.push_back(champKalOrr);
	imgList.push_back(champOkiCaeli);
	imgList.push_back(textBlank);
	imgList.push_back(textAlexTrix);
	imgList.push_back(textFlintDamascus);
	imgList.push_back(textJackCole);
	imgList.push_back(textKalOrr);
	imgList.push_back(textOkiCaeli);
	return imgList;
}

void PlayerPortrait::ClearImages()
{
	champBLANK->SetAlpha(0.0f);
	champAlexTrix->SetAlpha(0.0f);
	champFlintDamascus->SetAlpha(0.0f);
	champJackCole->SetAlpha(0.0f);
	champKalOrr->SetAlpha(0.0f);
	champOkiCaeli->SetAlpha(0.0f);

	textBlank->SetAlpha(0.0f);
	textAlexTrix->SetAlpha(0.0f);
	textFlintDamascus->SetAlpha(0.0f);
	textJackCole->SetAlpha(0.0f);
	textKalOrr->SetAlpha(0.0f);
	textOkiCaeli->SetAlpha(0.0f);
}

void PlayerPortrait::Update(const float deltaTime)
{
	switch (chartype)
	{
	case GAME::PlayerPortrait::NONE:
		ClearImages();
		champBLANK->SetAlpha(1.0f);
		textBlank->SetAlpha(1.0f);
		break;
	case GAME::PlayerPortrait::ALEX:
		ClearImages();
		champAlexTrix->SetAlpha(1.0f);
		textAlexTrix->SetAlpha(1.0f);
		break;
	case GAME::PlayerPortrait::FLINT:
		ClearImages();
		champFlintDamascus->SetAlpha(1.0f);
		textFlintDamascus->SetAlpha(1.0f);
		break;
	case GAME::PlayerPortrait::JACK:
		ClearImages();
		champJackCole->SetAlpha(1.0f);
		textJackCole->SetAlpha(1.0f);
		break;
	case GAME::PlayerPortrait::KAL:
		ClearImages();
		champKalOrr->SetAlpha(1.0f);
		textKalOrr->SetAlpha(1.0f);
		break;
	case GAME::PlayerPortrait::OKI:
		ClearImages();
		champOkiCaeli->SetAlpha(1.0f);
		textOkiCaeli->SetAlpha(1.0f);
		break;
	default:
		ClearImages();
		break;
	}
}

void PlayerPortrait::HandleEvents(SDL_Event events)
{

}

void PlayerPortrait::HandleStates(const Uint8 *state)
{

}
