#ifndef PLAYERPORTRAIT_H
#define PLAYERPORTRAIT_H

#include <BlueFlameEngine\Engine\Rendering\2D\ImageUI.h>
#include <vector>

using namespace ENGINE;

namespace GAME {

	class PlayerPortrait
	{

	public:
		// Enum for which character selected
		enum CHARTYPE { NONE, ALEX, FLINT, JACK, KAL, OKI};

		PlayerPortrait();
		PlayerPortrait(float pX, float pY, float tX, float tY);
		~PlayerPortrait();

		void SetUpImages();

		//set character selected
		void SetCharType(CHARTYPE pN) { chartype = pN; };

		std::vector<ImageUI*> GetImages();
		CHARTYPE GetCharType() { return chartype; };
		int GetCharAmount() { return charAmount; };

		void ClearImages();

		void Update(const float deltaTime);
		void HandleEvents(SDL_Event events);
		void HandleStates(const Uint8 *state);

	private:

		float portX = 0;
		float portY = 0;
		float textX = 0;
		float textY = 0;

		CHARTYPE chartype = NONE;
		int charAmount = 6;

		ImageUI* champBLANK;
		ImageUI* champKalOrr;
		ImageUI* champAlexTrix;
		ImageUI* champFlintDamascus;
		ImageUI* champJackCole;
		ImageUI* champOkiCaeli;

		ImageUI* textBlank;
		ImageUI* textKalOrr;
		ImageUI* textAlexTrix;
		ImageUI* textFlintDamascus;
		ImageUI* textJackCole;
		ImageUI* textOkiCaeli;

	};
}
#endif // !PLAYERPORTRAIT_H
