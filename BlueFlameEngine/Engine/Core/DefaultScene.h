#pragma once
#ifndef DEFAULTSCENE_H
#define DEFAULTSCENE_H

#include "Scene.h"
#include "../Rendering/2D/TextUI.h"

namespace ENGINE {

	class DefaultScene : public Scene
	{
	public:
		DefaultScene();
		~DefaultScene();

		bool Initialize();
		void Update(const float deltaTime);
		void FixedUpdate(const float deltaTime);
		void HandleEvents(SDL_Event events);
		void HandleStates(const Uint8 *state);

	private:
		TextUI* text;
	};
}
#endif

